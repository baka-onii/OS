#include <iostream>
#include <vector>
#include <algorithm>

//#define MAX_PROCESSES 100

class Process   // class to make processes into seperate objects
{
public:
    int ProcessID, ArrivalTime, BurstTime;

    Process(int id, int at, int bt = 0)
        : ProcessID(id), ArrivalTime(at), BurstTime(bt) {}

    bool operator<(const Process& other) const {
        return ArrivalTime < other.ArrivalTime;
    }

    bool operator==(const Process& other) const {
        return ProcessID == other.ProcessID;
    }

    Process& operator--() {
        --BurstTime;
        return *this; 
    }

    Process operator--(int) {
        Process temp = *this; // because int++ increments after it is used
        --BurstTime;
        return temp; // Return the copy (before decrement)
    }
};

void Execute(Process& a)
{
    a.BurstTime--;
}

int main() {
    int processTime = 0;
    int numberProcesses = 0;

    std::vector<Process> processes;

    std::cout << "Enter number of Processes: " << std::endl;
    std::cin >> numberProcesses;

    for (int process = 1; process <= numberProcesses; process++) {
        int id, at, bt;     // enter processes and make seperate objects

        std::cout << "Enter Arrival Time for Process " << process << std::endl;
        std::cin >> at;

        std::cout << "Enter Burst Time for Process " << process << std::endl;
        std::cin >> bt;

        processes.emplace_back(process, at, bt);
    }

    std::sort(processes.begin(), processes.end());

    std::cout << std::endl << "Processes sorted by Arrival Time:" << std::endl;
    //std::cout << "pID  " << "AT  " << "BT" << std::endl;
    for (const auto& p : processes) {   // print processes
        std::cout << "ProcessID: " << p.ProcessID
            << ", ArrivalTime: " << p.ArrivalTime
            << ", BurstTime: " << p.BurstTime << std::endl;
    }

    std::cout << "\nExecuting Processes by Non-Preemptive SJF: " << std::endl;

    {
        int tracker = 0;
        while (!processes.empty())  // vector is empty / processes are finished
        {
            std::vector<Process> readyProcesses;
            for (int i = 0; i < processes.size(); i++) {    // traverse vector to find processes ready to execute

                if (processes[i].ArrivalTime <= processTime)
                    readyProcesses.emplace_back(processes[i]);
            }

            if (readyProcesses.empty())
            {
                processTime++;
                continue;
            }

            if (!readyProcesses.empty()) {   // track the processes
                auto minBurstTime = std::min_element(readyProcesses.begin(), readyProcesses.end(), [](const Process& a, const Process& b) {
                    return a.BurstTime < b.BurstTime;
                    });

                if (minBurstTime != readyProcesses.end()) {
                    Process priorityElement = *minBurstTime;
                    auto it = std::find(processes.begin(), processes.end(), priorityElement);
                    std::cout << "\nExecuting Process " << it->ProcessID << std::endl;
                    Execute(*it);    // Execute the process for 1 cycle (-1 BT)

                    if (it->BurstTime <= 0)
                    {
                        std::cout << "\nProcess " << it->ProcessID << " Completed\n";
                        processes.erase(it);   // Process Completed
                    }
                }

                processTime++;
                
                for (const auto& p : processes) {   // print processes
                    std::cout << "ProcessID: " << p.ProcessID
                        << ", ArrivalTime: " << p.ArrivalTime
                        << ", BurstTime: " << p.BurstTime << std::endl;
                }
                std::cout << "\nCurrent CPU Time: " << processTime << "\n" << std::endl;
            }
            
        }
        if (processes.empty())
        {
            std::cout << "Operation Completed!" << std::endl;
        }
    }

    return 0;
}
