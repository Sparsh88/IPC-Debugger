#include <stdio.h>
#include <windows.h>
#include <string.h>

/* ============================================================
   ADVANCED IPC DEBUGGER (SIMULATION BASED)
   ------------------------------------------------------------
   PLATFORM : WINDOWS
   LANGUAGE : C
   PURPOSE  : OS LAB / IPC DEBUGGER PROJECT
   ============================================================ */

/* ============================ CONSTANTS ============================ */

#define MAX_BUFFER         256
#define MAX_EVENTS         100
#define MAX_THREADS        20
#define MAX_NAME_LEN       64
#define MAX_LOG_LEN        256

/* ============================ ENUMS ============================ */

typedef enum {
    THREAD_CREATED,
    THREAD_RUNNING,
    THREAD_WAITING,
    THREAD_TERMINATED
} thread_state;

typedef enum {
    IPC_PIPE,
    IPC_MESSAGE_QUEUE,
    IPC_SHARED_MEMORY
} ipc_type;

/* ============================ STRUCTURES ============================ */

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    thread_state state;
} ThreadInfo;

typedef struct {
    char description[MAX_LOG_LEN];
} EventRecord;

/* ============================ GLOBAL IPC DATA ============================ */

char pipe_buffer[MAX_BUFFER] = "";
char message_queue[MAX_BUFFER] = "";
char shared_memory[MAX_BUFFER] = "";

/* ============================ GLOBAL METRICS ============================ */

int pipe_ops = 0;
int mq_ops   = 0;
int shm_ops  = 0;

/* ============================ THREAD REGISTRY ============================ */

ThreadInfo thread_table[MAX_THREADS];
int thread_count = 0;

/* ============================ EVENT HISTORY ============================ */

EventRecord event_log[MAX_EVENTS];
int event_count = 0;

/* ============================ SYNCHRONIZATION ============================ */

CRITICAL_SECTION pipe_cs;
CRITICAL_SECTION mq_cs;
CRITICAL_SECTION shm_cs;
CRITICAL_SECTION registry_cs;
CRITICAL_SECTION log_cs;
CRITICAL_SECTION resource_A;
CRITICAL_SECTION resource_B;

/* ============================ WINDOWS EVENTS ============================ */

HANDLE pipe_event;
HANDLE mq_event;
HANDLE shm_event;

/* ============================ UTILITY FUNCTIONS ============================ */

void add_event(const char *msg) {
    if (event_count < MAX_EVENTS) {
        strcpy(event_log[event_count].description, msg);
        event_count++;
    }
}

void register_thread(const char *name, thread_state state) {
    EnterCriticalSection(&registry_cs);

    if (thread_count < MAX_THREADS) {
        thread_table[thread_count].id = thread_count + 1;
        strcpy(thread_table[thread_count].name, name);
        thread_table[thread_count].state = state;
        thread_count++;
    }

    LeaveCriticalSection(&registry_cs);
}

void update_thread_state(const char *name, thread_state new_state) {
    EnterCriticalSection(&registry_cs);

    for (int i = 0; i < thread_count; i++) {
        if (strcmp(thread_table[i].name, name) == 0) {
            thread_table[i].state = new_state;
            break;
        }
    }

    LeaveCriticalSection(&registry_cs);
}

/* ============================ LOGGER ============================ */

void log_event(const char *module, const char *event) {
    EnterCriticalSection(&log_cs);
    FILE *fp = fopen("ipc_log.txt", "a");
    if (!fp) {
        LeaveCriticalSection(&log_cs);
        return;
    }

    SYSTEMTIME t;
    GetLocalTime(&t);

    fprintf(fp, "[%02d:%02d:%02d] [%s] %s\n",
            t.wHour, t.wMinute, t.wSecond, module, event);
    fclose(fp);

    add_event(event);
    LeaveCriticalSection(&log_cs);
}

/* ============================ PIPE IPC ============================ */

DWORD WINAPI pipe_sender(LPVOID lpParam) {
    register_thread("Pipe-Sender", THREAD_CREATED);
    update_thread_state("Pipe-Sender", THREAD_RUNNING);

    EnterCriticalSection(&pipe_cs);
    strcpy(pipe_buffer, "Hello via Pipe IPC");
    pipe_ops++;
    printf("[PIPE] Sender wrote data\n");
    log_event("PIPE", "Pipe sender wrote data");
    LeaveCriticalSection(&pipe_cs);

    SetEvent(pipe_event);

    update_thread_state("Pipe-Sender", THREAD_TERMINATED);
    return 0;
}

