
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
    page_title="TaskFlow | Smart Task Manager",
    page_icon="📋",
    layout="wide",
    initial_sidebar_state="expanded"
)

st.markdown("""
<style>
@import url('https://fonts.googleapis.com/css2?family=DM+Sans:wght@400;500;600;700&family=Manrope:wght@400;500;600;700;800&display=swap');

html, body, [class*="css"] {
    font-family: 'DM Sans', sans-serif;
}

.stApp {
    background: #f5f7f6;
}

.block-container {
    max-width: 1450px;
    padding-top: 2rem;
    padding-bottom: 3rem;
}

[data-testid="stSidebar"] {
    background: #102b25;
}

[data-testid="stSidebar"] * {
    color: #f0f8f4;
}

[data-testid="stSidebar"] .stButton button {
    background: #1c4439;
    border: 1px solid #31594d;
    color: white;
}

.hero {
    background: linear-gradient(125deg, #102f27, #176653);
    border-radius: 24px;
    padding: 32px 36px;
    color: white;
    margin-bottom: 26px;
    box-shadow: 0 12px 32px rgba(16,47,39,.13);
}

.hero h1 {
    color: white;
    font-family: 'Manrope', sans-serif;
    font-size: 34px;
    font-weight: 800;
    margin: 0 0 8px 0;
}

.hero p {
    color: #d0e7dd;
    font-size: 15px;
    margin-bottom: 0;
}

.eyebrow {
    color: #a9e5ce;
    font-size: 11px;
    font-weight: 700;
    letter-spacing: 2px;
    text-transform: uppercase;
    margin-bottom: 10px;
}

.metric-card {
    border-radius: 18px;
    padding: 22px;
    background: white;
    border: 1px solid #e6ece8;
    box-shadow: 0 5px 18px rgba(23,55,43,.035);
    min-height: 135px;
}

.metric-label {
    color: #718078;
    font-size: 13px;
    font-weight: 600;
}

.metric-value {
    font-family: 'Manrope', sans-serif;
    color: #173b30;
    font-size: 34px;
    font-weight: 800;
    margin-top: 9px;
}

.metric-icon {
    font-size: 22px;
    margin-bottom: 10px;
}

.section-title {
    font-family: 'Manrope', sans-serif;
    color: #173b30;
    font-size: 22px;
    font-weight: 800;
    margin: 8px 0 4px 0;
}

.section-subtitle {
    color: #829087;
    font-size: 13px;
    margin-bottom: 18px;
}

.panel {
    background: white;
    border: 1px solid #e4ebe6;
    border-radius: 20px;
    padding: 24px;
    margin-bottom: 18px;
}

.task-title {
    color: #173b30;
    font-family: 'Manrope', sans-serif;
    font-size: 18px;
    font-weight: 800;
    overflow-wrap: anywhere;
}

.task-description {
    color: #728078;
    font-size: 14px;
    margin: 7px 0 14px 0;
    overflow-wrap: anywhere;
}

.task-meta {
    color: #66756c;
    font-size: 12px;
    line-height: 1.9;
}

.priority-high {
    color: #b42318;
    background: #fff0ee;
    border-radius: 8px;
    padding: 5px 9px;
    font-size: 11px;
    font-weight: 700;
}

.priority-medium {
    color: #9a6700;
    background: #fff6dc;
    border-radius: 8px;
    padding: 5px 9px;
    font-size: 11px;
    font-weight: 700;
}

.priority-low {
    color: #176653;
    background: #e5f6ed;
    border-radius: 8px;
    padding: 5px 9px;
    font-size: 11px;
    font-weight: 700;
}

.status-completed {
    color: #176653;
    background: #e5f6ed;
    border-radius: 8px;
    padding: 5px 9px;
    font-size: 11px;
    font-weight: 700;
}

.status-pending {
    color: #8a6200;
    background: #fff5d7;
    border-radius: 8px;
    padding: 5px 9px;
    font-size: 11px;
    font-weight: 700;
}

.stButton button {
    border-radius: 10px;
    font-weight: 600;
    min-height: 40px;
    transition: all .2s ease;
}

.stButton button[kind="primary"] {
    background: #176653;
    border-color: #176653;
}

.stButton button:hover {
    border-color: #176653;
    color: #176653;
}

div[data-baseweb="input"] input,
div[data-baseweb="textarea"] textarea {
    border-radius: 10px;
}

div[data-testid="stForm"] {
    background: #ffffff;
    border: 1px solid #e4ebe6;
    border-radius: 18px;
    padding: 22px;
}

div[data-testid="stProgress"] > div > div {
    background: #176653;
}

hr {
    border-color: #e4ebe6;
}

.footer {
    color: #829087;
    font-size: 12px;
    text-align: center;
    padding: 25px 0 0 0;
}
</style>
""", unsafe_allow_html=True)


