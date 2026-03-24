# MiniGit - A Simplified Version Control System

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Status](https://img.shields.io/badge/Status-Active-success.svg)

A lightweight, Git-inspired version control system built from scratch in C++ as part of the Data Structures & Algorithms course at ITU. MiniGit demonstrates core VCS concepts including file tracking, commit history, delta storage, and change detection using fundamental data structures.

## Table of Contents
- [Features](#features)
- [How It Works](#how-it-works)
- [Installation](#installation)
- [Usage](#usage)
- [Technical Details](#technical-details)
- [Project Structure](#project-structure)
- [Demo](#demo)
- [Author](#author)
- [Acknowledgments](#acknowledgments)

## Features

- **Repository Initialization** - Initialize version control in any directory
- **File & Directory Tracking** - Automatically detects new, modified, and deleted files/folders
- **Hash-Based Change Detection** - Uses SHA hashing to efficiently identify file modifications
- **Delta Storage** - Stores only changes (deltas) instead of complete files for optimal space efficiency
- **Commit History** - Maintains a complete linked-list-based commit tree with timestamps
- **Recursive Directory Scanning** - Tracks nested folder structures
- **Interactive Terminal Interface** - User-friendly command-line interface
- **Real-time Status Updates** - See what's changed before committing

## How It Works

### Core Concepts

#### 1. **Hashing Mechanism**
```
File Content → Hash Function → Unique Number
"Hello World" → 12345678
"Hello Earth" → 87654321
```
Each file's content is hashed. If the hash changes, the file has been modified.

#### 2. **Delta Storage**
```
Commit 1: Stores complete file (100 lines)
Commit 2: Stores only changes (2 lines modified)
         ↳ Saves storage space!
```
First commit stores full content. Subsequent commits only store the differences (deltas).

#### 3. **Linked List Commit Structure**
```
[HEAD] → [Commit 3] → [Commit 2] → [Commit 1] → NULL
           Latest      Parent       Parent
```
Each commit points to its parent, forming a traversable history.

### Data Structures Used

- **Map (Red-Black Tree)** - Fast O(log n) file lookup by name
- **Vector (Dynamic Array)** - Storing lists of files, deltas, and changes
- **Linked List** - Commit history with parent pointers
- **Hash Table** - Content-based change detection
- **Struct** - Organizing file metadata and commit information

## Installation

### Prerequisites
- C++ Compiler with C++17 support (GCC 7+ or Clang 5+)
- Linux, macOS, or Windows with WSL

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/BSSE24062/minigit.git
cd minigit

# Compile
g++ -std=c++17 main.cpp functions.cpp -o minigit

# Run
./minigit
```

### Using Makefile (Optional)
```bash
make build    # Compile the project
make run      # Run MiniGit
make clean    # Clean build files
```

## Usage

### Basic Commands

#### Initialize Repository
```bash
mini-git> init ./my_project
```
Creates a new repository in the specified directory.

#### Check Status
```bash
mini-git> status
```
Shows new, modified, and deleted files/directories.

#### Commit Changes
```bash
mini-git> commit "Your commit message"
```
Saves the current state with a descriptive message.

#### View History
```bash
mini-git> log
```
Displays complete commit history with timestamps.

#### Get Help
```bash
mini-git> help
```
Shows all available commands.

#### Exit
```bash
mini-git> exit
```
Closes the program.

### Example Workflow

```bash
# Start MiniGit
./minigit

# Initialize repository
mini-git> init ./docs

# In another terminal, create files
cd docs
echo "Hello World" > file.txt
mkdir folder
echo "Nested content" > folder/nested.txt

# Back to MiniGit
mini-git> status
# Shows: New directories and files

mini-git> commit "Initial commit"
# Stores full file content

# Modify a file
echo "Hello World Updated" > file.txt

mini-git> status
# Shows: Modified file (detected via hash)

mini-git> commit "Updated greeting"
# Stores only the delta (changes)

mini-git> log
# Shows complete commit history

mini-git> exit
```

## Technical Details

### File: `functions.h`
Header file containing:
- `FileInfo` struct - File metadata and content
- `FileDelta` struct - Line-by-line changes
- `CommitNode` struct - Commit data with parent pointer
- `MiniGit` class - Main version control logic

### File: `functions.cpp`
Implementation including:
- `hashString()` - Content hashing
- `scanRepository()` - Recursive directory traversal
- `computeDelta()` - Line-by-line diff algorithm
- `init()` - Repository initialization
- `status()` - Change detection
- `commit()` - State persistence
- `log()` - History traversal

### File: `main.cpp`
Interactive terminal interface with command parsing.

### Algorithms Implemented

1. **Hash-Based Change Detection**
   - Time Complexity: O(n) where n is file size
   - Space Complexity: O(1) for hash storage

2. **Delta Computation**
   - Time Complexity: O(n) for line-by-line comparison
   - Space Complexity: O(k) where k is number of changes

3. **Commit History Traversal**
   - Time Complexity: O(m) where m is number of commits
   - Space Complexity: O(m) for commit chain

4. **Recursive Directory Scanning**
   - Time Complexity: O(n) where n is total files/folders
   - Space Complexity: O(d) where d is max depth

## Project Structure

```
minigit/
├── functions.h          # Header declarations
├── functions.cpp        # Implementation
├── main.cpp            # Entry point & CLI
├── README.md           # Documentation
├── Makefile            # Build automation (optional)
└── test_repo/          # Example repository
    └── .minigit/       # Version control data
```

## Demo

### Terminal Output Example

```
=================================
    Mini-Git Version Control     
=================================
Commands: init, status, commit, log, help, exit

mini-git> init ./test_repo
Initialized mini-git repository in ./test_repo

mini-git> status

=== Repository Status ===

New files:
  + file.txt
  + README.md

New directories:
  + src/

mini-git> commit "Initial project setup"
Creating initial commit...
  Added: file.txt (hash: 1234567890)
  Added: README.md (hash: 9876543210)
  Added: src/ (directory)

[Commit 1] Initial project setup
Timestamp: 2024-12-29 15:45:30

mini-git> log

=== Commit History ===

Commit ID: 1
Message: Initial project setup
Timestamp: 2024-12-29 15:45:30
---
```

##  Author

**Zakki Ullah Baig**
- Institution: Information Technology University (ITU)
- Course: SE200T - Data Structures & Algorithms
- Semester: Fall 2025

## Acknowledgments

- **Course Instructor**: Usama Bin Shakeel
- **Teaching Assistants**: Afaq Khawar, Ryan Naveed, Ahmad Mustafa
- **Lab Engineer**: Raza Dastagir
- **Department**: Computer and Software Engineering, ITU

This project was developed as Lab 13 (Open-Ended Lab) to demonstrate practical application of data structures including linked lists, hash tables, maps, and trees in building a real-world version control system.

## License

This project is developed for educational purposes as part of the DSA course curriculum.

## Links

- GitHub Repository: (https://github.com/BSSE24062/minigit)
- LinkedIn: (https://www.linkedin.com/in/zakki-ullah-baig-7455a0319/)
- Portfolio: (https://z4kki.netlify.app)

---

 If you found this project interesting, please give it a star!

**Keywords**: #VersionControl #DataStructures #Algorithms #CPP #Git #SystemProgramming #SoftwareEngineering #ITU #DSA