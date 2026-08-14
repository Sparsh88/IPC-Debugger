# IPC Debugger

A multi-threaded Windows C console application for simulating, synchronizing, and inspecting Inter-Process Communication (IPC) mechanisms, thread lifecycle states, and concurrency deadlocks.

---

## GitHub Repository

- **GitHub Repository**: [https://github.com/Sparsh88/IPC-Debugger](https://github.com/Sparsh88/IPC-Debugger)

---

## Overview

**IPC Debugger** is a simulation and diagnostic console utility written in C for the Windows operating system. Developed to explore core operating system principles, it models primary inter-process communication mechanisms—**Pipes**, **Message Queues**, and **Shared Memory**—using native Win32 multi-threading and synchronization primitives.

The project provides interactive tools to observe how concurrent threads exchange data, transition through execution states, handle mutex locks, and recover or stall during resource contention. It features a built-in thread registry, an in-memory event buffer, an aggregate operation metrics dashboard, and thread-safe persistent logging.

Designed as an educational and technical portfolio demonstration for systems programming and operating systems concepts, it illustrates low-level Win32 thread management, critical sections, event signaling, and deadlock reproduction.

---

## Problem Statement

* **Theoretical Concurrency Concepts**: IPC mechanisms, mutex locks, and thread synchronization are often studied theoretically without hands-on runtime visibility into data handoffs.
* **Race Conditions & Lock Contention**: Unsynchronized concurrent memory access leads to undefined behavior and race conditions that require strict synchronization boundaries.
* **Lack of State Transparency**: Standard terminal utilities often hide thread lifecycle transitions (`CREATED`, `RUNNING`, `WAITING`, `TERMINATED`) during execution.
* **Deadlock Demonstration**: Understanding circular wait conditions requires a controlled environment that safely illustrates how un-ordered lock acquisition causes permanent execution stalls.

---

## Key Features

### 1. Multi-Threaded IPC Simulations
* **Pipe IPC Simulation**: Spawns sender and receiver threads (`Pipe-Sender`, `Pipe-Receiver`) to perform synchronized message passing through a dedicated pipe buffer using Win32 event notifications.
* **Message Queue Simulation**: Simulates asynchronous message delivery where the producer thread enqueues messages and triggers receiver threads via signaled events.
* **Shared Memory Simulation**: Models shared memory read/write operations across threads protected by mutual exclusion locks.

### 2. Win32 Concurrency & Synchronization
* **Critical Sections**: Implements 7 distinct `CRITICAL_SECTION` objects (`pipe_cs`, `mq_cs`, `shm_cs`, `registry_cs`, `log_cs`, `resource_A`, `resource_B`) ensuring thread safety across shared buffers, registries, logs, and simulated resources.
* **Event Signaling**: Utilizes auto-reset Win32 event handles (`CreateEvent`, `SetEvent`, `WaitForSingleObject`) to coordinate producer-consumer handoffs without busy waiting.

### 3. Thread Lifecycle Registry
* Maintains an in-memory registry table (`ThreadInfo thread_table[MAX_THREADS]`) tracking each thread's assigned ID, descriptive name, and active state (`THREAD_CREATED`, `THREAD_RUNNING`, `THREAD_WAITING`, `THREAD_TERMINATED`).
* Dynamic state updates at key execution milestones (e.g., waiting for event signal, running in critical section, terminating upon task completion).

### 4. Deadlock Simulation
* Models a classic two-thread circular wait condition:
  * `Deadlock-1` acquires `Resource A`, sleeps 100ms, and attempts to acquire `Resource B`.
  * `Deadlock-2` acquires `Resource B`, sleeps 100ms, and attempts to acquire `Resource A`.
* Demonstrates how inverted lock ordering causes mutual waiting and execution blockages.

### 5. Metrics Dashboard & Event History
* **IPC Metrics Dashboard**: Displays cumulative operation counters for completed Pipe, Message Queue, and Shared Memory transactions (`pipe_ops`, `mq_ops`, `shm_ops`).
* **Event History Buffer**: Stores up to 100 recent system and thread events in memory for fast CLI inspection.

### 6. Thread-Safe Persistent Logging
* Automatically writes timestamped entries (`[HH:MM:SS] [MODULE] EVENT`) to `ipc_log.txt` under `log_cs` protection.
* Logs application lifecycle events, data transmissions, and deadlock triggers.

---

## Tech Stack

| Category | Technology | Purpose |
|---|---|---|
| **Language** | C (C99 / Standard C) | Core simulation logic, data structures, and CLI interface |
| **Platform** | Microsoft Windows | Native target execution environment |
| **System API** | Win32 API (`windows.h`) | Thread creation, event signaling, critical sections, and timing |
| **Threading** | Win32 Threads (`CreateThread`) | Asynchronous worker, sender, receiver, and deadlock threads |
| **Synchronization** | `CRITICAL_SECTION`, `HANDLE` (Events) | Mutual exclusion locks and thread notification signals |
| **File I/O** | Standard C File I/O (`stdio.h`) | Persistent disk logging to `ipc_log.txt` |

---

## Architecture

```text
+-------------------------------------------------------------+
|                      User CLI Interface                     |
|                 (Interactive Console Menu)                  |
+------------------------------+------------------------------+
                               |
                               v
+-------------------------------------------------------------+
|                     Main Thread Controller                  |
|          Initializes Synchronization Primitives & Menu      |
+----+--------------------+--------------------+--------------+
     |                    |                    |
     v                    v                    v
+----+------------+ +-----+------------+ +-----+------------+
|   Pipe IPC      | | Message Queue    | | Shared Memory    |
| - pipe_sender   | | - mq_sender      | | - shm_writer     |
| - pipe_receiver | | - mq_receiver    | | - shm_reader     |
+----+------------+ +-----+------------+ +-----+------------+
     |                    |                    |
     +--------------------+--------------------+
                          |
                          v
+-------------------------------------------------------------+
|                Win32 Synchronization Layer                  |
| - Critical Sections (pipe_cs, mq_cs, shm_cs, registry_cs)   |
| - Win32 Events (pipe_event, mq_event, shm_event)            |
+----+---------------------------------------------------+----+
     |                                                   |
     v                                                   v
+----+------------------------+     +--------------------+----+
| Thread Registry & Dashboard |     |  Persistent Logger      |
| - thread_table[]            |     | - ipc_log.txt           |
| - event_log[] & Counters    |     | - log_cs protection     |
+-----------------------------+     +-------------------------+
```

---

## Application Flow

1. **System Initialization**:
   * Initializes 7 critical sections (`pipe_cs`, `mq_cs`, `shm_cs`, `registry_cs`, `log_cs`, `resource_A`, `resource_B`).
   * Creates 3 auto-reset Win32 event handles (`pipe_event`, `mq_event`, `shm_event`).
   * Writes the initial `[SYSTEM] IPC Debugger Started` entry to `ipc_log.txt`.

2. **User Menu Selection**:
   * The user selects an option (1–7, 0 to exit) from the interactive CLI menu.

3. **Thread Dispatch & Registration**:
   * For IPC options (1, 2, or 3), the main process spawns sender and receiver threads via `CreateThread`.
   * Each thread registers itself in `thread_table` and sets its initial state to `THREAD_CREATED` and then `THREAD_RUNNING`.

4. **Synchronized Data Transfer**:
   * The sender thread enters its critical section, updates the respective shared buffer (`pipe_buffer`, `message_queue`, or `shared_memory`), increments the operation counter, logs the event, leaves the critical section, and signals the event handle via `SetEvent`.
   * The receiver thread transitions to `THREAD_WAITING`, waits on the event handle via `WaitForSingleObject`, transitions to `THREAD_RUNNING`, reads the buffer under critical section protection, and logs the read event.

5. **Thread Termination & Cleanup**:
   * Worker threads transition their status to `THREAD_TERMINATED` in the registry table.
   * The main thread waits for both thread handles (`WaitForSingleObject`) and releases them via `CloseHandle`.

6. **Inspection & Reporting**:
   * Choosing option 4 displays real-time operation counters on the IPC Dashboard.
   * Choosing option 5 prints the Thread Table showing registered thread IDs, names, and states.
   * Choosing option 6 prints the in-memory Event History log.

7. **Deadlock Simulation (Option 7)**:
   * Spawns `Deadlock-1` and `Deadlock-2` which acquire `Resource A` and `Resource B` respectively in reverse order with a 100ms delay, demonstrating unresolvable circular lock waiting.

8. **Application Shutdown (Option 0)**:
   * Logs `[SYSTEM] IPC Debugger Terminated`, deletes all critical sections, closes event handles, and cleanly exits the program.

---

## Project Structure

```text
IPC-Debugger/
├── .vscode/
│   └── launch.json        # Visual Studio Code C/C++ debug launch configurations
├── ipc_debugger.c         # Complete C source code (IPC logic, threads, mutexes, CLI)
├── ipc_debugger.exe       # Compiled native Windows executable
├── ipc_log.txt            # Generated persistent log file with timestamps
└── README.md              # Project documentation
```

---

## Getting Started

### Prerequisites

* **Operating System**: Microsoft Windows (10 / 11 / Server)
* **C Compiler**: GCC (MinGW / MSYS2) or MSVC (`cl.exe`)

### Compilation

Using **GCC (MinGW)**:
```cmd
gcc ipc_debugger.c -o ipc_debugger.exe
```

Using **MSVC Developer Command Prompt**:
```cmd
cl ipc_debugger.c /Fe:ipc_debugger.exe
```

### Running the Application

Execute the compiled binary from Command Prompt or PowerShell:
```cmd
ipc_debugger.exe
```

### Menu Options Reference

```text
========== IPC DEBUGGER MENU ==========
1. Pipe IPC           - Spawns sender/receiver threads for Pipe simulation
2. Message Queue IPC  - Spawns sender/receiver threads for Message Queue simulation
3. Shared Memory IPC  - Spawns writer/reader threads for Shared Memory simulation
4. Show Dashboard     - Displays cumulative IPC operation metrics
5. Show Thread Table  - Displays active/historical thread states
6. Show Event History - Displays recent in-memory event records
7. Simulate Deadlock  - Demonstrates circular wait contention with two worker threads
0. Exit               - Cleans up handles/mutexes and terminates application
```
