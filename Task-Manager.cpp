#include<bits/stdc++.h>
using namespace std;

struct Task{
    int id;
    string title;
    string description;
    int priority;
    string deadline;
    bool completed;
    string category;
    bool recurring;
};

class TaskManager{
    private:
     vector<Task> tasks;
     int nextnum =1 ;
    public:
      void addTask(){
        Task task;

        task.id = nextnum++;
        
        cout << "Enter Task Title";
        getline(cin >> ws, task.title);

        cout << "Enter description: ";
        getline(cin , task.description);

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
        
      }

      void displaytasks(){
        

      }
};



int main(){
     

    TaskManager manager;
    manager.addTask();
    return 0;
}