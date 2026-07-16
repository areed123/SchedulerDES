#ifndef METRICS_H
#define METRICS_H

#include <vector>
#include <string>

class metrics {
private:
    struct ProcessRecord {
        int pid;
        double arrivalTime;
        double departureTime;
        double serviceTime;
    };

    std::vector<ProcessRecord> processRecords;
    double totalSimulationTime = 0.0;
    double totalBusyTime = 0.0;
    int totalProcesses = 0;

public:
    metrics() = default;

    void recordArrival(int pid, double arrivalTime);
    void recordDeparture(int pid, double departureTime);
    void recordServiceTime(int pid, double serviceTime);
    void recordCpuBusy(double duration);
    void setTotalSimulationTime(double time);
    void setTotalProcesses(int count);

    double calculateAverageTurnaroundTime() const;
    double calculateThroughput() const;
    double calculateCpuUtilization() const;

    void printSummary() const;
    void saveToFile(const std::string& filename) const;
};

#endif
