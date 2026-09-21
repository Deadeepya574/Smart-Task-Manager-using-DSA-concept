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
      void addtask(){
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
        getline(cin >> ws, )



      }
}

int main(){
    Task task1;
    task1.id = 1;
    task1.title = "Learn c++ in 2 days";
    task1.description = "learning c++";
    task1.priority = 1;
    task1.deadline = 2days;
    task1.category = "study";

    return 0;
}