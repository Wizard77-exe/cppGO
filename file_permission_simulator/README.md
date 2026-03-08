# FILE PERMISSION SIMULATION
---
This file is a simple simulator to show file permission management using bitwise operations. My main goal on creating it, is to learn bitwise operations.
---
## Modules
---
1. <iostream> - standard input and output stream
2. <vector> - STL container. Basically a dynamic array.
---
## Features
---
- Creates Admin or Normal User
- (admin) - set other users' permissions
- (admin) - provoke other users' permissions
- (admin) - toggle other users' permissions
- (admin) - create new users
- (admin) - list all users and their ids and permissions
- (normal user) - Read a file
- (normal user) - Write to a file
- (normal user) - Delete file
- (normal user) - Execute file
- (normal user) - See its own permissions
---
## Future Improvement
---
1. Add real file management using <fstream> (file handline)
2. Add password hashing
3. TUI - Terminal User Interface using <ncurses>
4. others.
---
## Cloning
1. Clone the Repository
```bash
git clone https://github.com/Wizard77-exe/cppGO
```
2. Head to the folder file
```bash
cd file_permission_simulator
```
3. Compile .cpp file
```bash
g++ file-termission-simulator.cpp -o permission-simulator.exe
```
4. Run the .exe file
```
./permission-simulator.exe
```