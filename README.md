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

- If you use Windows

```bash
# Build from Makefile
make

# Run the program
./main.exe
```

- If you use Linux or macos

- Note: Make sure you have golang installed

```bash
# Give permission to .sh file
chmod +x build.sh

# Run the file
./build.sh

# Run the program
./main
```

### Project Structure

- **Features**: Functionality modules
- **Libs**: Data structure modules
- **Database**: Storage

### Members
