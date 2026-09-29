# Smart Task Manager using DSA Concepts 

![GitHub Stars](https://img.shields.io/github/stars/Deadeepya574/Smart-Task-Manager-using-DSA-concept?style=social)
![GitHub Forks](https://img.shields.io/github/forks/Deadeepya574/Smart-Task-Manager-using-DSA-concept?style=social)
![GitHub Language](https://img.shields.io/github/languages/top/Deadeepya574/Smart-Task-Manager-using-DSA-concept)

##  Description

This project implements a **Smart Task Manager** that leverages various Data Structures and Algorithms (DSA) for efficient task management. It features a modern web-based user interface (frontend) built with HTML, CSS, and JavaScript, powered by a Python Flask backend. The core task logic, including advanced operations like undo/redo and priority management, is handled by a high-performance C++ engine that communicates with the Flask application via standard input/output streams.

This architecture demonstrates a robust inter-process communication pattern, combining the strengths of different programming languages to deliver a responsive and feature-rich task management experience. Users can add, update, delete, complete tasks, and organize them with priorities, deadlines, and categories, all while benefiting from the underlying DSA optimizations.

##  Table of Contents

*   [ Description](#-description)
*   [ Features](#-features)
*   [ Tech Stack](#%EF%B8%8F-tech-stack)
*   [ Project Structure](#-project-structure)
*   [ Installation](#%EF%B8%8F-installation)
    *   [Prerequisites](#prerequisites)
    *   [Clone the Repository](#clone-the-repository)
    *   [Build the C++ Engine](#build-the-c-engine)
    *   [Set up the Python Backend](#set-up-the-python-backend)
*   [ Usage](#%E2%96%B6%EF%B8%8F-usage)
*   [ API Reference](#-api-reference)
*   [ Contributing](#-contributing)
*   ⚖️ License](#%E2%99%94%EF%B8%8F-license)
*   [ Important Links](#-important-links)
*   [udos Footer](#-footer)

##  Features

This Smart Task Manager offers a comprehensive set of functionalities to help you stay organized:

*    **Task CRUD Operations**: Easily Add, View, Update, and Delete tasks.
*    **Priority Management**: Assign priorities (High, Medium, Low) to tasks.
*    **Deadline Tracking**: Set and manage deadlines for your tasks.
*    **Categorization**: Organize tasks into custom categories.
*    **Undo/Redo Functionality**: Revert or reapply previous actions, powered by `std::stack` in the C++ engine.
*    **Highest Priority Task View**: Quickly identify your most critical pending task using a `std::priority_queue`.
*    **Task Search**: Find tasks by ID or Title.
*    **Dashboard Statistics**: Get an overview of total, pending, completed, and high-priority tasks.
*    **Task Filtering**: Filter tasks by status (Pending/Completed) or priority level directly from the UI.
*    **Task Sorting**: Sort tasks by priority or deadline (console-only feature in the C++ component, though the web UI offers filtering).
*    **Hybrid Architecture**: Python Flask backend orchestrating a high-performance C++ core engine for task logic.

##  Tech Stack

The project is built using a combination of powerful technologies:

*   **Frontend**: 
    *   HTML5
    *   CSS3
    *   JavaScript
*   **Backend (API Layer)**: 
    *   Python 3.x
    *   Flask
    *   Flask-CORS
*   **Backend (Core Logic)**: 
    *   C++17 (or newer)
    *   Standard Template Library (STL) - `vector`, `stack`, `priority_queue`

##  Project Structure

The repository is organized into two main components: `backend` and `frontend`.

```
Smart-Task-Manager-using-DSA-concept/
├── backend/
│   ├── app.py              # Flask API server, communicates with C++ engine
│   ├── Task-Manager.cpp    # C++ TaskManager class implementation (console version)
│   ├── main.cpp            # Duplicate of Task-Manager.cpp (console version)
│   ├── TaskManager.h       # C++ TaskManager class declarations (empty in analysis, inferred)
│   ├── engine.cpp          # C++ executable source for inter-process communication
│   ├── engine.exe          # Compiled C++ engine (required by Flask backend)
│   └── tasks.json          # (Empty, likely for future persistence or testing)
├── frontend/
│   ├── index.html          # Main HTML for the web UI
│   ├── script.js           # Frontend JavaScript logic and API calls
│   └── style.css           # Frontend CSS styling
├── index.html              # Redirects to frontend/index.html
└── README.md               # Project documentation
```

##  Installation

To get the Smart Task Manager up and running on your local machine, follow these steps:

### Prerequisites

Ensure you have the following installed:

*   **Python 3.x**: Download from [python.org](https://www.python.org/downloads/).
*   **C++ Compiler**: A modern C++ compiler (e.g., g++ for Linux/macOS/WSL, MSVC for Windows) is required to build the C++ engine.

### Clone the Repository

First, clone the project repository to your local machine:

```bash
git clone https://github.com/Deadeepya574/Smart-Task-Manager-using-DSA-concept.git
cd Smart-Task-Manager-using-DSA-concept
```

### Build the C++ Engine

Navigate to the `backend` directory and compile the C++ engine. The `engine.cpp` file uses a `TaskManager` class, whose full definition is expected from `TaskManager.h` and its implementation is similar to `Task-Manager.cpp` or `main.cpp`.

```bash
cd backend
# For Linux/macOS/WSL (using g++):
g++ engine.cpp -o engine.exe -std=c++17 -I. -Wall -Wextra # Assuming TaskManager.h is in current directory and TaskManager class is self-contained or linked.
# If Task-Manager.cpp contains the TaskManager class implementation, you might need:
g++ engine.cpp Task-Manager.cpp -o engine.exe -std=c++17 -I. -Wall -Wextra

# For Windows (using MinGW g++):
g++ engine.cpp -o engine.exe -std=c++17 -I. -Wall -Wextra
# Or with Task-Manager.cpp:
g++ engine.cpp Task-Manager.cpp -o engine.exe -std=c++17 -I. -Wall -Wextra

# Ensure engine.exe is created in the 'backend' directory.
```

**Note**: The provided `TaskManager.h` is empty in the analysis. For successful compilation, `engine.cpp` implicitly relies on the `Task` struct and `TaskManager` class definition. It's assumed that the functionality from `Task-Manager.cpp` (or `main.cpp`) is either part of `TaskManager.h` or linked during compilation.

### Set up the Python Backend

Still in the `backend` directory, install the required Python dependencies:

```bash
pip install Flask Flask-Cors
cd .. # Go back to the root directory if you want to run from there, or stay in backend for direct execution
```

##  Usage

Once both the C++ engine and Python backend are set up, you can start the application:

1.  **Run the Flask Backend**: From the project's root directory or `backend` directory:
    ```bash
    cd backend
    python app.py
    ```
    The Flask server will start, typically on `http://127.0.0.1:5000`.

2.  **Access the Web UI**: Open your web browser and navigate to `http://127.0.0.1:5000`.
    *   The `index.html` at the project root will automatically redirect you to `frontend/index.html`.

### How to use the application:

*   **Add Tasks**: Use the 

---
**<p align="center">Generated by [ReadmeCodeGen](https://www.readmecodegen.com/)</p>**
