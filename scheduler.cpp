#include <iostream>
#include <cassert>
#include <cmath>
#include <random>
#include "scheduler.h"

static std::mt19937 gen(42034);    // fixed seed
static std::uniform_real_distribution<> rdis(0.0, 1.0);

// Basic eventQ helpers
void eventQ::pushEvent(event* e){
    eNode* node = new eNode(e);
    if(!head){
        head = tail = node;
        return;
    }

    // Insert in ascending time order (earliest first)
    if(e->time < head->nodeEvent->time){
        node->next = head;
        head->prev = node;
        head = node;
        return;
    }

    eNode* cur = head;
    while(cur->next && cur->next->nodeEvent->time <= e->time){
        cur = cur->next;
    }
    // insert after cur
    node->next = cur->next;
    node->prev = cur;
    cur->next = node;
    if(node->next) node->next->prev = node;
    else tail = node;
}

event* eventQ::popEvent(){
    if(!head) return nullptr;
    eNode* n = head;
    event* e = n->nodeEvent;
    head = head->next;
    if(head) head->prev = nullptr;
    else tail = nullptr;
    delete n;
    return e;
}

bool scheduler::cpuIdle(){
    return busyCores < coreCount;
}

bool scheduler::processCompleted(event* e){
    // if depart event, assume process completed when event->type == 1
    return e && e->type != 0;
}

void scheduler::arrivalHandler(event* arrival){
    // place incoming process on ready queue
    if(arrival && arrival->proc){
        rq_push(arrival->proc);
        if(metricsPtr){
            metricsPtr->recordArrival(arrival->proc->pid, arrival->proc->arr);
        }
    }

    // if a core is free and there is work, start it
    if(cpuIdle() && rq.size > 0){
        if(busyCores == 0 && metricsPtr){
            lastBusyStartTime = clock;
        }
        runProcess();
    }

    // generate next arrival
    genArrival();

    // free the arrival event struct (process instance lives on in rq)
    delete arrival;
}

void scheduler::departHandler(event* depart){
    if(processCompleted(depart)){
        processCounter++;
        if(metricsPtr){
            metricsPtr->recordDeparture(depart->proc->pid, clock);
            metricsPtr->recordServiceTime(depart->proc->pid, depart->proc->ser);
        }
        delete depart->proc;
        delete depart;
    }
    else{
        contextSwitchCount++;
        rq_push(depart->proc);
    }
    if(rq.size == 0){
        if(metricsPtr && busyCores > 0){
            metricsPtr->recordCpuBusy(clock - lastBusyStartTime);
        }
        freeCore();
    }
    else{
        genDepart(rq_top());
        rq_pop();
    }
}

void scheduler::genArrival(){
    process* newProc = new process;
    newProc->pid = arrivalCounter++;
    newProc->prio = 0;
    newProc->arr = clock + (-1.0/arrRate*std::log(rdis(gen)));
    newProc->ser = (-servTime*std::log(rdis(gen)));
    newProc->exc = 0.0;

    event* ev = new event{0, newProc->arr, newProc};
    eq.pushEvent(ev);
}

void scheduler::genDepart(process* proc){
    if(!preemptive){
        // schedule a depart after remaining service
        double departTime = clock + (proc->ser - proc->exc);
        event* ev = new event{1, departTime, proc};
        eq.pushEvent(ev);
    }
    else{
        // preemptive behavior not implemented in this minimal fix
        double departTime = clock + (proc->ser - proc->exc);
        event* ev = new event{1, departTime, proc};
        eq.pushEvent(ev);
    }
}

void scheduler::runProcess(){
    assert(busyCores < coreCount);
    busyCores++;
    genDepart(rq_top());
    rq_pop();
}

void scheduler::freeCore(){
    assert(busyCores>0);
    busyCores--;
}

process* scheduler::rq_top(){
    return rq.head ? rq.head->node : nullptr;
}

void scheduler::rq_pop(){
    if(!rq.head) return;
    readyNode* temp = rq.head;
    rq.head = rq.head->next;
    if(!rq.head) rq.tail = nullptr;
    delete temp;
    rq.size--;
}

void scheduler::rq_push(process* process){
     if(rq.size == 0){
            rq.head = new readyNode(process);
            rq.tail = rq.head;
        }
        else{
            rq.tail->next = new readyNode(process);
            rq.tail = rq.tail->next;
        }
        rq.size++;
}

void scheduler::eq_push(event* e){
    eq.pushEvent(e);
}

event* scheduler::eq_pop(){
    return eq.popEvent();
}

void scheduler::run(){
    // seed first arrival
    genArrival();
    while(processCounter < processCount){
        event* nextEvent = eq_pop();
        if(!nextEvent) break;
        clock = nextEvent->time;
        if ( nextEvent->type == 0){
            arrivalHandler(nextEvent);
        }
        else {
            departHandler(nextEvent);
        }
    }
    std::printf("Simulation Completed %i processes ran, Clock Time: %f \n", processCounter, clock);
    
    // Report metrics
    if(metricsPtr){
        metricsPtr->setTotalSimulationTime(clock);
        metricsPtr->setTotalProcesses(processCounter);
        metricsPtr->printSummary();
    }
}

// simple setters (if needed elsewhere)
// implementations already in-header as inline methods; no additional definitions required here.
