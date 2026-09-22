#include <bits/stdc++.h>
using namespace std;

struct Task
{
    int id;
    string title;
    string description;
    int priority;
    string deadline;
    bool completed;
    string category;
};

enum class ActionType
{
    ADD,
    DELETE,
    UPDATE,
    COMPLETE
};

struct Action
{
    ActionType type;
    Task oldTask;
    Task newTask;
};

struct ComparePriority
{
    bool operator()(const Task &a, const Task &b)
    {
        return a.priority < b.priority;
    }
};

class TaskManager
{
private:
    vector<Task> tasks;
    stack<Action> undoStack;
    stack<Action> redoStack;
    int nextnum = 1;

public:
    int findTaskIndex(int id)
    {

        for (int i = 0; i < tasks.size(); i++)
        {

            if (tasks[i].id == id)
                return i;
        }

        return -1;
    }

    void addTask()
    {
        Task task;

        task.id = nextnum++;

        cout << "Enter Task Title";
        getline(cin >> ws, task.title);

        cout << "Enter description: ";
        getline(cin, task.description);

        cout << "Enter Priority (3,2,1)";
        cin >> task.priority;

        cout << "Enter Deadline (YYY-MM-DD): ";
        cin >> task.deadline;

        cout << "Enter Category: ";
        getline(cin >> ws, task.category);

        task.completed = false;

        tasks.push_back(task);

        Action action;
        action.type = ActionType::ADD;
        action.newTask = task;

        undoStack.push(action);

        while (!redoStack.empty())
        {
            redoStack.pop();
        }

        cout << "Task added successfully!! ";
        return;
    }

    void displayTasks()
{
    if (tasks.empty())
    {
        cout << "\nNo tasks available.\n";
        return;
    }

    cout << "\n========== ALL TASKS ==========\n";

    for (const Task& task : tasks)
    {
        cout << "\n-----------------------------\n";
        cout << "ID          : " << task.id << endl;
        cout << "Title       : " << task.title << endl;
        cout << "Description : " << task.description << endl;

        cout << "Priority    : ";

        if (task.priority == 3)
            cout << "High";
        else if (task.priority == 2)
            cout << "Medium";
        else
            cout << "Low";

        cout << endl;

        cout << "Deadline    : " << task.deadline << endl;

        cout << "Status      : "
             << (task.completed ? "Completed" : "Pending")
             << endl;

        cout << "Category    : " << task.category << endl;
    }

    cout << "\n-----------------------------\n";
}

    void updateTask()
    {

        int id;

        cout << "\nEnter Task ID to update: ";
        cin >> id;

        int index = findTaskIndex(id);

        if (index == -1)
        {
            cout << "\nTask not found.\n";
            return;
        }

        Task oldTask = tasks[index];

        cout << "\nEnter new title: ";
        getline(cin >> ws, tasks[index].title);

        cout << "Enter new description: ";
        getline(cin, tasks[index].description);

        cout << "Enter new priority (3 = High, 2 = Medium, 1 = Low): ";
        cin >> tasks[index].priority;

        cout << "Enter new deadline (YYYY-MM-DD): ";
        cin >> tasks[index].deadline;

        cout << "Enter new category: ";
        getline(cin >> ws, tasks[index].category);
        Task newTask = tasks[index];
        Action action;

        action.type = ActionType::UPDATE;
        action.oldTask = oldTask;
        action.newTask = newTask;

        undoStack.push(action);
        while (!redoStack.empty())
            redoStack.pop();

        cout << "\nTask updated successfully!\n";
    }

    void deleteTask()
    {

        int id;

        cout << "\nEnter Task ID to delete: ";
        cin >> id;

        int index = findTaskIndex(id);

        if (index == -1)
        {
            cout << "\nTask not found.\n";
            return;
        }

        Task deletedTask = tasks[index];

        tasks.erase(tasks.begin() + index);

        Action action;

        action.type = ActionType::DELETE;
        action.oldTask = deletedTask;

        undoStack.push(action);

        while (!redoStack.empty())
            redoStack.pop();

        cout << "Task deleted successfully!";
    }

