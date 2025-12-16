🧠 IPC Debugger (Simulation-Based)

📌 Overview

The IPC Debugger is a simulation-based Inter-Process Communication (IPC) debugging and monitoring tool developed in C for Windows systems.
The project demonstrates and analyzes the working of major IPC mechanisms—Pipes, Message Queues, and Shared Memory—using multithreading, synchronization, and logging techniques.

Since native Windows systems do not support POSIX IPC system calls, this project implements a conceptual simulation that preserves the theoretical behavior of IPC mechanisms while remaining executable on Windows.

🎯 Objectives

To demonstrate Inter-Process Communication (IPC) concepts

To simulate Pipe, Message Queue, and Shared Memory IPC

To monitor IPC operations using thread state tracking

To log IPC events for debugging and analysis

To build an OS-lab-ready IPC Debugger project

⚙️ Features

🔹 Pipe IPC simulation

🔹 Message Queue IPC simulation

🔹 Shared Memory IPC simulation

🔹 Multithreaded execution using Windows threads

🔹 Thread lifecycle and state monitoring

🔹 Centralized logging (ipc_log.txt)

🔹 IPC operation counters and statistics

🔹 Event history tracking

🔹 Deadlock simulation module

🔹 Menu-driven debugger interface

🔹 Graceful termination (no forced Ctrl+C)

🏗️ Project Architecture
+---------------------+
|     User Menu       |
+---------------------+
          |
          v
+---------------------+
| IPC Simulation Layer|
|  - Pipe             |
|  - Message Queue    |
|  - Shared Memory    |
+---------------------+
          |
          v
+---------------------+
| Synchronization     |
| (Critical Sections)|
+---------------------+
          |
          v
+---------------------+
| Debugging & Logging |
+---------------------+

🧪 Technologies Used

Programming Language: C

Operating System: Windows

Compiler: MinGW (GCC)

IDE: Visual Studio Code

Threading Model: Windows Threads

Synchronization: Critical Sections

📂 Repository Structure
IPC-Debugger/
│
├── ipc_debugger.c     # Complete 500+ line source code
├── ipc_log.txt        # Generated log file after execution
└── README.md          # Project documentation

▶️ How to Compile and Run
1️⃣ Prerequisites

MinGW installed and added to system PATH

VS Code or any C-compatible editor

2️⃣ Compile the Code
gcc ipc_debugger.c -o ipc_debugger

3️⃣ Run the Program
.\ipc_debugger

4️⃣ Exit the Program

Use menu option:

0. Exit

📊 Sample Output
===== ADVANCED IPC DEBUGGER =====
1. Pipe IPC
2. Message Queue IPC
3. Shared Memory IPC
4. Show Dashboard

[PIPE] Sender wrote data
[PIPE] Receiver read: Hello via Pipe IPC

🗂️ Logging and Debugging

All IPC activities are logged with timestamps in:

ipc_log.txt

Sample Log Output
[14:42:10] [PIPE] Pipe sender wrote data
[14:42:11] [MESSAGE_QUEUE] Message enqueued
[14:42:12] [SHARED_MEMORY] Shared memory written


This log file helps in tracing IPC events and debugging communication behavior.

⚠️ Limitations

This project simulates IPC behavior and does not use kernel-level IPC system calls

POSIX IPC APIs are not used due to Windows OS limitations

Intended primarily for academic and educational purposes

🎓 Academic Relevance

This project is suitable for:

Operating Systems Laboratory

IPC and Process Synchronization topics

OS practical examinations

Mini-project or lab project submissions

🎤 Viva Explanation (One-Line)

“This project is a simulation-based IPC debugger that demonstrates pipe, message queue, and shared memory communication using multithreading and synchronization in C.”

🚀 Future Enhancements

Linux version using real POSIX IPC

Graphical dashboard for IPC monitoring

Performance benchmarking and analysis

IPC fault injection and recovery simulation

👨‍💻 Author

Name: Sparsh Chauhan
Subject: Operating Systems
Project Type: Academic / Lab Project

📜 License

This project is intended for educational use only.

✅ Final Note

This repository demonstrates a strong conceptual understanding of Inter-Process Communication, process synchronization, and operating system principles, making it suitable for academic evaluation and viva examinations.
