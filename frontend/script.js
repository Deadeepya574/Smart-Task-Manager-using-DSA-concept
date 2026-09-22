let tasks = [];
let nextId = 1;

let undoStack = [];
let redoStack = [];

function addTask() {
    const title = document.getElementById("title").value.trim();
    const description = document.getElementById("description").value.trim();
    const priority = Number(document.getElementById("priority").value);
    const deadline = document.getElementById("deadline").value;
    const category = document.getElementById("category").value.trim();

    if (!title) {
        alert("Please enter a task title.");
        return;
    }

    const task = {
        id: nextId++,
        title,
        description,
        priority,
        deadline,
        category,
        completed: false
    };

    tasks.push(task);

    undoStack.push({
        type: "ADD",
        task: { ...task }
    });

    redoStack = [];

    clearForm();
    renderTasks();
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

    let filteredTasks = tasks.filter(task => {

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
                <span>Deadline: ${task.deadline || "Not set"}</span>
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

function toggleTask(id) {
    const task = tasks.find(task => task.id === id);

    if (!task) return;

    const oldTask = { ...task };

    task.completed = !task.completed;

    const newTask = { ...task };

    undoStack.push({
        type: "COMPLETE",
        oldTask,
        newTask
    });

    redoStack = [];

    renderTasks();
}

function deleteTask(id) {
    const index = tasks.findIndex(task => task.id === id);

    if (index === -1) return;

    const deletedTask = { ...tasks[index] };

    tasks.splice(index, 1);

    undoStack.push({
        type: "DELETE",
        task: deletedTask,
        index
    });

    redoStack = [];

    renderTasks();
}

function editTask(id) {
    const task = tasks.find(task => task.id === id);

    if (!task) return;

    const oldTask = { ...task };

    const title = prompt("Enter new title:", task.title);

    if (title === null || title.trim() === "") {
        return;
    }

    const description =
        prompt("Enter new description:", task.description);

    if (description === null) {
        return;
    }

    task.title = title.trim();
    task.description = description;

    const newTask = { ...task };

    undoStack.push({
        type: "UPDATE",
        oldTask,
        newTask
    });

    redoStack = [];

    renderTasks();
}

function undo() {
    if (undoStack.length === 0) {
        alert("Nothing to undo.");
        return;
    }

    const action = undoStack.pop();

    if (action.type === "ADD") {

        const index =
            tasks.findIndex(task => task.id === action.task.id);

        if (index !== -1) {
            tasks.splice(index, 1);
        }

        redoStack.push(action);
    }

    else if (action.type === "DELETE") {

        tasks.splice(action.index, 0, action.task);

        redoStack.push(action);
    }

    else if (action.type === "UPDATE") {

        const index =
            tasks.findIndex(task => task.id === action.newTask.id);

        if (index !== -1) {
            tasks[index] = { ...action.oldTask };
        }

        redoStack.push(action);
    }

    else if (action.type === "COMPLETE") {

        const index =
            tasks.findIndex(task => task.id === action.newTask.id);

        if (index !== -1) {
            tasks[index] = { ...action.oldTask };
        }

        redoStack.push(action);
    }

    renderTasks();
}

function redo() {
    if (redoStack.length === 0) {
        alert("Nothing to redo.");
        return;
    }

    const action = redoStack.pop();

    if (action.type === "ADD") {

        tasks.push({ ...action.task });

        undoStack.push(action);
    }

    else if (action.type === "DELETE") {

        const index =
            tasks.findIndex(task => task.id === action.task.id);

        if (index !== -1) {
            tasks.splice(index, 1);
        }

        undoStack.push(action);
    }

    else if (action.type === "UPDATE") {

        const index =
            tasks.findIndex(task => task.id === action.newTask.id);

        if (index !== -1) {
            tasks[index] = { ...action.newTask };
        }

        undoStack.push(action);
    }

    else if (action.type === "COMPLETE") {

        const index =
            tasks.findIndex(task => task.id === action.newTask.id);

        if (index !== -1) {
            tasks[index] = { ...action.newTask };
        }

        undoStack.push(action);
    }

    renderTasks();
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
    div.textContent = value;
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

renderTasks();
