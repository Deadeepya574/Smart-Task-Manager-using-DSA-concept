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