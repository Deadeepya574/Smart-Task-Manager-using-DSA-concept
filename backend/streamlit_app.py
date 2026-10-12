
import os
import subprocess
import threading
from pathlib import Path

import streamlit as st

ROOT = Path(__file__).resolve().parent
BACKEND = ROOT / "backend"
SOURCE = BACKEND / "engine.cpp"
HEADER = BACKEND / "TaskManager.h"
ENGINE = BACKEND / ("engine.exe" if os.name == "nt" else "engine")

st.set_page_config(
    page_title="Smart Task Manager",
    page_icon="📋",
    layout="wide"
)


@st.cache_resource
def start_engine():
    if not SOURCE.exists() or not HEADER.exists():
        raise FileNotFoundError(
            "Create backend/engine.cpp and backend/TaskManager.h first."
        )

    needs_compile = (
        not ENGINE.exists()
        or SOURCE.stat().st_mtime > ENGINE.stat().st_mtime
        or HEADER.stat().st_mtime > ENGINE.stat().st_mtime
    )

    if needs_compile:
        subprocess.run(
            [
                "g++",
                "-std=c++17",
                str(SOURCE),
                "-o",
                str(ENGINE)
            ],
            check=True,
            cwd=str(BACKEND)
        )

    process = subprocess.Popen(
        [str(ENGINE)],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
        bufsize=1,
        cwd=str(BACKEND)
    )

    return process, threading.Lock()


def command(cmd):
    process, lock = start_engine()

    with lock:
        process.stdin.write(cmd + "\n")
        process.stdin.flush()

        response = process.stdout.readline().strip()

        if not response:
            raise RuntimeError("The C++ engine stopped.")

        if response.startswith("ERROR"):
            raise ValueError(response.split("|", 1)[-1])

        return response


def get_tasks():
    process, lock = start_engine()
    tasks = []

    with lock:
        process.stdin.write("LIST\n")
        process.stdin.flush()

        while True:
            line = process.stdout.readline().strip()

            if line == "END":
                break

            if not line.startswith("TASK|"):
                raise RuntimeError("Unexpected response from C++ engine.")

            parts = line.split("|")

            tasks.append({
                "id": int(parts[1]),
                "title": parts[2],
                "description": parts[3],
                "priority": int(parts[4]),
                "deadline": parts[5],
                "completed": parts[6] == "1",
                "category": parts[7]
            })

    return tasks


def safe_text(value):
    return (
        isinstance(value, str)
        and "|" not in value
        and "\n" not in value
        and "\r" not in value
    )


def validate_task(title, description, deadline, category):
    values = [title, description, deadline, category]

    if not all(safe_text(value) for value in values):
        st.error("Text fields cannot contain | or line breaks.")
        return False

    if not title.strip():
        st.error("Task title is required.")
        return False

    return True


st.title("📋 Smart Task Manager")
st.caption("Organize tasks, prioritize work, and track progress.")

try:
    tasks = get_tasks()
except Exception as error:
    st.error(f"Could not start the C++ engine: {error}")
    st.stop()

total = len(tasks)
completed = sum(task["completed"] for task in tasks)
pending = total - completed
high_priority = sum(
    task["priority"] == 3 and not task["completed"]
    for task in tasks
)

col1, col2, col3, col4 = st.columns(4)
col1.metric("Total Tasks", total)
col2.metric("Pending", pending)
col3.metric("Completed", completed)
col4.metric("High Priority", high_priority)

st.divider()

left, right = st.columns([1, 1.6])