    void completeTask()
    {

        int id;

        cout << "\nEnter Task ID: ";
        cin >> id;

        int index = findTaskIndex(id);

        if (index == -1)
        {
            cout << "\nTask not found.\n";
            return;
        }
        Task oldTask = tasks[index];
        tasks[index].completed = !tasks[index].completed;
        Task newTask = tasks[index];
        Action action;

        action.type = ActionType::COMPLETE;
        action.oldTask = oldTask;
        action.newTask = newTask;

        undoStack.push(action);
        while (!redoStack.empty())
            redoStack.pop();

        if (tasks[index].completed)
            cout << "\nTask marked as COMPLETED.\n";
        else
            cout << "\nTask marked as PENDING.\n";
    }

    void shownextpriorityTask()
    {
        if (tasks.empty())
        {
            cout << "No tasks";
            return;
        }
        priority_queue<Task, vector<Task>, ComparePriority> pq;

        for (const Task &task : tasks)
        {
            if (!task.completed)
            {
                pq.push(task);
            }
        }
        if (pq.empty())
        {
            cout << "All completed";
            return;
        }

        Task task = pq.top();

        cout << "ID       : " << task.id << endl;
        cout << "Title    : " << task.title << endl;
        cout << "Priority : ";

        if (task.priority == 3)
            cout << "High";
        else if (task.priority == 2)
            cout << "Medium";
        else
            cout << "Low";
    }

    void searchTaskbyid()
    {
        int id;
        cout << "Enter id to search";
        cin >> id;

        for (const Task &task : tasks)
        {
            if (task.id == id)
            {

                cout << "ID          : " << task.id << endl;
                cout << "Title       : " << task.title << endl;
                cout << "Description : " << task.description << endl;

                cout << "Priority    : ";

                if (task.priority == 3)
                    cout << "High";
                else if (task.priority == 2)
                    cout << "Medium";
                else
                    cout << "Low";

                cout << endl;

                cout << "Deadline    : " << task.deadline << endl;

                cout << "Status      : "
                     << (task.completed ? "Completed" : "Pending")
                     << endl;

                cout << "Category    : " << task.category << endl;
 

                return;
            }
        }
        cout << "Task Not Found";
    }

    void searchbyTitle()
    {
        string title;
        cout << "Enter the title to search";
        cin >> title;

        bool found = false;
        for (const Task &task : tasks)
        {

            if (task.title == title)
            {

                cout << "\nTask Found!";
                cout << "\nID       : " << task.id;
                cout << "\nTitle    : " << task.title;
                cout << "\nPriority : " << task.priority;
                cout << "\nDeadline : " << task.deadline;
                cout << "\nStatus   : "
                     << (task.completed ? "Completed" : "Pending")
                     << endl;

                found = true;
            }
        }
        if (!found)
        {
            cout << "No Task Found";
        }
    }

