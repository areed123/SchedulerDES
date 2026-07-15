#include <iostream>
#include "flavors.h"
#include "metrics.h"

int main(){
    std::cout << "SchedulerDES: running simple FCFS simulation" << std::endl;
    
    metrics m;
    FCFS sched;
    sched.setMetrics(&m);
    
    // configure run with specified parameters
    sched.setProcessCount(1000);  // run 1000 processes
    sched.setArrRate(10.0);       // 10 processes per time unit
    sched.setServTime(0.1);       // 0.1 time units average service time
    sched.setCoreCount(1);

    sched.run();
    
    m.saveToFile("simulation_results.txt");
    return 0;
}
