# Process Scheduler

**Process Scheduler** is a process scheduling system implemented in C++ that provides a range of scheduling algorithms to manage process execution efficiently.

## Features

- **FCFS (First-Come, First-Served)**
- **SJF (Shortest Job First)**
- **SJF Preemptive (Shortest Job First Preemptive)**
- **HRRN (Highest Response Ratio Next)**
- **Round Robin**
- **Priority Scheduling**

## Tech Stack

- **Programming Language**: C++
- **Libraries/Frameworks**: C++ Standard Library

## Installation

To build and run the project, follow these steps:

1. **Clone the repository**:

   ```bash
   git clone <repository-url>
   cd SchedMaster
   ```

2. **Compile the code**:

   ```bash
   g++ -std=c++11 -o scheduler main.cpp
   ```

3. **Run the executable**:
   ```bash
   ./scheduler
   ```

## Usage

After running the program, you can choose from the following scheduling algorithms. Each algorithm will output a Gantt chart and a process table.

- **FCFS Scheduling**: Executes processes in the order they arrive.
- **SJF Scheduling**: Selects the process with the shortest burst time.
- **SJF Preemptive Scheduling**: Preempts the current process if a new process with a shorter burst time arrives.
- **HRRN Scheduling**: Chooses the process with the highest response ratio.
- **Round Robin Scheduling**: Allocates a fixed time slice to each process in a cyclic order.
- **Priority Scheduling**: Executes processes based on their priority.

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to propose changes or improvements.

## License

This project is licensed under the [MIT License](LICENSE).

## Contact

For questions or feedback, please contact:

- **Email**: [yaaash04@gmail.com](mailto:yaaash04@gmail.com)