DWORD WINAPI pipe_receiver(LPVOID lpParam) {
    register_thread("Pipe-Receiver", THREAD_CREATED);
    update_thread_state("Pipe-Receiver", THREAD_RUNNING);

    update_thread_state("Pipe-Receiver", THREAD_WAITING);
    WaitForSingleObject(pipe_event, INFINITE);
    update_thread_state("Pipe-Receiver", THREAD_RUNNING);

    EnterCriticalSection(&pipe_cs);
    printf("[PIPE] Receiver read: %s\n", pipe_buffer);
    log_event("PIPE", "Pipe receiver read data");
    LeaveCriticalSection(&pipe_cs);

    update_thread_state("Pipe-Receiver", THREAD_TERMINATED);
    return 0;
}

/* ============================ MESSAGE QUEUE IPC ============================ */

DWORD WINAPI mq_sender(LPVOID lpParam) {
    register_thread("MQ-Sender", THREAD_CREATED);
    update_thread_state("MQ-Sender", THREAD_RUNNING);

    EnterCriticalSection(&mq_cs);
    strcpy(message_queue, "Hello via Message Queue IPC");
    mq_ops++;
    printf("[MSG QUEUE] Message enqueued\n");
    log_event("MESSAGE_QUEUE", "Message enqueued");
    LeaveCriticalSection(&mq_cs);

    SetEvent(mq_event);

    update_thread_state("MQ-Sender", THREAD_TERMINATED);
    return 0;
}

DWORD WINAPI mq_receiver(LPVOID lpParam) {
    register_thread("MQ-Receiver", THREAD_CREATED);
    update_thread_state("MQ-Receiver", THREAD_RUNNING);

    update_thread_state("MQ-Receiver", THREAD_WAITING);
    WaitForSingleObject(mq_event, INFINITE);
    update_thread_state("MQ-Receiver", THREAD_RUNNING);

    EnterCriticalSection(&mq_cs);
    printf("[MSG QUEUE] Message dequeued: %s\n", message_queue);
    log_event("MESSAGE_QUEUE", "Message dequeued");
    LeaveCriticalSection(&mq_cs);

    update_thread_state("MQ-Receiver", THREAD_TERMINATED);
    return 0;
}

/* ============================ SHARED MEMORY IPC ============================ */

DWORD WINAPI shm_writer(LPVOID lpParam) {
    register_thread("SHM-Writer", THREAD_CREATED);
    update_thread_state("SHM-Writer", THREAD_RUNNING);

    EnterCriticalSection(&shm_cs);
    strcpy(shared_memory, "Hello via Shared Memory IPC");
    shm_ops++;
    printf("[SHM] Data written\n");
    log_event("SHARED_MEMORY", "Shared memory written");
    LeaveCriticalSection(&shm_cs);

    SetEvent(shm_event);

    update_thread_state("SHM-Writer", THREAD_TERMINATED);
    return 0;
}

DWORD WINAPI shm_reader(LPVOID lpParam) {
    register_thread("SHM-Reader", THREAD_CREATED);
    update_thread_state("SHM-Reader", THREAD_RUNNING);

    update_thread_state("SHM-Reader", THREAD_WAITING);
    WaitForSingleObject(shm_event, INFINITE);
    update_thread_state("SHM-Reader", THREAD_RUNNING);

    EnterCriticalSection(&shm_cs);
    printf("[SHM] Data read: %s\n", shared_memory);
    log_event("SHARED_MEMORY", "Shared memory read");
    LeaveCriticalSection(&shm_cs);

    update_thread_state("SHM-Reader", THREAD_TERMINATED);
    return 0;
}

/* ============================ DEADLOCK SIMULATION ============================ */

DWORD WINAPI deadlock_thread_1(LPVOID lpParam) {
    register_thread("Deadlock-1", THREAD_CREATED);
    update_thread_state("Deadlock-1", THREAD_RUNNING);

    EnterCriticalSection(&resource_A);
    printf("[DEADLOCK] Thread 1 acquired Resource A\n");
    log_event("DEADLOCK", "Thread 1 acquired Resource A");
    
    Sleep(100);

    update_thread_state("Deadlock-1", THREAD_WAITING);
    printf("[DEADLOCK] Thread 1 waiting for Resource B...\n");
    EnterCriticalSection(&resource_B);
    
    // Unreachable
    LeaveCriticalSection(&resource_B);
    LeaveCriticalSection(&resource_A);
    update_thread_state("Deadlock-1", THREAD_TERMINATED);
    return 0;
}

DWORD WINAPI deadlock_thread_2(LPVOID lpParam) {
    register_thread("Deadlock-2", THREAD_CREATED);
    update_thread_state("Deadlock-2", THREAD_RUNNING);

    EnterCriticalSection(&resource_B);
    printf("[DEADLOCK] Thread 2 acquired Resource B\n");
    log_event("DEADLOCK", "Thread 2 acquired Resource B");
    
    Sleep(100);

    update_thread_state("Deadlock-2", THREAD_WAITING);
    printf("[DEADLOCK] Thread 2 waiting for Resource A...\n");
    EnterCriticalSection(&resource_A);
    
    // Unreachable
    LeaveCriticalSection(&resource_A);
    LeaveCriticalSection(&resource_B);
    update_thread_state("Deadlock-2", THREAD_TERMINATED);
    return 0;
}