with left:
    st.subheader("Add a Task")

    with st.form("add_task_form", clear_on_submit=True):
        title = st.text_input("Task title")
        description = st.text_area("Description")
        priority_label = st.selectbox(
            "Priority", ["High", "Medium", "Low"]
        )
        deadline = st.date_input(
            "Deadline", value=None
        )
        category = st.text_input("Category")

        submitted = st.form_submit_button(
            "Add Task", use_container_width=True
        )

        if submitted:
            deadline_text = deadline.isoformat() if deadline else ""
            priority = {
                "High": 3,
                "Medium": 2,
                "Low": 1
            }[priority_label]

            if validate_task(
                title, description, deadline_text, category
            ):
                try:
                    command(
                        f"ADD|{title}|{description}|{priority}|"
                        f"{deadline_text}|{category}"
                    )
                    st.success("Task added.")
                    st.rerun()
                except Exception as error:
                    st.error(str(error))

    st.subheader("Task History")

    history1, history2 = st.columns(2)

    with history1:
        if st.button("↶ Undo", use_container_width=True):
            try:
                command("UNDO")
                st.rerun()
            except Exception as error:
                st.error(str(error))

    with history2:
        if st.button("↷ Redo", use_container_width=True):
            try:
                command("REDO")
                st.rerun()
            except Exception as error:
                st.error(str(error))

with right:
    st.subheader("Your Tasks")

    search = st.text_input("🔎 Search tasks")
    status_filter = st.selectbox(
        "Status",
        ["All", "Pending", "Completed"]
    )
    priority_filter = st.selectbox(
        "Priority filter",
        ["All", "High", "Medium", "Low"]
    )

    visible_tasks = tasks

    if search:
        query = search.lower()
        visible_tasks = [
            task for task in visible_tasks
            if query in task["title"].lower()
            or query in task["description"].lower()
            or query in task["category"].lower()
        ]

    if status_filter == "Pending":
        visible_tasks = [
            task for task in visible_tasks if not task["completed"]
        ]
    elif status_filter == "Completed":
        visible_tasks = [
            task for task in visible_tasks if task["completed"]
        ]

    priority_values = {
        "High": 3,
        "Medium": 2,
        "Low": 1
    }

    if priority_filter != "All":
        visible_tasks = [
            task for task in visible_tasks
            if task["priority"] == priority_values[priority_filter]
        ]

    for task in visible_tasks:
        priority_name = {
            3: "High",
            2: "Medium",
            1: "Low"
        }[task["priority"]]

        with st.container(border=True):
            st.markdown(f"### {task['title']}")
            st.write(task["description"] or "No description")
            st.caption(
                f"ID: {task['id']} · "
                f"Priority: {priority_name} · "
                f"Deadline: {task['deadline'] or 'Not set'} · "
                f"Category: {task['category'] or 'None'}"
            )

            st.write(
                "✅ Completed" if task["completed"] else "⏳ Pending"
            )

            a, b, c = st.columns(3)

            with a:
                label = "Mark Pending" if task["completed"] else "Complete"

                if st.button(label, key=f"complete_{task['id']}"):
                    try:
                        command(f"COMPLETE|{task['id']}")
                        st.rerun()
                    except Exception as error:
                        st.error(str(error))

            with b:
                with st.popover("Edit"):
                    with st.form(f"edit_{task['id']}"):
                        new_title = st.text_input(
                            "Title", value=task["title"]
                        )
                        new_description = st.text_area(
                            "Description", value=task["description"]
                        )
                        new_priority = st.selectbox(
                            "Priority",
                            ["High", "Medium", "Low"],
                            index=3 - task["priority"]
                        )
                        new_deadline = st.text_input(
                            "Deadline (YYYY-MM-DD)",
                            value=task["deadline"]
                        )
                        new_category = st.text_input(
                            "Category", value=task["category"]
                        )

                        save = st.form_submit_button("Save changes")

                        if save and validate_task(
                            new_title,
                            new_description,
                            new_deadline,
                            new_category
                        ):
                            try:
                                priority = {
                                    "High": 3,
                                    "Medium": 2,
                                    "Low": 1
                                }[new_priority]

                                command(
                                    f"UPDATE|{task['id']}|{new_title}|"
                                    f"{new_description}|{priority}|"
                                    f"{new_deadline}|{new_category}"
                                )
                                st.success("Task updated.")
                                st.rerun()
                            except Exception as error:
                                st.error(str(error))

            with c:
                if st.button("Delete", key=f"delete_{task['id']}"):
                    try:
                        command(f"DELETE|{task['id']}")
                        st.rerun()
                    except Exception as error:
                        st.error(str(error))

    if not visible_tasks:
        st.info("No tasks match your filters.")
