#include "scheduler.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <climits>

// Utility function to print Gantt Chart
void printGanttChart(const std::vector<Process> &processes, const std::vector<int> &finishTimes)
{
    int n = processes.size();
    std::cout << "Gantt Chart:\n";
    std::cout << std::setw(4) << "Time";
    for (int i = 0; i < n; ++i)
    {
        std::cout << std::setw(10) << processes[i].id;
    }
    std::cout << '\n';

    int currentTime = 0;
    for (int i = 0; i < n; ++i)
    {
        std::cout << std::setw(4) << currentTime;
        currentTime += processes[i].burstTime;
        std::cout << std::setw(10) << "|";
    }
    std::cout << std::setw(4) << currentTime << '\n';
}

// FCFS Scheduling
void FCFS(std::vector<Process> &processes)
{
    int n = processes.size();
    std::vector<int> waitTime(n, 0);
    std::vector<int> turnAroundTime(n);
    std::vector<int> finishTimes(n);

    int totalWaitTime = 0, totalTurnAroundTime = 0;
    int time = 0;

    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            waitTime[i] = time - processes[i].arrivalTime;
        }
        time += processes[i].burstTime;
        finishTimes[i] = time;
        turnAroundTime[i] = finishTimes[i] - processes[i].arrivalTime;
        totalWaitTime += waitTime[i];
        totalTurnAroundTime += turnAroundTime[i];
    }

    std::cout << "FCFS Scheduling:\n";
    printGanttChart(processes, finishTimes);
    for (int i = 0; i < n; ++i)
    {
        std::cout << "Process " << processes[i].id
                  << " - Wait Time: " << waitTime[i]
                  << ", Turnaround Time: " << turnAroundTime[i] << '\n';
    }
    std::cout << "Average Wait Time: " << (float)totalWaitTime / n << '\n';
    std::cout << "Average Turnaround Time: " << (float)totalTurnAroundTime / n << '\n';
}

// SJF Scheduling
bool compareBurstTime(Process a, Process b)
{
    return a.burstTime < b.burstTime;
}

void SJF(std::vector<Process> &processes)
{
    int n = processes.size();
    std::sort(processes.begin(), processes.end(), compareBurstTime);

    std::vector<int> waitTime(n, 0);
    std::vector<int> turnAroundTime(n);
    std::vector<int> finishTimes(n);

    int totalWaitTime = 0, totalTurnAroundTime = 0;
    int time = 0;

    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            waitTime[i] = time - processes[i].arrivalTime;
        }
        time += processes[i].burstTime;
        finishTimes[i] = time;
        turnAroundTime[i] = finishTimes[i] - processes[i].arrivalTime;
        totalWaitTime += waitTime[i];
        totalTurnAroundTime += turnAroundTime[i];
    }

    std::cout << "SJF Scheduling:\n";
    printGanttChart(processes, finishTimes);
    for (int i = 0; i < n; ++i)
    {
        std::cout << "Process " << processes[i].id
                  << " - Wait Time: " << waitTime[i]
                  << ", Turnaround Time: " << turnAroundTime[i] << '\n';
    }
    std::cout << "Average Wait Time: " << (float)totalWaitTime / n << '\n';
    std::cout << "Average Turnaround Time: " << (float)totalTurnAroundTime / n << '\n';
}

// SJF Preemptive Scheduling
void SJFPreemptive(std::vector<Process> &processes)
{
    int n = processes.size();
    std::vector<int> remainingBurstTime(n);
    std::vector<int> finishTimes(n);
    std::vector<int> waitTime(n, 0);
    std::vector<int> turnAroundTime(n);

    for (int i = 0; i < n; ++i)
    {
        remainingBurstTime[i] = processes[i].burstTime;
    }

    int time = 0;
    int completed = 0;
    while (completed < n)
    {
        int minIndex = -1;
        int minBurstTime = INT_MAX;
        for (int i = 0; i < n; ++i)
        {
            if (processes[i].arrivalTime <= time && remainingBurstTime[i] > 0 && remainingBurstTime[i] < minBurstTime)
            {
                minBurstTime = remainingBurstTime[i];
                minIndex = i;
            }
        }
        if (minIndex != -1)
        {
            remainingBurstTime[minIndex]--;
            if (remainingBurstTime[minIndex] == 0)
            {
                finishTimes[minIndex] = time + 1;
                turnAroundTime[minIndex] = finishTimes[minIndex] - processes[minIndex].arrivalTime;
                waitTime[minIndex] = turnAroundTime[minIndex] - processes[minIndex].burstTime;
                completed++;
            }
        }
        time++;
    }

    int totalWaitTime = 0, totalTurnAroundTime = 0;
    std::cout << "SJF Preemptive Scheduling:\n";
    printGanttChart(processes, finishTimes);
    for (int i = 0; i < n; ++i)
    {
        totalWaitTime += waitTime[i];
        totalTurnAroundTime += turnAroundTime[i];
        std::cout << "Process " << processes[i].id
                  << " - Wait Time: " << waitTime[i]
                  << ", Turnaround Time: " << turnAroundTime[i] << '\n';
    }
    std::cout << "Average Wait Time: " << (float)totalWaitTime / n << '\n';
    std::cout << "Average Turnaround Time: " << (float)totalTurnAroundTime / n << '\n';
}

