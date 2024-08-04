#include "scheduler.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>

using namespace std;

void print_gantt_chart(const vector<Process> &processes, const vector<Idle> &idle_periods)
{
    cout << "Gantt Chart:\n|";
    int last_time = 0;

    for (const auto &idle : idle_periods)
    {
        cout << string(idle.end_time - idle.start_time, ' ') << "Idle" << string(idle.end_time - idle.start_time, ' ') << "|";
        last_time = idle.end_time;
    }

    for (const auto &process : processes)
    {
        cout << string(process.completion_time - last_time, ' ') << "P" << process.id << string(process.completion_time - last_time, ' ') << "|";
        last_time = process.completion_time;
    }
    cout << '\n';
}

void print_process_table(const vector<Process> &processes, const string &algorithm_name)
{
    cout << algorithm_name << " Scheduling:\n"
         << "ID\tArrival\tBurst\tWaiting\tTurnaround\tCompletion\n";
    for (const auto &p : processes)
    {
        cout << p.id << '\t' << p.arrival_time << '\t' << p.burst_time << '\t'
             << p.waiting_time << '\t' << p.turnaround_time << "\t\t" << p.completion_time << '\n';
    }
}

void fcfs_scheduling(vector<Process> &processes)
{
    int current_time = 0;
    for (auto &p : processes)
    {
        p.waiting_time = max(0, current_time - p.arrival_time);
        p.turnaround_time = p.waiting_time + p.burst_time;
        p.completion_time = current_time + p.burst_time;
        current_time = p.completion_time;
    }

    print_process_table(processes, "FCFS");
    print_gantt_chart(processes, {});
}

void sjf_scheduling(vector<Process> &processes)
{
    int current_time = 0;
    vector<Process> gantt_chart;
    vector<Idle> idle_periods;
    vector<bool> finished(processes.size(), false);
    int completed = 0;

    while (completed < processes.size())
    {
        vector<Process *> ready_queue;
        for (auto &p : processes)
        {
            if (p.arrival_time <= current_time && !finished[p.id])
            {
                ready_queue.push_back(&p);
            }
        }

        if (ready_queue.empty())
        {
            if (!idle_periods.empty() && idle_periods.back().end_time == current_time)
            {
                idle_periods.back().end_time++;
            }
            else
            {
                idle_periods.push_back(Idle{current_time, current_time + 1});
            }
            current_time++;
            continue;
        }

        if (!idle_periods.empty() && idle_periods.back().end_time < current_time)
        {
            idle_periods.back().end_time = current_time;
        }

        auto shortest_job = min_element(ready_queue.begin(), ready_queue.end(),
                                        [](const Process *a, const Process *b)
                                        { return a->burst_time < b->burst_time; });

        Process *current_process = *shortest_job;
        current_process->waiting_time = max(0, current_time - current_process->arrival_time);
        current_process->turnaround_time = current_process->waiting_time + current_process->burst_time;
        current_process->completion_time = current_time + current_process->burst_time;
        current_time = current_process->completion_time;
        finished[current_process->id] = true;
        completed++;

        gantt_chart.push_back(*current_process);
    }

    print_process_table(processes, "SJF");
    print_gantt_chart(gantt_chart, idle_periods);
}

void sjf_preemptive_scheduling(vector<Process> &processes)
{
    int current_time = 0;
    int completed = 0;
    vector<Process> gantt_chart;
    vector<Idle> idle_periods;
    vector<int> remaining_time(processes.size());
    vector<bool> finished(processes.size(), false);

    for (size_t i = 0; i < processes.size(); ++i)
    {
        remaining_time[i] = processes[i].burst_time;
    }

    while (completed < processes.size())
    {
        int shortest = -1;
        int min_burst = numeric_limits<int>::max();

        for (size_t i = 0; i < processes.size(); ++i)
        {
            if (processes[i].arrival_time <= current_time && remaining_time[i] > 0)
            {
                if (remaining_time[i] < min_burst)
                {
                    min_burst = remaining_time[i];
                    shortest = i;
                }
            }
        }

        if (shortest == -1)
        {
            if (!idle_periods.empty() && idle_periods.back().end_time == current_time)
            {
                idle_periods.back().end_time++;
            }
            else
            {
                idle_periods.push_back(Idle{current_time, current_time + 1});
            }
            current_time++;
            continue;
        }

        if (!idle_periods.empty() && idle_periods.back().end_time < current_time)
        {
            idle_periods.back().end_time = current_time;
        }

        remaining_time[shortest]--;
        gantt_chart.push_back(processes[shortest]);
        current_time++;

        if (remaining_time[shortest] == 0)
        {
            completed++;
            processes[shortest].completion_time = current_time;
            processes[shortest].turnaround_time = processes[shortest].completion_time - processes[shortest].arrival_time;
            processes[shortest].waiting_time = processes[shortest].turnaround_time - processes[shortest].burst_time;
        }
    }

    print_process_table(processes, "SJF Preemptive");
    print_gantt_chart(gantt_chart, idle_periods);
}

