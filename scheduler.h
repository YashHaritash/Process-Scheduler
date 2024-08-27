#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>

struct Process
{
    int id;
    int arrivalTime;
    int burstTime;
    int priority; // Used for Priority Scheduling
};

// Scheduling functions
void FCFS(std::vector<Process> &processes);
void SJF(std::vector<Process> &processes);
void SJFPreemptive(std::vector<Process> &processes);
void HRRN(std::vector<Process> &processes);
void RoundRobin(std::vector<Process> &processes, int quantum);
void PriorityScheduling(std::vector<Process> &processes);

#endif // SCHEDULER_H
