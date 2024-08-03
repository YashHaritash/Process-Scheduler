#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <queue>
#include <string>

class Process
{
public:
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int remaining_time;
    int priority; // Add this line

    Process(int i, int a, int b, int p = 0) // Update constructor
        : id(i), arrival_time(a), burst_time(b), waiting_time(0), turnaround_time(0), completion_time(0), remaining_time(b), priority(p)
    {
    }
};

struct Idle
{
    int start_time;
    int end_time;
};

void print_gantt_chart(const std::vector<Process> &processes, const std::vector<Idle> &idle_periods);
void fcfs_scheduling(std::vector<Process> &processes);
void sjf_scheduling(std::vector<Process> &processes);
void sjf_preemptive_scheduling(std::vector<Process> &processes);
void hrrn_scheduling(std::vector<Process> &processes);
void round_robin_scheduling(std::vector<Process> &processes, int quantum);
void priority_scheduling(std::vector<Process> &processes); // Add this line

#endif // SCHEDULER_H
