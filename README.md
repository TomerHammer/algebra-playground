# 🧮 Algebra Playground

A **C++17 command-line application** for performing and exploring **matrix algebra operations**.  
This project was developed to practice **object-oriented programming**, **modular design**, and **numerical problem solving** in C++.  
It reflects a strong motivation to grow as a developer and continuously learn from new challenges and experiences.
---

## 🚀 Features

- Create and manage matrices interactively  
- Perform matrix operations: addition, subtraction, multiplication, transpose  
- Compute determinant, rank, and inverse  
- Solve linear systems of equations (Ax = b)  
- Save and load entire workspaces  
- Includes automated unit and integration tests  

---

## 🧠 Example Session

```
Algebraic Matrix CLI v1.0
Available commands:
  - create
  - load
  - help
  - exit
> create A 3 3
Matrix 'A' created:
  Dimensions: 3 x 3
Available commands:
  - create
  - delete
  - assign
  - scalar_multiply
  - transpose
  - rank
  - det
  - inverse
  - list
  - show
  - save
  - load
  - help
  - exit
> show A
Matrix 'A':
|  0.000|  0.000|  0.000|
|  0.000|  0.000|  0.000|
|  0.000|  0.000|  0.000|

> assign A  
Assign value for element in (0, 0)
> 1
Assign value for element in (0, 1)
> 2
Assign value for element in (0, 2)
> 3
Assign value for element in (1, 0)
> 4
Assign value for element in (1, 1)
> 5
Assign value for element in (1, 2)
> 6
Assign value for element in (2, 0)
> 7
Assign value for element in (2, 1)
> 8
Assign value for element in (2, 2)
> 9

> det A
Determinant of matrix 'A' is: 0.000

> create B 3 3 2
Matrix 'B' created:
  Dimensions: 3 x 3

> add C A B
> show C
Matrix 'C':
|  3.000|  4.000|  5.000|
|  6.000|  7.000|  8.000|
|  9.000| 10.000| 11.000|

> exit
Exiting CLI.
```

---

### 🧭 Command Formatting

For detailed command usage and syntax, use the built-in `help` command in the CLI.

---

## 🧩 Project Structure

```
ALGEBRA-PLAYGROUND/
├── cli/
│   ├── CLI.cpp
│   └── CLI.h
├── include/
│   ├── Matrix.h
│   └── Workspace.h
├── src/
│   ├── Matrix.cpp
│   └── Workspace.cpp
├── tests/
│   ├── run_tests.sh
│   ├── matrixTests.cpp
│   └── cliTests/
├── workspaces/
├── CMakeLists.txt
├── README.md
└── .gitignore
└── main.cpp
```

---

## ⚙️ Build & Run

### Build
```bash
cmake -S . -B build
cmake --build build
```

### Run CLI
```bash
./build/bin/prog
```

### Run Tests
```bash
chmod +x tests/run_tests.sh
./tests/run_tests.sh
```

### 🪟 Running on Windows

1. **Install:**
   - [CMake](https://cmake.org/download/)
   - Either **MinGW (g++)** or **Visual Studio Build Tools**

2. **Open PowerShell or CMD**, navigate to your project:
   ```bash
   cd path\to\ALGEBRA-PLAYGROUND
   cmake -S . -B build
   cmake --build build
   ```

3. The executables will be located in:
   ```
   build\bin\prog.exe
   build\bin\matrixTests.exe
   ```

4. **Run the CLI:**
   ```bash
   .\build\bin\prog.exe
   ```

5. **Run the tests:**
   - If you have Git Bash installed:
     ```bash
     bash tests/run_tests.sh
     ```
   - Or run manually in CMD:
     ```bash
     build\bin\matrixTests.exe
     ```

---

## 🧪 Testing

The project includes:
- **Unit tests** for matrix operations  
- **Integration tests** for CLI functionality  
- Test automation script: `run_tests.sh`

---

## 🧭 Design Overview

- **Object-oriented structure** with clear separation of concerns (`Matrix`, `Workspace`, `CLI`)  
- **Error-safe implementation** using custom exception handling  
- **Consistent interface** for matrix operations and user commands  
- **Cross-platform support** via CMake build system  

---

## 💡 Future Work

- Add LU and QR decompositions  
- Add GUI visualization layer  
- Add command history support (e.g., recalling previous commands like in terminals)
- Add undo/redo functionality

---

## 👨‍💻 Author

**Tomer Hammer**  
Computer Science student at the Technion.  
Highly motivated to keep improving, learn from experienced developers, and take part in meaningful software projects.  
Interested in backend development and system design.

- [LinkedIn](https://www.linkedin.com/in/tomer-hammer/)  
- [GitHub](https://github.com/TomerHammer)

---

## 🏁 Summary

This project demonstrates the implementation of **linear algebra operations**  
through a clean, testable, and extensible C++ codebase.
