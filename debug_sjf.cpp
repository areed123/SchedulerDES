#include <iostream>
#include <vector>
#include <cassert>
#include "flavors.h"

int main(){
    std::cout << "Debug SJF smoke test\n";
    SJF sched;
    sched.setCoreCount(1);

    // Create processes with known service times (out-of-order)
    double services[] = {5.0, 1.0, 3.0, 2.0, 4.0};
    int N = sizeof(services)/sizeof(services[0]);

    for(int i=0;i<N;i++){
        process* p = new process;
        p->pid = i+1;
        p->prio = 0;
        p->arr = 0.0;
        p->ser = services[i];
        p->exc = 0.0;
        std::cerr << "Pushed proc pid=" << p->pid << " ser=" << p->ser << "\n";
        sched.rq_push(p);
    }

    int processed = 0;
    std::vector<int> depart_order;

    // Run until all processes depart
    while(processed < N){
        if(sched.cpuIdle() && sched.rq_top() != nullptr){
            sched.runProcess();
        }
        event* ev = sched.eq_pop();
        if(!ev){
            std::cerr << "No event to process, aborting\n";
            break;
        }
        // Note: clock is internal; departHandler uses its own clock.
        if(ev->type == 1){
            if(ev->proc) depart_order.push_back(ev->proc->pid);
            sched.departHandler(ev);
            processed++;
        } else {
            // shouldn't happen in this test
            sched.arrivalHandler(ev);
        }
    }

    std::cout << "Departure order: ";
    for(int id : depart_order) std::cout << id << " ";
    std::cout << "\n";

    std::cout << "Expected order by shortest service-time (pids with services 1.0,2.0,3.0,4.0,5.0) -> pid order: 2 4 3 5 1\n";

    return 0;
}
