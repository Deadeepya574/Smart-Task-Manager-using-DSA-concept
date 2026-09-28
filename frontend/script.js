
const API_URL = "http://127.0.0.1:5000";

let tasks = [];

async function apiRequest(endpoint, method = "GET", body = null) {
    const options = {
        method,
        headers: {
            "Content-Type": "application/json"
        }
    };

    if (body !== null) {
        options.body = JSON.stringify(body);
    }

    const response = await fetch(`${API_URL}${endpoint}`, options);

    const data = await response.json();

    if (!response.ok) {
        throw new Error(data.error || "Something went wrong");
    }

    return data;
}

async function loadTasks() {
    try {
        tasks = await apiRequest("/tasks");
        renderTasks();
    } catch (error) {
        alert(error.message);
    }
}

async function addTask() {
    const title = document.getElementById("title").value.trim();
    const description = document.getElementById("description").value.trim();
    const priority = Number(document.getElementById("priority").value);
    const deadline = document.getElementById("deadline").value;
    const category = document.getElementById("category").value.trim();

    if (!title) {
        alert("Please enter a task title.");
        return;
    }

    try {
        await apiRequest("/tasks", "POST", {
            title,
            description,
            priority,
            deadline,
            category
        });

        clearForm();
        await loadTasks();

    } catch (error) {
        alert(error.message);
    }
}

function clearForm() {
    document.getElementById("title").value = "";
    document.getElementById("description").value = "";
    document.getElementById("priority").value = "3";
    document.getElementById("deadline").value = "";
    document.getElementById("category").value = "";
}

function renderTasks() {
    const taskList = document.getElementById("taskList");

    const searchText =
        document.getElementById("search").value.toLowerCase();

    const filter =
        document.getElementById("filter").value;

    const filteredTasks = tasks.filter(task => {

        const matchesSearch =
            task.title.toLowerCase().includes(searchText) ||
            task.description.toLowerCase().includes(searchText) ||
            task.category.toLowerCase().includes(searchText);

        let matchesFilter = true;

        if (filter === "pending") {
            matchesFilter = !task.completed;
        }

        if (filter === "completed") {
            matchesFilter = task.completed;
        }

        if (filter === "high") {
            matchesFilter = task.priority === 3;
        }

        return matchesSearch && matchesFilter;
    });

    taskList.innerHTML = "";

    if (filteredTasks.length === 0) {
        taskList.innerHTML = "<p>No tasks found.</p>";
        updateDashboard();
        return;
    }

    filteredTasks.forEach(task => {

        const card = document.createElement("div");

        card.className = "task-card";

        card.innerHTML = `
            <h3>${escapeHTML(task.title)}</h3>

            <p>${escapeHTML(task.description)}</p>

            <div class="task-info">
                <span>ID: ${task.id}</span>
                <span>Priority: ${getPriorityText(task.priority)}</span>
                <span>Deadline: ${escapeHTML(task.deadline || "Not set")}</span>
                <span>Category: ${escapeHTML(task.category || "None")}</span>
                <span>Status: ${task.completed ? "Completed" : "Pending"}</span>
            </div>

            <div class="task-actions">
                <button onclick="toggleTask(${task.id})">
                    ${task.completed ? "Mark Pending" : "Complete"}
                </button>

                <button onclick="editTask(${task.id})">
                    Edit
                </button>

                <button onclick="deleteTask(${task.id})">
                    Delete
                </button>
            </div>
        `;

        taskList.appendChild(card);
    });

    updateDashboard();
}

function getPriorityText(priority) {
    if (priority === 3) return "High";
    if (priority === 2) return "Medium";
    return "Low";
}

async function toggleTask(id) {
    try {
        await apiRequest(`/tasks/${id}/complete`, "POST");
        await loadTasks();
    } catch (error) {
        alert(error.message);
    }
}

async function deleteTask(id) {
    try {
        await apiRequest(`/tasks/${id}`, "DELETE");
        await loadTasks();
    } catch (error) {
        alert(error.message);
    }
}

async function editTask(id) {
    const task = tasks.find(task => task.id === id);

    if (!task) return;

    const title = prompt("Enter new title:", task.title);

    if (title === null || title.trim() === "") {
        return;
    }

    const description =
        prompt("Enter new description:", task.description);

    if (description === null) {
        return;
    }

    try {
        await apiRequest(`/tasks/${id}`, "PUT", {
            title: title.trim(),
            description,
            priority: task.priority,
            deadline: task.deadline,
            category: task.category
        });

        await loadTasks();

    } catch (error) {
        alert(error.message);
    }
}

async function undo() {
    try {
        await apiRequest("/undo", "POST");
        await loadTasks();
    } catch (error) {
        alert(error.message);
    }
}

async function redo() {
    try {
        await apiRequest("/redo", "POST");
        await loadTasks();
    } catch (error) {
        alert(error.message);
    }
}

function updateDashboard() {
    const total = tasks.length;

    const completed =
        tasks.filter(task => task.completed).length;

    const pending = total - completed;

    const highPriority =
        tasks.filter(
            task => task.priority === 3 && !task.completed
        ).length;

    document.getElementById("totalTasks").textContent = total;
    document.getElementById("pendingTasks").textContent = pending;
    document.getElementById("completedTasks").textContent = completed;
    document.getElementById("highPriorityTasks").textContent = highPriority;
}

function escapeHTML(value) {
    const div = document.createElement("div");
    div.textContent = String(value ?? "");
    return div.innerHTML;
}

document.getElementById("search").addEventListener(
    "input",
    renderTasks
);

document.getElementById("filter").addEventListener(
    "change",
    renderTasks
);

loadTasks();