@st.cache_resource
def start_engine():
    if not SOURCE.exists() or not HEADER.exists():
        raise FileNotFoundError(
            "Missing backend/engine.cpp or backend/TaskManager.h."
        )

    needs_compile = (
        not ENGINE.exists()
        or SOURCE.stat().st_mtime > ENGINE.stat().st_mtime
        or HEADER.stat().st_mtime > ENGINE.stat().st_mtime
    )

    if needs_compile:
        subprocess.run(
            ["g++", "-std=c++17", str(SOURCE), "-o", str(ENGINE)],
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
            raise RuntimeError("C++ engine stopped unexpectedly.")

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
    if not all(safe_text(v) for v in [
        title, description, deadline, category
    ]):
        st.error("Text fields cannot contain | or line breaks.")
        return False

    if not title.strip():
        st.error("Please enter a task title.")
        return False

    return True


def priority_name(priority):
    return {3: "High", 2: "Medium", 1: "Low"}[priority]


def priority_class(priority):
    return {
        3: "priority-high",
        2: "priority-medium",
        1: "priority-low"
    }[priority]


def render_metric(icon, label, value):
    st.markdown(
        f"""
        <div class="metric-card">
            <div class="metric-icon">{icon}</div>
            <div class="metric-label">{label}</div>
            <div class="metric-value">{value}</div>
        </div>
        """,
        unsafe_allow_html=True
    )


st.markdown("""
<div class="hero">
    <div class="eyebrow">YOUR PERSONAL PRODUCTIVITY SPACE</div>
    <h1>Smart Task Manager ✦</h1>
    <p>Plan smarter. Prioritize better. Make every day count.</p>
</div>
""", unsafe_allow_html=True)

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

m1, m2, m3, m4 = st.columns(4)

with m1:
    render_metric("📚", "TOTAL TASKS", total)
with m2:
    render_metric("⏳", "PENDING", pending)
with m3:
    render_metric("✅", "COMPLETED", completed)
with m4:
    render_metric("🔥", "HIGH PRIORITY", high_priority)

st.write("")

left, right = st.columns([0.85, 1.45], gap="large")

with left:
    st.markdown(
        '<div class="section-title">✚ Create a task</div>'
        '<div class="section-subtitle">Add something you want to accomplish.</div>',
        unsafe_allow_html=True
    )

    with st.form("add_task_form", clear_on_submit=True):
        title = st.text_input(
            "Task title", placeholder="e.g. Complete DSA practice"
        )
        description = st.text_area(
            "Description", placeholder="What needs to be done?"
        )

        p1, p2 = st.columns(2)

        with p1:
            priority_label = st.selectbox(
                "Priority", ["High", "Medium", "Low"]
            )

        with p2:
            category = st.selectbox(
                "Category",
                ["Study", "Work", "Personal", "Project", "Other"]
            )

        deadline = st.date_input(
            "Deadline", value=None
        )

        submitted = st.form_submit_button(
            "✚ Add task", type="primary", use_container_width=True
        )

        if submitted:
            deadline_text = deadline.isoformat() if deadline else ""
            priority = {"High": 3, "Medium": 2, "Low": 1}[priority_label]

            if validate_task(title, description, deadline_text, category):
                try:
                    command(
                        f"ADD|{title}|{description}|{priority}|"
                        f"{deadline_text}|{category}"
                    )
                    st.success("Your task has been added!")
                    st.rerun()
                except Exception as error:
                    st.error(str(error))

    st.markdown(
        '<div class="section-title">↶ Task history</div>'
        '<div class="section-subtitle">Reverse or restore your last action.</div>',
        unsafe_allow_html=True
    )

    h1, h2 = st.columns(2)

    with h1:
        if st.button("↶ Undo", use_container_width=True):
            try:
                command("UNDO")
                st.rerun()
            except Exception as error:
                st.error(str(error))

    with h2:
        if st.button("↷ Redo", use_container_width=True):
            try:
                command("REDO")
                st.rerun()
            except Exception as error:
                st.error(str(error))

    st.markdown(
        '<div class="panel">'
        '<div class="task-title">A little progress adds up.</div>'
        '<div class="task-description">Keep moving forward, one task at a time.</div>'
        '</div>',
        unsafe_allow_html=True
    )

with right:
    st.markdown(
        '<div class="section-title">Your workspace</div>'
        '<div class="section-subtitle">Find, filter, and manage everything on your list.</div>',
        unsafe_allow_html=True
    )

    search = st.text_input(
        "🔎 Search",
        placeholder="Search title, description, or category"
    )

    f1, f2 = st.columns(2)

    with f1:
        status_filter = st.selectbox(
            "Status", ["All", "Pending", "Completed"]
        )

    with f2:
        priority_filter = st.selectbox(
            "Priority", ["All", "High", "Medium", "Low"]
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

    if priority_filter != "All":
        priority_value = {
            "High": 3, "Medium": 2, "Low": 1
        }[priority_filter]

        visible_tasks = [
            task for task in visible_tasks
            if task["priority"] == priority_value
        ]

    st.caption(f"Showing {len(visible_tasks)} of {total} tasks")

    if total:
        st.progress(completed / total, text=f"{completed} of {total} tasks completed")

    if not visible_tasks:
        st.info("✨ No matching tasks. Try changing your filters or add a new task.")

    for task in visible_tasks:
        p_name = priority_name(task["priority"])
        p_class = priority_class(task["priority"])
        status_text = "Completed" if task["completed"] else "Pending"
        status_class = "status-completed" if task["completed"] else "status-pending"

        with st.container(border=True):
            st.markdown(
                f"""
                <div class="task-title">{task['title']}</div>
                <div class="task-description">{task['description'] or 'No description provided.'}</div>
                <span class="{p_class}">{p_name} priority</span>
                &nbsp;
                <span class="{status_class}">{status_text}</span>
                <div class="task-meta">
                    🗓️ Deadline: {task['deadline'] or 'Not set'}<br>
                    🏷️ Category: {task['category'] or 'None'}<br>
                    🔢 Task ID: {task['id']}
                </div>
                """,
                unsafe_allow_html=True
            )

            a, b, c = st.columns(3)

            with a:
                label = "↩ Pending" if task["completed"] else "✓ Complete"

                if st.button(label, key=f"complete_{task['id']}", use_container_width=True):
                    try:
                        command(f"COMPLETE|{task['id']}")
                        st.rerun()
                    except Exception as error:
                        st.error(str(error))

            with b:
                with st.popover("✎ Edit", use_container_width=True):
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
                            "Deadline (YYYY-MM-DD)", value=task["deadline"]
                        )
                        new_category = st.text_input(
                            "Category", value=task["category"]
                        )

                        save = st.form_submit_button(
                            "Save changes", use_container_width=True
                        )

                        if save and validate_task(
                            new_title,
                            new_description,
                            new_deadline,
                            new_category
                        ):
                            priority = {
                                "High": 3, "Medium": 2, "Low": 1
                            }[new_priority]

                            try:
                                command(
                                    f"UPDATE|{task['id']}|{new_title}|"
                                    f"{new_description}|{priority}|"
                                    f"{new_deadline}|{new_category}"
                                )
                                st.success("Task updated!")
                                st.rerun()
                            except Exception as error:
                                st.error(str(error))

            with c:
                if st.button(
                    "🗑 Delete",
                    key=f"delete_{task['id']}",
                    use_container_width=True
                ):
                    try:
                        command(f"DELETE|{task['id']}")
                        st.rerun()
                    except Exception as error:
                        st.error(str(error))

st.markdown(
    '<div class="footer">SMART TASK MANAGER · BUILT WITH STREAMLIT, C++ & DSA</div>',
    unsafe_allow_html=True
)