void hrrn_scheduling(vector<Process> &processes)
{
    int current_time = 0;
    int completed = 0;
    vector<Process> gantt_chart;
    vector<Idle> idle_periods;
    vector<bool> finished(processes.size(), false);

    while (completed < processes.size())
    {
        vector<Process *> ready_queue;
        for (auto &p : processes)
        {
            if (p.arrival_time <= current_time && !finished[p.id])
            {
                ready_queue.push_back(&p);
            }
        }

        if (ready_queue.empty())
        {
            if (!idle_periods.empty() && idle_periods.back().end_time == current_time)
            {
                idle_periods.back().end_time++;
            }
            else
            {
                idle_periods.push_back(Idle{current_time, current_time + 1});
            }
            current_time++;
            continue;
        }

        if (!idle_periods.empty() && idle_periods.back().end_time < current_time)
        {
            idle_periods.back().end_time = current_time;
        }

        auto highest_response_ratio = max_element(ready_queue.begin(), ready_queue.end(),
                                                  [current_time](const Process *a, const Process *b)
                                                  {
                                                      double ratio_a = (current_time - a->arrival_time + a->burst_time) / static_cast<double>(a->burst_time);
                                                      double ratio_b = (current_time - b->arrival_time + b->burst_time) / static_cast<double>(b->burst_time);
                                                      return ratio_a < ratio_b;
                                                  });

        Process *current_process = *highest_response_ratio;
        current_process->waiting_time = max(0, current_time - current_process->arrival_time);
        current_process->turnaround_time = current_process->waiting_time + current_process->burst_time;
        current_process->completion_time = current_time + current_process->burst_time;
        current_time = current_process->completion_time;
        completed++;
        finished[current_process->id] = true;

        gantt_chart.push_back(*current_process);
    }

    print_process_table(processes, "HRRN");
    print_gantt_chart(gantt_chart, idle_periods);
}

void round_robin_scheduling(vector<Process> &processes, int quantum)
{
    if (quantum <= 0)
    {
        throw invalid_argument("Quantum must be a positive integer");
    }

    int current_time = 0;
    int completed = 0;
    vector<Process> gantt_chart;
    vector<Idle> idle_periods;
    vector<int> remaining_time(processes.size());
    queue<int> ready_queue;
    vector<bool> finished(processes.size(), false);
    vector<bool> in_queue(processes.size(), false);

    for (size_t i = 0; i < processes.size(); ++i)
    {
        remaining_time[i] = processes[i].burst_time;
    }

    while (completed < processes.size())
    {
        // Add new processes to the ready queue at each time quantum
        for (size_t i = 0; i < processes.size(); ++i)
        {
            if (processes[i].arrival_time <= current_time && !finished[i] && !in_queue[i])
            {
                ready_queue.push(i);
                in_queue[i] = true;
            }
        }

        if (ready_queue.empty())
        {
            // Handle idle periods
            if (idle_periods.empty() || idle_periods.back().end_time != current_time)
            {
                idle_periods.push_back(Idle{current_time, current_time});
            }
            else
            {
                idle_periods.back().end_time++;
            }
            current_time++;
            continue; // Move to the next iteration
        }

        if (!idle_periods.empty() && idle_periods.back().end_time < current_time)
        {
            idle_periods.back().end_time = current_time;
        }

        int current_process = ready_queue.front();
        ready_queue.pop();

        int time_slice = min(quantum, remaining_time[current_process]);
        remaining_time[current_process] -= time_slice;
        gantt_chart.push_back(processes[current_process]);

        current_time += time_slice;

        // Add newly arrived processes to the ready queue after processing
        for (size_t i = 0; i < processes.size(); ++i)
        {
            if (processes[i].arrival_time <= current_time && !finished[i] && !in_queue[i])
            {
                ready_queue.push(i);
                in_queue[i] = true;
            }
        }

        if (remaining_time[current_process] == 0)
        {
            completed++;
            processes[current_process].completion_time = current_time;
            processes[current_process].turnaround_time = processes[current_process].completion_time - processes[current_process].arrival_time;
            processes[current_process].waiting_time = processes[current_process].turnaround_time - processes[current_process].burst_time;
            finished[current_process] = true;
            in_queue[current_process] = false; // Remove from queue
        }
        else
        {
            ready_queue.push(current_process); // Re-add to queue if not finished
        }
    }

    // Close any open idle periods at the end
    if (!idle_periods.empty() && idle_periods.back().end_time < current_time)
    {
        idle_periods.back().end_time = current_time;
    }

    print_process_table(processes, "Round Robin");
    print_gantt_chart(gantt_chart, idle_periods);
}

void priority_scheduling(vector<Process> &processes)
{
    int current_time = 0;
    int completed = 0;
    vector<Process> gantt_chart;
    vector<Idle> idle_periods;
    vector<bool> finished(processes.size(), false);

    while (completed < processes.size())
    {
        vector<Process *> ready_queue;
        for (auto &p : processes)
        {
            if (p.arrival_time <= current_time && !finished[p.id])
            {
                ready_queue.push_back(&p);
            }
        }

        if (ready_queue.empty())
        {
            if (!idle_periods.empty() && idle_periods.back().end_time == current_time)
            {
                idle_periods.back().end_time++;
            }
            else
            {
                idle_periods.push_back(Idle{current_time, current_time + 1});
            }
            current_time++;
            continue;
        }

        if (!idle_periods.empty() && idle_periods.back().end_time < current_time)
        {
            idle_periods.back().end_time = current_time;
        }

        auto highest_priority = max_element(ready_queue.begin(), ready_queue.end(),
                                            [](const Process *a, const Process *b)
                                            { return a->priority < b->priority; });

        Process *current_process = *highest_priority;
        current_process->waiting_time = max(0, current_time - current_process->arrival_time);
        current_process->turnaround_time = current_process->waiting_time + current_process->burst_time;
        current_process->completion_time = current_time + current_process->burst_time;
        current_time = current_process->completion_time;
        completed++;
        finished[current_process->id] = true;

        gantt_chart.push_back(*current_process);
    }

    print_process_table(processes, "Priority");
    print_gantt_chart(gantt_chart, idle_periods);
}