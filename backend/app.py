from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import subprocess
import threading
from pathlib import Path

app = Flask(__name__)
CORS(app)

ENGINE_PATH = Path(__file__).resolve().parent / "engine.exe"

engine = subprocess.Popen(
    [str(ENGINE_PATH)],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.DEVNULL,
    text=True,
    bufsize=1
)

engine_lock = threading.Lock()


def send_command(command):
    with engine_lock:
        engine.stdin.write(command + "\n")
        engine.stdin.flush()

        response = engine.stdout.readline().strip()

        if not response:
            raise RuntimeError("C++ engine stopped unexpectedly")

        return response


def get_tasks():
    with engine_lock:
        engine.stdin.write("LIST\n")
        engine.stdin.flush()

        tasks = []

        while True:
            line = engine.stdout.readline().strip()

            if line == "END":
                break

            if line.startswith("TASK|"):
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


def validate_text(value):
    return (
        isinstance(value, str)
        and "|" not in value
        and "\n" not in value
        and "\r" not in value
    )


FRONTEND_PATH = Path(__file__).resolve().parent.parent / "frontend"


@app.route("/")
def home():
    return send_from_directory(FRONTEND_PATH, "index.html")


@app.route("/<path:filename>")
def frontend_files(filename):
    return send_from_directory(FRONTEND_PATH, filename)

@app.route("/tasks", methods=["GET"])
def list_tasks():
    return jsonify(get_tasks())


@app.route("/tasks", methods=["POST"])
def add_task():
    data = request.get_json(silent=True) or {}

    title = data.get("title", "")
    description = data.get("description", "")
    priority = data.get("priority", 1)
    deadline = data.get("deadline", "")
    category = data.get("category", "")

    if not all(validate_text(x) for x in [
        title, description, deadline, category
    ]):
        return jsonify({"error": "Invalid text input"}), 400

    if not title.strip():
        return jsonify({"error": "Title is required"}), 400

    try:
        priority = int(priority)
    except (TypeError, ValueError):
        return jsonify({"error": "Priority must be 1, 2, or 3"}), 400

    if priority not in [1, 2, 3]:
        return jsonify({"error": "Priority must be 1, 2, or 3"}), 400

    response = send_command(
        f"ADD|{title}|{description}|{priority}|{deadline}|{category}"
    )

    if response.startswith("ERROR"):
        return jsonify({"error": response}), 400

    return jsonify(get_tasks()), 201


@app.route("/tasks/<int:task_id>", methods=["PUT"])
def update_task(task_id):
    data = request.get_json(silent=True) or {}

    title = data.get("title", "")
    description = data.get("description", "")
    priority = data.get("priority", 1)
    deadline = data.get("deadline", "")
    category = data.get("category", "")

    if not all(validate_text(x) for x in [
        title, description, deadline, category
    ]):
        return jsonify({"error": "Invalid text input"}), 400

    try:
        priority = int(priority)
    except (TypeError, ValueError):
        return jsonify({"error": "Invalid priority"}), 400

    if priority not in [1, 2, 3]:
        return jsonify({"error": "Priority must be 1, 2, or 3"}), 400

    response = send_command(
        f"UPDATE|{task_id}|{title}|{description}|{priority}|{deadline}|{category}"
    )

    if response.startswith("ERROR"):
        return jsonify({"error": response}), 404

    return jsonify(get_tasks())


@app.route("/tasks/<int:task_id>", methods=["DELETE"])
def delete_task(task_id):
    response = send_command(f"DELETE|{task_id}")

    if response.startswith("ERROR"):
        return jsonify({"error": response}), 404

    return jsonify(get_tasks())


@app.route("/tasks/<int:task_id>/complete", methods=["POST"])
def complete_task(task_id):
    response = send_command(f"COMPLETE|{task_id}")

    if response.startswith("ERROR"):
        return jsonify({"error": response}), 404

    return jsonify(get_tasks())


@app.route("/undo", methods=["POST"])
def undo():
    response = send_command("UNDO")

    if response.startswith("ERROR"):
        return jsonify({"error": response}), 400

    return jsonify(get_tasks())


@app.route("/redo", methods=["POST"])
def redo():
    response = send_command("REDO")

    if response.startswith("ERROR"):
        return jsonify({"error": response}), 400

    return jsonify(get_tasks())


if __name__ == "__main__":
    app.run(debug=True, use_reloader=False, port=5000)