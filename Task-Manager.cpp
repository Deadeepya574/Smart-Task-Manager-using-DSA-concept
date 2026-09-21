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
    priority_queue<Task,vector<Task>,ComparePriority> priorityTasks;
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
        priorityTasks.push(task);

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

    void deletetask(){
        int id;
        cout << "Enter id to delete";
        for(int i = 0;i<tasks.size();i++){
            if(tasks[i].id = id){
                tasks.erase(tasks.begin()+i);
                return ;
            } 
        } 
    cout << "Task Not Found"; 
    }

    void completeTasks(){
        for(Task& task : tasks){
            if(task.id == id){
                task.completed = !task.completed;
                if(task.completed){
                    cout << "Task Completed";
                }
                else{
                    cout << "Task Pending";
                }
                return;
            }
        }
    }


    void shownextpriorityTask(){
        if(priorityTasks.empty()){
            cout << "No tasks";
            return;
        }
        

    }
};


struct ComparePriority{
    bool operator()(const Task& a, const Task& b){
        return a.priority < b.priority;
    } 
}



int main()
{

    TaskManager manager;
    manager.addTask();
    manager.addTask();
    manager.displayTasks();
    manager.updateTasks();
    manager.deletetask();
    manager.displayTasks(); 
    manager.completeTask();

    return 0;
}