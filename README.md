# 🧠 IPC Debugger (Simulation-Based)

A simulation-based **Inter-Process Communication (IPC) Debugging and Monitoring Tool** developed in **C** for Windows systems.

This project demonstrates and analyzes the working of major IPC mechanisms—**Pipes, Message Queues, and Shared Memory**—using multithreading, synchronization, and logging techniques.

Since Windows does not natively support POSIX IPC system calls, the project implements a conceptual simulation that preserves the theoretical behavior of IPC mechanisms while remaining executable on Windows.

---

# 🎯 Objectives

- Demonstrate Inter-Process Communication (IPC) concepts.
- Simulate Pipe, Message Queue, and Shared Memory communication.
- Monitor IPC operations using thread state tracking.
- Log IPC events for debugging and analysis.
- Build an Operating Systems lab-ready IPC Debugger project.

---

# ✨ Features

- Pipe IPC Simulation
- Message Queue IPC Simulation
- Shared Memory IPC Simulation
- Multithreaded Execution using Windows Threads
- Thread Lifecycle and State Monitoring
- Centralized Logging (`ipc_log.txt`)
- IPC Operation Counters and Statistics
- Event History Tracking
- Deadlock Simulation Module
- Menu-Driven Debugger Interface
- Graceful Program Termination

---

# 🏗️ System Architecture

```
+----------------------+
|      User Menu       |
+----------------------+
            |
            ▼
+----------------------+
|  IPC Simulation Layer|
|  - Pipe              |
|  - Message Queue     |
|  - Shared Memory     |
+----------------------+
            |
            ▼
+----------------------+
|    Synchronization   |
|  (Critical Sections) |
+----------------------+
            |
            ▼
+----------------------+
| Debugging & Logging  |
+----------------------+
```

---

# 🛠️ Technologies Used

| Component | Technology |
|------------|------------|
| Programming Language | C |
| Operating System | Windows |
| Compiler | MinGW (GCC) |
| IDE | Visual Studio Code |
| Threading Model | Windows Threads |
| Synchronization | Critical Sections |

---

# 📁 Repository Structure

```text
IPC-Debugger/
│
├── ipc_debugger.c     # Main source code
├── ipc_log.txt        # Generated log file
└── README.md          # Project documentation
```

---

# ⚙️ Installation

## Prerequisites

Make sure the following are installed:

- MinGW (GCC)
- Visual Studio Code (optional)
- Windows Operating System

Also ensure that MinGW is added to the system PATH.

---

# ▶️ How to Compile

```bash
gcc ipc_debugger.c -o ipc_debugger
```

---

# ▶️ How to Run

```bash
.\ipc_debugger
```

---

# ❌ Exit the Program

Select the following option from the menu:

```
Exit
```

The program terminates gracefully without requiring `Ctrl + C`.

---

# 📊 Sample Output

```text
===== ADVANCED IPC DEBUGGER =====

1. Pipe IPC
2. Message Queue IPC
3. Shared Memory IPC
4. Show Dashboard
5. Exit

[PIPE] Sender wrote data
[PIPE] Receiver read: Hello via Pipe IPC
```

---

# 🗂️ Logging and Debugging

All IPC activities are logged with timestamps inside:

```text
ipc_log.txt
```

### Sample Log Output

```text
[14:42:10] [PIPE] Pipe sender wrote data
[14:42:11] [MESSAGE_QUEUE] Message enqueued
[14:42:12] [SHARED_MEMORY] Shared memory written
```

The log file helps in:

- Tracking IPC operations
- Debugging communication behavior
- Analyzing execution flow
- Monitoring thread activities

---

# 📈 Supported IPC Simulations

## Pipe Communication

- Sender thread writes data.
- Receiver thread reads data.
- Simulates unidirectional communication.

### Example

```text
Sender → Pipe → Receiver
```

---

## Message Queue Communication

- Messages are stored in a queue.
- Sender pushes messages.
- Receiver consumes messages.

### Example

```text
Producer → Queue → Consumer
```

---

## Shared Memory Communication

- Multiple threads access a common memory region.
- Synchronization is handled using critical sections.

### Example

```text
Thread A
    ↓
Shared Memory
    ↑
Thread B
```

---

# 🔒 Synchronization Mechanism

The project uses **Windows Critical Sections** to avoid race conditions during shared resource access.

Functions performed:

- Thread synchronization
- Safe memory access
- Preventing inconsistent data states

---

# 📊 Monitoring Features

The debugger keeps track of:

- Thread states
- IPC event history
- Message statistics
- Operation counters
- Log records
- Execution flow

---

# ⚠️ Limitations

- This project simulates IPC behavior.
- It does not use kernel-level IPC mechanisms.
- POSIX IPC APIs are not implemented because Windows lacks native support.
- Intended primarily for academic and educational purposes.

---

# 🎓 Academic Relevance

This project is suitable for:

- Operating Systems Laboratory
- Process Synchronization Topics
- Inter-Process Communication Topics
- OS Practical Examinations
- Mini Projects
- Lab Project Submissions
- Viva Demonstrations

---

# 🎤 Viva Explanation

> "This project is a simulation-based IPC debugger that demonstrates Pipe, Message Queue, and Shared Memory communication using multithreading and synchronization techniques in C."

---

# 🚀 Future Enhancements

- Linux Version using Real POSIX IPC
- Graphical Dashboard for IPC Monitoring
- Performance Benchmarking
- IPC Fault Injection and Recovery Simulation
- Real-Time Statistics Visualization
- Thread Priority Monitoring
- Cross-Platform Support

---

# 📷 Sample Workflow

```text
User
 ↓
Select IPC Type
 ↓
Create Threads
 ↓
Simulate Communication
 ↓
Synchronization
 ↓
Logging
 ↓
Display Output
 ↓
Update Dashboard
```

---

# 👨‍💻 Author

**Sparsh Chauhan**

- B.Tech Computer Science Engineering
- Operating Systems Lab Project
- Full Stack Web Developer

---

# 📜 License

This project is intended for educational and academic purposes only.

---

# ⭐ Support

If you found this project useful, please consider giving it a **Star ⭐** on GitHub.

---

## Made with ❤️ using C, Windows Threads, and Operating System Concepts
