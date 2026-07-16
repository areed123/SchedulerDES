#include <iostream>
#include <string>
#include <limits>
#include "flavors.h"
#include "metrics.h"
#include "scheduler_config.h"

// Forward declarations of menu functions
void displayMainMenu();
void editParameters(SimulationConfig& config);
void runSimulation(SimulationConfig& config, metrics& m, int runNumber, bool saveToFile);
scheduler* createScheduler(const std::string& algorithmName);

// Helper: clear input buffer
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Create scheduler instance based on algorithm name
scheduler* createScheduler(const std::string& algorithmName) {
    if (algorithmName == "FCFS") {
        return new FCFS();
    }
    if (algorithmName == "SJF") {
        return new SJF();
    }
    // Add more algorithms here as they're implemented
    std::cerr << "Unknown algorithm: " << algorithmName << std::endl;
    return nullptr;
}

// Display main menu
void displayMainMenu() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   SchedulerDES - Interactive Mode      ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << "1. View current configuration" << std::endl;
    std::cout << "2. Edit parameters" << std::endl;
    std::cout << "3. Load configuration from file" << std::endl;
    std::cout << "4. Save configuration to file" << std::endl;
    std::cout << "5. Run simulation" << std::endl;
    std::cout << "6. Toggle output mode (file/console)" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "────────────────────────────────────────" << std::endl;
    std::cout << "Enter choice: ";
}

// Edit individual parameters
void editParameters(SimulationConfig& config) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║   Edit Configuration                   ║" << std::endl;
        std::cout << "╚════════════════════════════════════════╝" << std::endl;
        config.displayConfig();
        std::cout << "1. Edit process count" << std::endl;
        std::cout << "2. Edit arrival rate" << std::endl;
        std::cout << "3. Edit service time" << std::endl;
        std::cout << "4. Edit core count" << std::endl;
        std::cout << "5. Edit algorithm (currently: " << config.getAlgorithmName() << ")" << std::endl;
        std::cout << "0. Back to main menu" << std::endl;
        std::cout << "────────────────────────────────────────" << std::endl;
        std::cout << "Enter choice: ";
        
        std::cin >> choice;
        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "Invalid input. Please try again." << std::endl;
            continue;
        }

        switch (choice) {
            case 1: {
                std::cout << "Current process count: " << config.getProcessCount() << std::endl;
                std::cout << "Enter new process count (1-1000000): ";
                int count;
                std::cin >> count;
                if (std::cin.fail()) {
                    clearInputBuffer();
                    std::cout << "Invalid input." << std::endl;
                } else {
                    config.setProcessCount(count);
                }
                break;
            }
            case 2: {
                std::cout << "Current arrival rate: " << config.getArrivalRate() << std::endl;
                std::cout << "Enter new arrival rate (0.01-1000.0): ";
                double rate;
                std::cin >> rate;
                if (std::cin.fail()) {
                    clearInputBuffer();
                    std::cout << "Invalid input." << std::endl;
                } else {
                    config.setArrivalRate(rate);
                }
                break;
            }
            case 3: {
                std::cout << "Current service time: " << config.getServiceTime() << std::endl;
                std::cout << "Enter new service time (0.001-100.0): ";
                double time;
                std::cin >> time;
                if (std::cin.fail()) {
                    clearInputBuffer();
                    std::cout << "Invalid input." << std::endl;
                } else {
                    config.setServiceTime(time);
                }
                break;
            }
            case 4: {
                std::cout << "Current core count: " << config.getCoreCount() << std::endl;
                std::cout << "Enter new core count (1-256): ";
                int cores;
                std::cin >> cores;
                if (std::cin.fail()) {
                    clearInputBuffer();
                    std::cout << "Invalid input." << std::endl;
                } else {
                    config.setCoreCount(cores);
                }
                break;
            }
            case 5: {
                std::cout << "Current algorithm: " << config.getAlgorithmName() << std::endl;
                std::cout << "Available algorithms: FCFS" << std::endl;
                std::cout << "Enter new algorithm: ";
                clearInputBuffer();
                std::string algo;
                std::getline(std::cin, algo);
                config.setAlgorithmName(algo);
                break;
            }
            case 0:
                return;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
}

// Run a single simulation
void runSimulation(SimulationConfig& config, metrics& m, int runNumber, bool saveToFile) {
    std::cout << "\n>>> Running simulation #" << runNumber << "..." << std::endl;
    
    // Create scheduler instance
    scheduler* sched = createScheduler(config.getAlgorithmName());
    if (!sched) {
        return;
    }

    // Attach metrics and configure
    m.recordCpuBusy(0); // Reset/prep metrics for new run
    sched->setMetrics(&m);
    sched->setProcessCount(config.getProcessCount());
    sched->setArrRate(config.getArrivalRate());
    sched->setServTime(config.getServiceTime());
    sched->setCoreCount(config.getCoreCount());

    // Run simulation
    sched->run();

    // Handle output (metrics.saveToFile() already prepends "output/")
    if (saveToFile) {
        std::string filename = "sim_run_" + std::to_string(runNumber) + ".txt";
        m.saveToFile(filename);
    } else {
        std::cout << "\n[Results printed above]" << std::endl;
    }

    // Reset scheduler and metrics for next run
    sched->reset();
    
    // Clean up
    delete sched;

    std::cout << ">>> Simulation #" << runNumber << " complete!" << std::endl;
}

int main() {
    SimulationConfig config;
    bool outputToFile = true;
    int runNumber = 1;

    std::cout << "╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   SchedulerDES Interactive Simulator   ║" << std::endl;
    std::cout << "║   Version 2.0                          ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    int choice = -1;
    while (choice != 0) {
        displayMainMenu();
        
        std::cin >> choice;
        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << "Invalid input. Please try again." << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                config.displayConfig();
                break;

            case 2:
                editParameters(config);
                break;

            case 3: {
                std::cout << "Enter filename to load (e.g., config.cfg): ";
                clearInputBuffer();
                std::string filename;
                std::getline(std::cin, filename);
                config.loadFromFile(filename);
                std::cout << "Configuration loaded." << std::endl;
                break;
            }

            case 4: {
                std::cout << "Enter filename to save (e.g., config.cfg): ";
                clearInputBuffer();
                std::string filename;
                std::getline(std::cin, filename);
                config.saveToFile(filename);
                break;
            }

            case 5: {
                metrics m;
                runSimulation(config, m, runNumber, outputToFile);
                runNumber++;
                
                std::cout << "\nRun another simulation? (y/n): ";
                char again;
                std::cin >> again;
                if (again == 'y' || again == 'Y') {
                    continue; // Loop back to main menu
                } else {
                    choice = 0; // Exit
                }
                break;
            }

            case 6: {
                outputToFile = !outputToFile;
                std::string mode = outputToFile ? "file" : "console";
                std::cout << "Output mode set to: " << mode << std::endl;
                break;
            }

            case 0:
                std::cout << "\nThank you for using SchedulerDES!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}
