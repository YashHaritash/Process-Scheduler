#include "scheduler.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<Process> processes;
    int n, id, arrival, burst, quantum, priority;

    cout << "Enter the number of processes: ";
    cin >> n;

    // Collect process data
    for (int i = 0; i < n; ++i)
    {
        cout << "Enter arrival time and burst time for process " << i << ": ";
        cin >> arrival >> burst;

        // Default priority to 0
        priority = 0;

        processes.emplace_back(i, arrival, burst, priority);
    }

    cout << "Choose scheduling algorithm:\n1. FCFS\n2. SJF\n3. SJF Preemptive\n4. HRRN\n5. Round Robin\n6. Priority\n";
    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        fcfs_scheduling(processes);
        break;
    case 2:
        sjf_scheduling(processes);
        break;
    case 3:
        sjf_preemptive_scheduling(processes);
        break;
    case 4:
        hrrn_scheduling(processes);
        break;
    case 5:
        cout << "Enter time quantum for Round Robin: ";
        cin >> quantum;
        round_robin_scheduling(processes, quantum);
        break;
    case 6:
        // Ask for priority values only if Priority Scheduling is chosen
        for (int i = 0; i < n; ++i)
        {
            cout << "Enter priority for process " << i << ": ";
            cin >> priority;
            processes[i].priority = priority;
        }
        priority_scheduling(processes);
        break;
    default:
        cout << "Invalid choice" << endl;
    }

    return 0;
}
