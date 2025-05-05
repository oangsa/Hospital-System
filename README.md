# Hospital Management System

A comprehensive hospital management system implemented in C++ as part of the CPE112 Data Structures course. This system allows for efficient management of patient records, doctor schedules, appointments, and emergency cases using various data structures.

## Features

- User authentication and role-based access (Admin, Doctor, Nurse, Patient)
- Patient registration and management (OPD and IPD)
- Medical history tracking and retrieval
- Queue management for patients with priority-based emergency handling
- Undo functionality for administrative actions

## Data Structures Used

- **Map**: For user data storage and retrieval
- **Queue**: For standard patient waiting lists
- **Priority Queue**: For emergency patient handling based on ESI levels
- **Stack**: For undo functionality
- **Binary Search Trees**: For patient history storage
- **Linked Lists**: For underlying implementation of various structures
- **Set**: For tracking unique patient IDs

## Requirements

- GCC version 13 or higher
- Make utility
- Operating System:
  - Windows 11 (primary support)
  - Linux (supported)
  - macOS (future support planned)

## How to Run

1. Clone the repository

```bash
git clone https://github.com/oangsa/Hospital-System.git
```

2. Navigate to the project directory

```bash
cd Hospital-System
```

3. Compile and run:

- Compile Using Golang

```bash
go run compile/build.go

./main.exe #On Windows
./main #On Linux
```
- Compile Using C++

```bash
g++ compile/build.cpp -o build -std=c++17
```

- If you use Windows

```bash
./build.exe

# Run the program
./main.exe
```

- If you use Linux or MacOS

```bash
./build

# Run the program
./main
```

### Project Structure

- **Features**: Functionality modules
- **Libs**: Data structure modules
- **Database**: Storage

### Members
1. Mr. Suthang Sukrueangkun 67070503477
2. Mr. Sorawit Chaithong 67070503442
3. Mr. Wathit Tritsananawakit 67070503495