void simulate_deadlock() {
    printf("\n[DEADLOCK] Simulating circular wait...\n");
    log_event("DEADLOCK", "Deadlock simulation executed");
    
    HANDLE dt1 = CreateThread(NULL, 0, deadlock_thread_1, NULL, 0, NULL);
    HANDLE dt2 = CreateThread(NULL, 0, deadlock_thread_2, NULL, 0, NULL);
    
    if (dt1) CloseHandle(dt1);
    if (dt2) CloseHandle(dt2);
}

/* ============================ DASHBOARDS ============================ */

void show_thread_table() {
    printf("\n------ THREAD TABLE ------\n");
    for (int i = 0; i < thread_count; i++) {
        printf("ID:%d Name:%s State:%d\n",
               thread_table[i].id,
               thread_table[i].name,
               thread_table[i].state);
    }
}

void show_event_history() {
    printf("\n------ EVENT HISTORY ------\n");
    for (int i = 0; i < event_count; i++) {
        printf("%d. %s\n", i + 1, event_log[i].description);
    }
}

void show_dashboard() {
    printf("\n========== IPC DASHBOARD ==========\n");
    printf("Pipe Ops        : %d\n", pipe_ops);
    printf("Message Queue   : %d\n", mq_ops);
    printf("Shared Memory   : %d\n", shm_ops);
    printf("==================================\n");
}

/* ============================ MENU ============================ */

void menu() {
    printf("\n========== IPC DEBUGGER MENU ==========\n");
    printf("1. Pipe IPC\n");
    printf("2. Message Queue IPC\n");
    printf("3. Shared Memory IPC\n");
    printf("4. Show Dashboard\n");
    printf("5. Show Thread Table\n");
    printf("6. Show Event History\n");
    printf("7. Simulate Deadlock\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
}

/* ============================ MAIN ============================ */

int main() {
    int choice;
    HANDLE t1, t2;

    InitializeCriticalSection(&pipe_cs);
    InitializeCriticalSection(&mq_cs);
    InitializeCriticalSection(&shm_cs);
    InitializeCriticalSection(&registry_cs);
    InitializeCriticalSection(&log_cs);
    InitializeCriticalSection(&resource_A);
    InitializeCriticalSection(&resource_B);

    pipe_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    mq_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    shm_event = CreateEvent(NULL, FALSE, FALSE, NULL);

    log_event("SYSTEM", "IPC Debugger Started");


    do {
        menu();
        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            choice = -1;
        }

        switch (choice) {

            case 1:
                t1 = CreateThread(NULL, 0, pipe_sender, NULL, 0, NULL);
                t2 = CreateThread(NULL, 0, pipe_receiver, NULL, 0, NULL);
                if (t1) WaitForSingleObject(t1, INFINITE);
                if (t2) WaitForSingleObject(t2, INFINITE);
                if (t1) CloseHandle(t1);
                if (t2) CloseHandle(t2);
                break;

            case 2:
                t1 = CreateThread(NULL, 0, mq_sender, NULL, 0, NULL);
                t2 = CreateThread(NULL, 0, mq_receiver, NULL, 0, NULL);
                if (t1) WaitForSingleObject(t1, INFINITE);
                if (t2) WaitForSingleObject(t2, INFINITE);
                if (t1) CloseHandle(t1);
                if (t2) CloseHandle(t2);
                break;

            case 3:
                t1 = CreateThread(NULL, 0, shm_writer, NULL, 0, NULL);
                t2 = CreateThread(NULL, 0, shm_reader, NULL, 0, NULL);
                if (t1) WaitForSingleObject(t1, INFINITE);
                if (t2) WaitForSingleObject(t2, INFINITE);
                if (t1) CloseHandle(t1);
                if (t2) CloseHandle(t2);
                break;

            case 4:
                show_dashboard();
                break;

            case 5:
                show_thread_table();
                break;

            case 6:
                show_event_history();
                break;

            case 7:
                simulate_deadlock();
                break;

            case 0:
                log_event("SYSTEM", "IPC Debugger Terminated");
                printf("Exiting IPC Debugger...\n");
                break;

            default:
                printf("Invalid option\n");
        }

    } while (choice != 0);

    DeleteCriticalSection(&pipe_cs);
    DeleteCriticalSection(&mq_cs);
    DeleteCriticalSection(&shm_cs);
    DeleteCriticalSection(&registry_cs);
    DeleteCriticalSection(&log_cs);
    DeleteCriticalSection(&resource_A);
    DeleteCriticalSection(&resource_B);

    if (pipe_event) CloseHandle(pipe_event);
    if (mq_event) CloseHandle(mq_event);
    if (shm_event) CloseHandle(shm_event);

    return 0;
}
