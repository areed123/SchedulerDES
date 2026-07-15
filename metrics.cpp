#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include "metrics.h"

void metrics::recordArrival(int pid, double arrivalTime) {
    ProcessRecord rec;
    rec.pid = pid;
    rec.arrivalTime = arrivalTime;
    rec.departureTime = 0.0;
    rec.serviceTime = 0.0;
    processRecords.push_back(rec);
}

void metrics::recordDeparture(int pid, double departureTime) {
    for (auto& rec : processRecords) {
        if (rec.pid == pid) {
            rec.departureTime = departureTime;
            break;
        }
    }
}

void metrics::recordServiceTime(int pid, double serviceTime) {
    for (auto& rec : processRecords) {
        if (rec.pid == pid) {
            rec.serviceTime = serviceTime;
            break;
        }
    }
}

void metrics::recordCpuBusy(double duration) {
    totalBusyTime += duration;
}

void metrics::setTotalSimulationTime(double time) {
    totalSimulationTime = time;
}

void metrics::setTotalProcesses(int count) {
    totalProcesses = count;
}

double metrics::calculateAverageTurnaroundTime() const {
    // Only include processes with valid departure times
    double totalTurnaroundTime = 0.0;
    int validProcesses = 0;
    
    for (const auto& rec : processRecords) {
        if (rec.departureTime > 0.0) {  // Only count completed processes
            double turnaroundTime = rec.departureTime - rec.arrivalTime;
            totalTurnaroundTime += turnaroundTime;
            validProcesses++;
        }
    }
    
    if (validProcesses == 0) return 0.0;
    return totalTurnaroundTime / validProcesses;
}

double metrics::calculateThroughput() const {
    if (totalSimulationTime == 0.0) return 0.0;
    
    // Count only completed processes
    int completedProcesses = 0;
    for (const auto& rec : processRecords) {
        if (rec.departureTime > 0.0) {
            completedProcesses++;
        }
    }
    return static_cast<double>(completedProcesses) / totalSimulationTime;
}

double metrics::calculateCpuUtilization() const {
    if (totalSimulationTime == 0.0) return 0.0;
    return totalBusyTime / totalSimulationTime;
}

void metrics::printSummary() const {
    int completedProcesses = 0;
    for (const auto& rec : processRecords) {
        if (rec.departureTime > 0.0) {
            completedProcesses++;
        }
    }
    
    std::cout << "\n=== Simulation Metrics ===" << std::endl;
    std::cout << "Total Processes Completed: " << completedProcesses << std::endl;
    std::cout << "Total Simulation Time: " << std::fixed << std::setprecision(6) << totalSimulationTime << " time units" << std::endl;
    std::cout << "Average Turnaround Time: " << std::fixed << std::setprecision(6) << calculateAverageTurnaroundTime() << " time units" << std::endl;
    std::cout << "Process Throughput: " << std::fixed << std::setprecision(6) << calculateThroughput() << " processes per time unit" << std::endl;
    std::cout << "CPU Utilization: " << std::fixed << std::setprecision(4) << (calculateCpuUtilization() * 100.0) << "%" << std::endl;
}

void metrics::saveToFile(const std::string& filename) const {
    // Ensure output directory exists
    int ret = system("mkdir -p output");
    (void)ret; // Suppress unused variable warning
    
    int completedProcesses = 0;
    for (const auto& rec : processRecords) {
        if (rec.departureTime > 0.0) {
            completedProcesses++;
        }
    }
    
    std::ofstream outfile("output/" + filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file output/" << filename << std::endl;
        return;
    }

    outfile << "=== Simulation Metrics ===" << std::endl;
    outfile << "Total Processes Completed: " << completedProcesses << std::endl;
    outfile << "Total Simulation Time: " << std::fixed << std::setprecision(6) << totalSimulationTime << " time units" << std::endl;
    outfile << "Average Turnaround Time: " << std::fixed << std::setprecision(6) << calculateAverageTurnaroundTime() << " time units" << std::endl;
    outfile << "Process Throughput: " << std::fixed << std::setprecision(6) << calculateThroughput() << " processes per time unit" << std::endl;
    outfile << "CPU Utilization: " << std::fixed << std::setprecision(4) << (calculateCpuUtilization() * 100.0) << "%" << std::endl;
    
    outfile.close();
    std::cout << "Metrics saved to output/" << filename << std::endl;
}
