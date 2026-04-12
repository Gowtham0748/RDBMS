# 🗄️ NITCBase — Mini Relational Database Management System

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge&logo=cplusplus"/>
  <img src="https://img.shields.io/badge/Platform-Linux-orange?style=for-the-badge&logo=linux"/>
  <img src="https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/Stages-12%2F12-blueviolet?style=for-the-badge"/>
</p>

<p align="center">
  A fully implemented mini RDBMS built from scratch in C++ — disk, buffer, cache, indexing, and an interactive command interface included.
</p>

---

## 📌 Overview

NITCBase is a fully implemented mini Relational Database Management System (RDBMS) built from scratch in C++, developed as part of a laboratory course at **NIT Calicut**. It covers every internal layer of a real database engine — from disk simulation and buffer management to B+ Tree indexing and an interactive command-line interface.

This project is designed for educational purposes, providing hands-on experience with how a database works under the hood.

### ✨ Highlights

- 🖴 &nbsp;**Simulated 16MB disk** with block-level I/O
- ⚡ &nbsp;**32-block buffer pool** for fast in-memory access
- 🗂️ &nbsp;**Relation & attribute cache** for up to 12 open relations
- 🌲 &nbsp;**B+ Tree indexing** for efficient attribute-based search
- 🔍 &nbsp;**Relational algebra** — select, project, insert, and join
- 💬 &nbsp;**Custom command-line interface** with SQL-like syntax and batch script support
- 📦 &nbsp;**12 stages** completed end-to-end

---

## 🔄 How It Works

```
User Command → Frontend Interface → Schema / Algebra → Block Access → B+ Tree / Cache → Buffer → Disk
```

Each layer only talks to the one below it — a clean, real-world DBMS design.

---

## 📋 Table of Contents

- [Prerequisites](#-prerequisites)
- [Getting Started](#-getting-started)
- [Usage](#-usage)
- [NITCBase Documentation](#-nitcbase-documentation)
- [Contributing](#-contributing)
- [License](#-license)

---

## 🛠️ Prerequisites

Before running NITCBase, ensure you have the following installed:

- Linux-based OS (tested on Ubuntu 20.04)
- C/C++ compiler (`gcc` / `g++`)
- `make`
- `libreadline-dev`
- `git`

Install all required packages with:

```bash
sudo apt-get install build-essential libreadline-dev git
```

---

## 🚀 Getting Started

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/hatim-s/DataBase-Management-System
   ```

2. Navigate to the project directory:
   ```bash
   cd NITCBase/mynitcbase
   ```

3. Build the application:
   ```bash
   make
   ```

   > If you run into build issues, make sure all prerequisites are properly installed.

### Running NITCBase

```bash
./nitcbase
```

This starts the interactive command-line interface where you can run NITCBase commands directly.

---

## 💻 Usage

NITCBase provides a custom command-line interface with SQL-like syntax for performing database operations:

```
-- Create and manage tables
CREATE TABLE Students (RollNo NUMBER, Name STRING, CGPA NUMBER);
OPEN TABLE Students;

-- Insert and query data
INSERT INTO Students VALUES (1, Alice, 9.5);
SELECT * FROM Students WHERE CGPA >= 9.0 INTO TopStudents;

-- Indexing
CREATE INDEX ON Students.RollNo;

-- Join two relations
SELECT * FROM Students JOIN Courses ON Students.RollNo = Courses.RollNo INTO Output;

-- Run a batch script
RUN myscript.run;
```

> Note: NITCBase uses its own command syntax. Commands may look similar to SQL but are not standard SQL — for example, `SELECT` requires an `INTO` target relation.

For the complete list of supported commands, refer to the [NITCBase User Interface Documentation](https://nitcbase.github.io/docs/User%20Interface%20Commands/).

---

## 📖 NITCBase Documentation

For detailed information on the architecture, design, and all supported features:

📚 [NITCBase Official Documentation](https://nitcbase.github.io/)

---

## 🤝 Contributing

Contributions are welcome! If you'd like to improve NITCBase:

1. Fork the repository
2. Create a new branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -m 'Add some feature'`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a Pull Request

Please follow clear naming conventions and include a short description of your changes.

---

<p align="center">
  Thank you for checking out NITCBase! If you run into any issues or have questions, feel free to <a href="https://github.com/Gowtham0748/RDBMS/issues">open an issue</a>.
</p>
