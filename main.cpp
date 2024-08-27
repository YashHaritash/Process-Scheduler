#include "scheduler.h"
#include <iostream>

int main()
{
    int n;

    std::cout << "Enter the number of processes: ";
    std::cin >> n;

    std::vector<Process> processes(n);

    std::cout << "Enter process details (ID ArrivalTime BurstTime Priority):\n";
    for (int i = 0; i < n; ++i)
    {
        std::cout << "Process " << i + 1 << ": ";
        std::cin >> processes[i].id >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
    }

    int quantum;
    std::cout << "Enter the time quantum for Round Robin scheduling: ";
    std::cin >> quantum;

    // Run FCFS
    std::vector<Process> fcfsProcesses = processes;
    FCFS(fcfsProcesses);

    // Run SJF
    std::vector<Process> sjfProcesses = processes;
    SJF(sjfProcesses);

    // Run SJF Preemptive
    std::vector<Process> sjfPreemptiveProcesses = processes;
    SJFPreemptive(sjfPreemptiveProcesses);

    // Run HRRN
    std::vector<Process> hrrnProcesses = processes;
    HRRN(hrrnProcesses);

    // Run Round Robin
    std::vector<Process> rrProcesses = processes;
    RoundRobin(rrProcesses, quantum);

    // Run Priority Scheduling
    std::vector<Process> priorityProcesses = processes;
    PriorityScheduling(priorityProcesses);

    return 0;
}
