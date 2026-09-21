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
    bool recurring;
};

class TaskManager
{
private:
    vector<Task> tasks;
    int nextnum = 1;

public:
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

        char choice;
        cout << "Is this a recurring task? (y/n): ";
        cin >> choice;

        task.recurring = (choice == 'y' || choice == 'Y');

        task.completed = false;

        tasks.push_back(task);

        cout << "Task added successfully!! ";
        return;
    }

    void displayTasks()
    {
        if (tasks.empty())
        {
            cout << "No tasks available";
            return;
        }

        for (Task &task : tasks)
        {
            cout << "ID" << task.id;
            cout << "Title" << task.title;
            cout << "Description" << task.description;
        }
        cout << "endl";

        return;
    }

    void updateTasks()

    {

        int id;
        cout << "\nEnter Task ID to update: ";
        cin >> id;

        for (Task &task : tasks)
        {

            if (task.id == id)
            {
                cout << "Enter new title: ";
                getline(cin >> ws, task.title);

                cout << "Enter new description: ";
                getline(cin, task.description);

                cout << "Enter new priority (3, 2, 1): ";
                cin >> task.priority;

                cout << "Enter new deadline (YYYY-MM-DD): ";
                cin >> task.deadline;

                cout << "Enter new category: ";
                getline(cin >> ws, task.category);

                cout << "\nTask updated successfully!\n";
                return;
            }
        }
    }

    void deletetask()
    {
        int id;
        cout << "Enter id to delete";
        for (int i = 0; i < tasks.size(); i++)
        {
            if (tasks[i].id = id)
            {
                tasks.erase(tasks.begin() + i);
                return;
            }
        }
        cout << "Task Not Found";
    }

    void completeTasks()
    {
        for (Task &task : tasks)
        {
            if (task.id == id)
            {
                task.completed = !task.completed;
                if (task.completed)
                {
                    cout << "Task Completed";
                }
                else
                {
                    cout << "Task Pending";
                }
                return;
            }
        }
    }

    void shownextpriorityTask()
    {
        if (tasks.empty())
        {
            cout << "No tasks";
            return;
        }
        priority_queue<Task, vector<Task>, ComparePriority> pq;

        for(const Task& task : tasks){
            if(!task.completed){
                pq.push(task);
            }
        }
        if(pq.empty()){
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

    void searchTaskbyid(){
        int id;
        cout << "Enter id to search";
        cin >> id;

        for(const Task& task : tasks){
            if(task.id == id ){

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

            cout << "Recurring   : "
                 << (task.recurring ? "Yes" : "No")
                 << endl;

                 return;
            }
        }
        cout << "Task Not Found";
    }


    void searchbyTitle(){
        
    }
};

struct ComparePriority
{
    bool operator()(const Task &a, const Task &b)
    {
        return a.priority < b.priority;
    }
} 
main()
{

    TaskManager manager;
    manager.addTask();
    manager.addTask();
    manager.displayTasks();
    manager.updateTasks();
    manager.deletetask();
    manager.displayTasks();
    manager.completeTasks();
    manager.shownextpriorityTask();

    return 0;
}