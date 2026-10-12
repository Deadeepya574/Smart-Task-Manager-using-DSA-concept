# Smart Task Manager using DSA Concepts 
 
## Description

A productivity dashboard built with Streamlit and C++, designed to demonstrate how Data Structures and Algorithms (DSA) can be used to build a practical task management application.

The Streamlit interface communicates directly with a C++ engine through standard input/output. The C++ engine handles task operations and demonstrates data structures such as vectors, stacks, and priority queues.

## Table of Contents

*   [Description](#description)
*   [Features](#features)
*   [Tech Stack](#tech-stack)
*   [Project Structure](#project-structure)
*   [Installation](#installation)
    *   [Prerequisites](#prerequisites)
    *   [Clone the Repository](#clone-the-repository)
    *   [Build the C++ Engine](#build-the-c-engine)
    *   [Set up the Python Backend](#set-up-the-python-backend)
*   [Usage](#usage) 
*   [Important Links](#important-links) 

## Features

This Smart Task Manager offers a comprehensive set of functionalities to help you stay organized:

*   **Task CRUD Operations**: Easily Add, View, Update, and Delete tasks.
*   **Priority Management**: Assign priorities (High, Medium, Low) to tasks.
*   **Deadline Tracking**: Set and manage deadlines for your tasks.
*   **Categorization**: Organize tasks into custom categories.
*   **Undo/Redo Functionality**: Revert or reapply previous actions, powered by `std::stack` in the C++ engine.
*   **Highest Priority Task View**: Quickly identify your most critical pending task using a `std::priority_queue`.
*   **Task Search**: Find tasks by ID or Title.
*   **Dashboard Statistics**: Get an overview of total, pending, completed, and high-priority tasks.
*   **Task Filtering**: Filter tasks by status (Pending/Completed) or priority level directly from the UI.
*   **Task Sorting**: Sort tasks by priority or deadline (console-only feature in the C++ component, though the web UI offers filtering).
*   **Streamlit Dashboard**: Use a clean, interactive interface built with Python and Streamlit.

## Tech Stack

The project is built using a combination of powerful technologies:

*   **Frontend**:
    *   HTML5
    *   CSS3
    *   JavaScript
*   **Interface and DashBoard**:
    *   Streamlit
*   **Backend (Core Logic)**:
    *   C++17 (or newer)
    *   Standard Template Library (STL) - `vector`, `stack`, `priority_queue`

## Project Structure

The repository is organized into two main components: `backend` and `frontend`.

```
Smart-Task-Manager-using-DSA-concept/
├── backend/
│   ├── Task-Manager.cpp
│   ├── Task-Manager.exe
│   ├── TaskManager.h
│   ├── engine.cpp
│   ├── engine.exe
│   ├── main.cpp
│   ├── task_engine.exe
│   └── tasks.json
├── streamlit_app.py
├── requirements.txt
├── .gitignore
└── README.md
```
### Main Files

- `streamlit_app.py` — Implements the Streamlit dashboard and communicates with the C++ engine.
- `backend/engine.cpp` — Implements the command-based interface for communication with the application.
- `backend/TaskManager.h` — Defines the task structure and core task management operations.
- `backend/Task-Manager.cpp` — Contains the console-based task manager implementation.
- `backend/main.cpp` — Contains a console-oriented C++ program.
- `requirements.txt` — Lists the Python dependencies.
- `backend/tasks.json` — Present in the repository but not currently used by the C++ engine for persistent task storage.


## Installation

To get the Smart Task Manager up and running on your local machine, follow these steps:

### Prerequisites

Ensure you have the following installed:

*   **Python 3.x**: Download from [python.org](https://www.python.org/downloads/).
*   **C++ Compiler**: A modern C++ compiler (e.g., g++ for Linux/macOS/WSL, MSVC for Windows) is required to build the C++ engine.

### 1. Clone the Repository

```bash
git clone https://github.com/Deadeepya574/Smart-Task-Manager-using-DSA-concept.git
cd Smart-Task-Manager-using-DSA-concept
```

### 2. Create a Virtual Environment

For Windows PowerShell:

```powershell
py -m venv .venv
.venv\Scripts\Activate.ps1
```

### 3. Install Dependencies

```bash
python -m pip install --upgrade pip
pip install -r requirements.txt
```

Ensure that the `requirements.txt` file contains:

```text
streamlit
```

### 4. Verify the C++ Compiler

```bash
g++ --version
```

If the command is not recognized, install a compatible C++ compiler and configure its PATH.

### 5. Run the Application

From the project root directory, where `streamlit_app.py` is located, execute:

```bash
streamlit run streamlit_app.py
```

Streamlit will provide a local URL, usually:

```text
http://localhost:8501
```

Open this URL in your web browser to access the Smart Task Manager dashboard.

The application automatically attempts to compile and launch the C++ engine when required.

## Deployment on Streamlit Community Cloud

1. Push your project files to GitHub.
2. Visit [Streamlit Community Cloud](https://share.streamlit.io/).
3. Create a new application and select your GitHub repository.
4. Choose the `main` branch.
5. Set the main file path to `streamlit_app.py`.
6. Deploy the application.

If the cloud environment requires a C++ compiler, create a root-level `packages.txt` file containing:

```text
g++
```

Keep Python dependencies in `requirements.txt` and system packages in `packages.txt`.

Make sure `backend/engine.cpp` and `backend/TaskManager.h` are committed to GitHub so the application can compile the engine in its deployment environment.

After deployment, pushing changes to the connected GitHub branch can trigger an application redeployment.

## Usage

Once both the C++ engine and Streamlit are set up, you can start the application: 

### How to use the application:

*   **Add Tasks**: Use the task creation form on the dashboard to enter the task title, description, category, priority (High, Medium, Low), and deadline, then click **Add Task**.
*   **View & Filter Tasks**: See all your added tasks listed in the main panel. Use the filter dropdowns to view tasks by status (All, Pending, Completed) or priority level.
*   **Manage Task Status**: Click the checkbox or completion button next to a task to mark it as completed or reopen it.
*   **Delete Tasks**: Click the delete icon/button on any task card to remove it from your list.
*   **Undo / Redo Actions**: Use the **Undo** and **Redo** buttons in the interface to revert or reapply your recent task actions seamlessly, powered by the C++ backend stack.
*   **View Highest Priority Task**: Check the dedicated highlight card to instantly see the most critical pending task based on priority queue ordering.
*   **Search Tasks**: Type a keyword or task ID into the search bar to quickly filter and find specific tasks.

--- 


## Important Links

*   **Repository**: [GitHub - Deadeepya574/Smart-Task-Manager-using-DSA-concept](https://github.com/Deadeepya574/Smart-Task-Manager-using-DSA-concept)
*   **Project Link**: [Smart Task Manager]()
 
 