    void sortbypriority()
    {
        sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b)
             { return a.priority > b.priority; });

        cout << "Tasks sorted by priority";
        displayTasks();
    }

    void sortbydeadline()
    {
        sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b)
             { return a.deadline < b.deadline; });
        cout << "Tasks sorted by Deadline";
        displayTasks();
    }

    void filterbyPriority()
    {
        int priority;

        cout << "Enter Priority to filter";
        cin >> priority;

        bool found = false;

        for (const Task &task : tasks)
        {

            if (task.priority == priority)
            {

                cout << "\nID       : " << task.id;
                cout << "\nTitle    : " << task.title;
                cout << "\nPriority : " << task.priority;
                cout << "\nDeadline : " << task.deadline;
                cout << "\nStatus   : "
                     << (task.completed ? "Completed" : "Pending");

                found = true;
            }
        }
        if (!found)
        {
            cout << "No tasks with this priority";
        }
        cout << endl;
    }

    void filterbyStatus()
    {
        int choice;

        cout << "Enter num of status";
        cin >> choice;

        bool found = false;

        for (const Task &task : tasks)
        {

            bool isCompleted = task.completed;

            if ((choice == 1 && !isCompleted) ||
                (choice == 2 && isCompleted))
            {

                cout << "\nID       : " << task.id;
                cout << "\nTitle    : " << task.title;
                cout << "\nPriority : " << task.priority;
                cout << "\nDeadline : " << task.deadline;
                cout << "\nStatus   : "
                     << (task.completed ? "Completed" : "Pending");

                found = true;
            }
        }

        if (!found)
        {
            cout << "\nNo matching tasks found.\n";
        }

        cout << endl;
    }

    void filterTasks(int priority, bool completed)
    {
        bool found = false;

        for (const Task &task : tasks)
        {
            if (task.priority == priority && task.completed == completed)
            {
                cout << "Status: " << (task.completed ? "Completed" : "Pending");

                found = true;
            }
        }
        if (!found)
        {
            cout << "No matching tasks found";
        }
    }

    void undo()
    {

        if (undoStack.empty())
        {
            cout << "\nNothing to undo.\n";
            return;
        }

        Action action = undoStack.top();

        undoStack.pop();

        if (action.type == ActionType::ADD)
        {

            int index = findTaskIndex(action.newTask.id);

            if (index != -1)
            {
                tasks.erase(tasks.begin() + index);
            }
        }

        else if (action.type == ActionType::DELETE)
        {

            tasks.push_back(action.oldTask);
        }

        else if (action.type == ActionType::UPDATE)
        {

            int index = findTaskIndex(action.newTask.id);

            if (index != -1)
            {
                tasks[index] = action.oldTask;
            }
        }

        else if (action.type == ActionType::COMPLETE)
        {

            int index = findTaskIndex(action.newTask.id);

            if (index != -1)
            {
                tasks[index] = action.oldTask;
            }
        }

        redoStack.push(action);

        cout << "\nUndo successful!\n";
    }

    void redo()
    {

        if (redoStack.empty())
        {
            cout << "\nNothing to redo.\n";
            return;
        }

        Action action = redoStack.top();

        redoStack.pop();

        if (action.type == ActionType::ADD)
        {

            tasks.push_back(action.newTask);
        }

        else if (action.type == ActionType::DELETE)
        {

            int index = findTaskIndex(action.oldTask.id);

            if (index != -1)
            {
                tasks.erase(tasks.begin() + index);
            }
        }

        else if (action.type == ActionType::UPDATE)
        {

            int index = findTaskIndex(action.oldTask.id);

            if (index != -1)
            {
                tasks[index] = action.newTask;
            }
        }

        else if (action.type == ActionType::COMPLETE)
        {

            int index = findTaskIndex(action.oldTask.id);

            if (index != -1)
            {
                tasks[index] = action.newTask;
            }
        }

        undoStack.push(action);

        cout << "\nRedo successful!\n";
    }
};

int main()
{
    TaskManager manager;

    int choice;

    do
    {
        cout << "========== SMART TASK MANAGER ==========";
        cout << endl;
        cout << "1.  Add Task\n";
        cout << "2.  Display Tasks\n";
        cout << "3.  Update Task\n";
        cout << "4.  Delete Task\n";
        cout << "5.  Complete / Incomplete Task\n";
        cout << "6.  Show Highest Priority Task\n";
        cout << "7.  Search Task by ID\n";
        cout << "8.  Search Task by Title\n";
        cout << "9.  Sort Tasks by Priority\n";
        cout << "10. Sort Tasks by Deadline\n";
        cout << "11. Filter Tasks by Priority\n";
        cout << "12. Filter Tasks by Status\n";
        cout << "13. Undo\n";
        cout << "14. Redo\n";
        cout << "0.  Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            manager.addTask();
            break;

        case 2:
            manager.displayTasks();
            break;

        case 3:
            manager.updateTask();
            break;

        case 4:
            manager.deleteTask();
            break;

        case 5:
            manager.completeTask();
            break;

        case 6:
            manager.shownextpriorityTask();
            break;

        case 7:
            manager.searchTaskbyid();
            break;

        case 8:
            manager.searchbyTitle();
            break;

        case 9:
            manager.sortbypriority();
            break;

        case 10:
            manager.sortbydeadline();
            break;

        case 11:
            manager.filterbyPriority();
            break;

        case 12:
            manager.filterbyStatus();
            break;

        case 13:
            manager.undo();
            break;

        case 14:
            manager.redo();
            break;

        case 0:
            cout << "\nExiting Smart Task Manager...\n";
            break;

        default:
            cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}