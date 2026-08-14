# IPC Debugger — Win32 Concurrency & IPC Diagnostic Utility

A multi-threaded Windows C console application for simulating, synchronizing, and inspecting Inter-Process Communication (IPC) mechanisms, thread lifecycle states, and concurrency deadlocks.

---

## GitHub Repository

- **GitHub Repository:** [https://github.com/Sparsh88/IPC-Debugger](https://github.com/Sparsh88/IPC-Debugger)

---

## Overview

IPC Debugger is a simulation and diagnostic console utility written in C for the Windows operating system. Developed to explore core operating system principles, it models primary inter-process communication mechanisms—**Pipes**, **Message Queues**, and **Shared Memory**—using native Win32 multi-threading and synchronization primitives.

The project provides interactive tools to observe how concurrent threads exchange data, transition through execution states, handle critical section mutex locks, and recover or stall during resource contention. It features a built-in thread registry, an in-memory event buffer, an aggregate operation metrics dashboard, and thread-safe persistent file logging.

Designed as an educational and technical portfolio demonstration for systems programming and operating systems concepts, it illustrates low-level Win32 thread management, critical sections, event signaling, and deadlock reproduction.

---

## Problem Statement

- **Theoretical Concurrency Concepts:** IPC mechanisms, mutex locks, and thread synchronization are often studied theoretically without hands-on runtime visibility into data handoffs.
- **Race Conditions & Lock Contention:** Unsynchronized concurrent memory access leads to undefined behavior and race conditions that require strict synchronization boundaries.
- **Lack of State Transparency:** Standard terminal utilities often hide thread lifecycle transitions (`CREATED`, `RUNNING`, `WAITING`, `TERMINATED`) during execution.
- **Deadlock Demonstration:** Understanding circular wait conditions requires a controlled environment that safely illustrates how inverted lock acquisition causes permanent execution stalls.

---

## Key Features

- **Pipe IPC Simulation:** Spawns sender and receiver threads (`Pipe-Sender`, `Pipe-Receiver`) to perform synchronized message passing through a dedicated pipe buffer using Win32 event notifications.
- **Message Queue Simulation:** Simulates asynchronous FIFO message delivery where producer threads enqueue messages and trigger receiver threads via signaled events.
- **Shared Memory Simulation:** Models shared memory read/write operations across threads protected by mutual exclusion critical sections.
- **Win32 Critical Section Synchronization:** Implements 7 distinct `CRITICAL_SECTION` objects ensuring thread safety across shared buffers, registries, logs, and simulated resources.
- **Event-Driven Coordination:** Utilizes auto-reset Win32 event handles (`CreateEvent`, `SetEvent`, `WaitForSingleObject`) to coordinate producer-consumer handoffs without CPU-intensive busy waiting.
- **Thread Lifecycle Registry:** Maintains an in-memory registry table tracking thread IDs, descriptive names, and active states (`THREAD_CREATED`, `THREAD_RUNNING`, `THREAD_WAITING`, `THREAD_TERMINATED`).
- **Deadlock Simulation:** Reproduces a classic two-thread circular wait condition demonstrating how inverted lock ordering causes mutual waiting and execution stalls.
- **Metrics Dashboard & Thread-Safe Logging:** Real-time summary of messages sent/received, bytes transferred, and lock waits alongside `CRITICAL_SECTION`-guarded persistent file logging.

---

## Tech Stack

| Category | Technology | Purpose |
|---|---|---|
| Language | C (C99 / C11 Standard) | Low-level systems programming and memory management |
| Operating System API | Windows Win32 API (`windows.h`) | Thread creation, critical sections, event handles, and console I/O |
| Threading & Concurrency | `CreateThread`, `WaitForSingleObject`, `CRITICAL_SECTION` | Multi-threaded execution, mutual exclusion, and thread lifecycle |
| Synchronization Handles | `CreateEvent`, `SetEvent`, `ResetEvent` | Producer-consumer signaling and thread coordination |
| Development Tooling | GCC (MinGW-w64) / Clang / MSVC | C compiler toolchains for Windows |

---

## Architecture

```text
Windows Console User Interface
             │
             ├──> Interactive Command Menu & Options
             │
             ▼
Core IPC Simulation Engine (Win32 Multi-Threading)
  ├── 1. Pipe IPC (Pipe-Sender ──[Event Signal + CS]──> Pipe-Receiver)
  ├── 2. Message Queue (MQ-Producer ──[FIFO Queue + Event]──> MQ-Consumer)
  ├── 3. Shared Memory (SHM-Writer ──[Critical Section]──> SHM-Reader)
  └── 4. Deadlock Module (Deadlock-1 vs Deadlock-2 Circular Wait)
             │
             ├──> Thread Registry (State Tracker: CREATED | RUNNING | WAITING | TERMINATED)
             ├──> Aggregate Metrics Dashboard (Throughput & Lock Wait Telemetry)
             └──> Thread-Safe File Logger (log_cs Protected File Appends)
```

---

## Application Flow

1. **Initialization:** The application starts, initializes all 7 Win32 critical sections, creates event handles, and resets thread tables.
2. **Menu Selection:** The user selects an operation from the console menu (Pipe, Message Queue, Shared Memory, Deadlock, Registry, Metrics, or Clear Logs).
3. **Thread Dispatch:** The engine spawns worker threads via `CreateThread`, assigns descriptive labels, and registers them in `thread_table`.
4. **Synchronized Execution:** Worker threads acquire critical sections, exchange data payloads, signal completion events, and update operational metrics.
5. **State Logging:** State transitions and IPC data packets are logged to the console and appended to `ipc_debug.log` under mutex protection.
6. **Thread Cleanup:** Parent thread joins worker threads via `WaitForSingleObject` / `WaitForMultipleObjects` and closes thread handles.

---

## Project Structure

```text
IPC-Debugger/
├── src/
│   └── main.c                 # Complete C source code (threads, IPC, mutexes, menu)
├── docs/                      # Technical documentation and architecture diagrams
├── build.bat                  # Windows batch compilation script
├── Makefile                   # Makefile for MinGW/Make builds
└── README.md
```

---

## Getting Started

### Prerequisites

- **Windows Operating System** (Windows 10/11)
- **C Compiler**: MinGW-w64 (GCC), Clang, or Microsoft Visual C++ (MSVC)

### Compilation

Using **GCC (MinGW)**:

```bash
gcc -std=c99 -Wall -O2 src/main.c -o ipc_debugger.exe
```

Using **MSVC (Developer Command Prompt)**:

```cmd
cl.exe /W3 /O2 src/main.c /Fe:ipc_debugger.exe
```

### Running the Application

```cmd
ipc_debugger.exe
```

---

## Author

**Sparsh Chauhan**  
*Computer Science & Engineering Student | Systems & Full Stack Developer*

- **Portfolio:** [portfolio-flame-rho-29.vercel.app](https://portfolio-flame-rho-29.vercel.app/)
- **GitHub:** [@Sparsh88](https://github.com/Sparsh88)
- **LinkedIn:** [linkedin.com/in/sparshchauhan08](https://linkedin.com/in/sparshchauhan08)
- **Email:** [sparshchauhan050@gmail.com](mailto:sparshchauhan050@gmail.com)
