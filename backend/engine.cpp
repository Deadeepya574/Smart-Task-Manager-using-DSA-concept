
#include <bits/stdc++.h>
#include "TaskManager.h"
using namespace std;

vector<string> split(string s, char delimiter)
{
    vector<string> result;
    string item;
    stringstream ss(s);

    while (getline(ss, item, delimiter))
        result.push_back(item);

    if (!s.empty() && s.back() == delimiter)
        result.push_back("");

    return result;
}

void printTask(const Task &task)
{
    cout << "TASK|"
         << task.id << "|"
         << task.title << "|"
         << task.description << "|"
         << task.priority << "|"
         << task.deadline << "|"
         << (task.completed ? 1 : 0) << "|"
         << task.category << '\n';
}

int main()
{
    TaskManager manager;
    string line;

    while (getline(cin, line))
    {
        vector<string> data = split(line, '|');

        if (data.empty())
            continue;

        string command = data[0];

        try
        {
            if (command == "LIST")
            {
                vector<Task> tasks = manager.getTasks();

                for (const Task &task : tasks)
                    printTask(task);

                cout << "END" << endl;
            }

            else if (command == "ADD" && data.size() == 6)
            {
                bool success = manager.addTask(
                    data[1],
                    data[2],
                    stoi(data[3]),
                    data[4],
                    data[5]
                );

                cout << (success ? "OK|Task added" : "ERROR|Invalid task") << endl;
            }

            else if (command == "UPDATE" && data.size() == 7)
            {
                bool success = manager.updateTask(
                    stoi(data[1]),
                    data[2],
                    data[3],
                    stoi(data[4]),
                    data[5],
                    data[6]
                );

                cout << (success ? "OK|Task updated" : "ERROR|Update failed") << endl;
            }

            else if (command == "DELETE" && data.size() == 2)
            {
                bool success = manager.deleteTask(stoi(data[1]));

                cout << (success ? "OK|Task deleted" : "ERROR|Task not found") << endl;
            }

            else if (command == "COMPLETE" && data.size() == 2)
            {
                bool success = manager.completeTask(stoi(data[1]));

                cout << (success ? "OK|Status changed" : "ERROR|Task not found") << endl;
            }

            else if (command == "UNDO")
            {
                cout << (manager.undo() ? "OK|Undo successful" : "ERROR|Nothing to undo") << endl;
            }

            else if (command == "REDO")
            {
                cout << (manager.redo() ? "OK|Redo successful" : "ERROR|Nothing to redo") << endl;
            }

            else if (command == "QUIT")
            {
                cout << "OK|Exiting" << endl;
                break;
            }

            else
            {
                cout << "ERROR|Invalid command" << endl;
            }
        }
        catch (...)
        {
            cout << "ERROR|Invalid input" << endl;
        }
    }

    return 0;
}