// HRRN Scheduling
void HRRN(std::vector<Process> &processes)
{
    int n = processes.size();
    std::vector<int> waitTime(n, 0);
    std::vector<int> turnAroundTime(n);

    int totalWaitTime = 0, totalTurnAroundTime = 0;
    int time = 0, completed = 0;
    while (completed < n)
    {
        int maxRatioIndex = -1;
        float maxRatio = -1.0;

        for (int i = 0; i < n; ++i)
        {
            if (processes[i].arrivalTime <= time && waitTime[i] == 0)
            {
                float ratio = (time - processes[i].arrivalTime + processes[i].burstTime) / (float)processes[i].burstTime;
                if (ratio > maxRatio)
                {
                    maxRatio = ratio;
                    maxRatioIndex = i;
                }
            }
        }

        if (maxRatioIndex != -1)
        {
            waitTime[maxRatioIndex] = time - processes[maxRatioIndex].arrivalTime;
            time += processes[maxRatioIndex].burstTime;
            turnAroundTime[maxRatioIndex] = time - processes[maxRatioIndex].arrivalTime;
            totalWaitTime += waitTime[maxRatioIndex];
            totalTurnAroundTime += turnAroundTime[maxRatioIndex];
            completed++;
        }
        else
        {
            time++;
        }
    }

    std::cout << "HRRN Scheduling:\n";
    // Approximate Gantt Chart, not exact for HRRN
    printGanttChart(processes, turnAroundTime);
    for (int i = 0; i < n; ++i)
    {
        std::cout << "Process " << processes[i].id
                  << " - Wait Time: " << waitTime[i]
                  << ", Turnaround Time: " << turnAroundTime[i] << '\n';
    }
    std::cout << "Average Wait Time: " << (float)totalWaitTime / n << '\n';
    std::cout << "Average Turnaround Time: " << (float)totalTurnAroundTime / n << '\n';
}

// Round Robin Scheduling
void RoundRobin(std::vector<Process> &processes, int quantum)
{
    int n = processes.size();
    std::vector<int> remainingBurstTime(n);
    std::vector<int> waitTime(n, 0);
    std::vector<int> turnAroundTime(n);

    for (int i = 0; i < n; ++i)
    {
        remainingBurstTime[i] = processes[i].burstTime;
    }

    int time = 0;
    int completed = 0;
    while (completed < n)
    {
        for (int i = 0; i < n; ++i)
        {
            if (processes[i].arrivalTime <= time && remainingBurstTime[i] > 0)
            {
                int execTime = std::min(quantum, remainingBurstTime[i]);
                remainingBurstTime[i] -= execTime;
                time += execTime;

                if (remainingBurstTime[i] == 0)
                {
                    turnAroundTime[i] = time - processes[i].arrivalTime;
                    waitTime[i] = turnAroundTime[i] - processes[i].burstTime;
                    completed++;
                }
            }
        }
    }

    int totalWaitTime = 0, totalTurnAroundTime = 0;
    std::cout << "Round Robin Scheduling:\n";
    // Approximate Gantt Chart, not exact for Round Robin
    printGanttChart(processes, turnAroundTime);
    for (int i = 0; i < n; ++i)
    {
        totalWaitTime += waitTime[i];
        totalTurnAroundTime += turnAroundTime[i];
        std::cout << "Process " << processes[i].id
                  << " - Wait Time: " << waitTime[i]
                  << ", Turnaround Time: " << turnAroundTime[i] << '\n';
    }
    std::cout << "Average Wait Time: " << (float)totalWaitTime / n << '\n';
    std::cout << "Average Turnaround Time: " << (float)totalTurnAroundTime / n << '\n';
}

// Priority Scheduling
bool comparePriority(Process a, Process b)
{
    return a.priority > b.priority; // Higher priority comes first
}

void PriorityScheduling(std::vector<Process> &processes)
{
    int n = processes.size();
    std::sort(processes.begin(), processes.end(), comparePriority);

    std::vector<int> waitTime(n, 0);
    std::vector<int> turnAroundTime(n);
    std::vector<int> finishTimes(n);

    int totalWaitTime = 0, totalTurnAroundTime = 0;
    int time = 0;

    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            waitTime[i] = time - processes[i].arrivalTime;
        }
        time += processes[i].burstTime;
        finishTimes[i] = time;
        turnAroundTime[i] = finishTimes[i] - processes[i].arrivalTime;
        totalWaitTime += waitTime[i];
        totalTurnAroundTime += turnAroundTime[i];
    }

    std::cout << "Priority Scheduling:\n";
    printGanttChart(processes, finishTimes);
    for (int i = 0; i < n; ++i)
    {
        std::cout << "Process " << processes[i].id
                  << " - Wait Time: " << waitTime[i]
                  << ", Turnaround Time: " << turnAroundTime[i] << '\n';
    }
    std::cout << "Average Wait Time: " << (float)totalWaitTime / n << '\n';
    std::cout << "Average Turnaround Time: " << (float)totalTurnAroundTime / n << '\n';
}
