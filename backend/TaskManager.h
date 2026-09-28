
#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

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
    bool operator()(const Task &a, const Task &b) const
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

    int findTaskIndex(int id)
    {
        for (int i = 0; i < tasks.size(); i++)
        {
            if (tasks[i].id == id)
                return i;
        }

        return -1;
    }

    void clearRedo()
    {
        while (!redoStack.empty())
            redoStack.pop();
    }

public:

    vector<Task> getTasks()
    {
        return tasks;
    }

    bool addTask(string title, string description, int priority,
                 string deadline, string category)
    {
        if (priority < 1 || priority > 3)
            return false;

        Task task;

        task.id = nextnum++;
        task.title = title;
        task.description = description;
        task.priority = priority;
        task.deadline = deadline;
        task.completed = false;
        task.category = category;

        tasks.push_back(task);

        Action action;
        action.type = ActionType::ADD;
        action.newTask = task;

        undoStack.push(action);
        clearRedo();

        return true;
    }

    bool updateTask(int id, string title, string description,
                    int priority, string deadline, string category)
    {
        int index = findTaskIndex(id);

        if (index == -1 || priority < 1 || priority > 3)
            return false;

        Task oldTask = tasks[index];

        tasks[index].title = title;
        tasks[index].description = description;
        tasks[index].priority = priority;
        tasks[index].deadline = deadline;
        tasks[index].category = category;

        Action action;
        action.type = ActionType::UPDATE;
        action.oldTask = oldTask;
        action.newTask = tasks[index];

        undoStack.push(action);
        clearRedo();

        return true;
    }

    bool deleteTask(int id)
    {
        int index = findTaskIndex(id);

        if (index == -1)
            return false;

        Task deletedTask = tasks[index];

        tasks.erase(tasks.begin() + index);

        Action action;
        action.type = ActionType::DELETE;
        action.oldTask = deletedTask;

        undoStack.push(action);
        clearRedo();

        return true;
    }

    bool completeTask(int id)
    {
        int index = findTaskIndex(id);

        if (index == -1)
            return false;

        Task oldTask = tasks[index];

        tasks[index].completed = !tasks[index].completed;

        Action action;
        action.type = ActionType::COMPLETE;
        action.oldTask = oldTask;
        action.newTask = tasks[index];

        undoStack.push(action);
        clearRedo();

        return true;
    }

    vector<Task> getTasksByPriority()
    {
        priority_queue<Task, vector<Task>, ComparePriority> pq;

        for (const Task &task : tasks)
        {
            if (!task.completed)
                pq.push(task);
        }

        vector<Task> result;

        while (!pq.empty())
        {
            result.push_back(pq.top());
            pq.pop();
        }

        return result;
    }

    bool searchTaskById(int id, Task &result)
    {
        int index = findTaskIndex(id);

        if (index == -1)
            return false;

        result = tasks[index];
        return true;
    }

    vector<Task> searchByTitle(string title)
    {
        vector<Task> result;

        for (const Task &task : tasks)
        {
            if (task.title == title)
                result.push_back(task);
        }

        return result;
    }

    void sortByPriority()
    {
        sort(tasks.begin(), tasks.end(),
             [](const Task &a, const Task &b)
             {
                 return a.priority > b.priority;
             });
    }

    void sortByDeadline()
    {
        sort(tasks.begin(), tasks.end(),
             [](const Task &a, const Task &b)
             {
                 return a.deadline < b.deadline;
             });
    }

    vector<Task> filterByPriority(int priority)
    {
        vector<Task> result;

        for (const Task &task : tasks)
        {
            if (task.priority == priority)
                result.push_back(task);
        }

        return result;
    }

    vector<Task> filterByStatus(bool completed)
    {
        vector<Task> result;

        for (const Task &task : tasks)
        {
            if (task.completed == completed)
                result.push_back(task);
        }

        return result;
    }

    vector<Task> filterTasks(int priority, bool completed)
    {
        vector<Task> result;

        for (const Task &task : tasks)
        {
            if (task.priority == priority &&
                task.completed == completed)
            {
                result.push_back(task);
            }
        }

        return result;
    }

    bool undo()
    {
        if (undoStack.empty())
            return false;

        Action action = undoStack.top();
        undoStack.pop();

        if (action.type == ActionType::ADD)
        {
            int index = findTaskIndex(action.newTask.id);

            if (index != -1)
                tasks.erase(tasks.begin() + index);
        }
        else if (action.type == ActionType::DELETE)
        {
            tasks.push_back(action.oldTask);
        }
        else if (action.type == ActionType::UPDATE ||
                 action.type == ActionType::COMPLETE)
        {
            int index = findTaskIndex(action.oldTask.id);

            if (index != -1)
                tasks[index] = action.oldTask;
        }

        redoStack.push(action);

        return true;
    }

    bool redo()
    {
        if (redoStack.empty())
            return false;

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
                tasks.erase(tasks.begin() + index);
        }
        else if (action.type == ActionType::UPDATE ||
                 action.type == ActionType::COMPLETE)
        {
            int index = findTaskIndex(action.newTask.id);

            if (index != -1)
                tasks[index] = action.newTask;
        }

        undoStack.push(action);

        return true;
    }
};

#endif