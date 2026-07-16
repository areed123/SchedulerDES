#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <cstddef>
#include "metrics.h"

// Simple PODs: make members public to match current implementation
struct process{
    int pid;
    int prio;
    double arr;
    double ser;
    double exc;
};

struct event{
    int type;    // 0 = arrival, 1 = depart
    double time;
    process* proc;
};

struct eNode {
    eNode* next;
    eNode* prev;
    event* nodeEvent;
    eNode(event* e=nullptr) : next(nullptr), prev(nullptr), nodeEvent(e) {}
};

class eventQ{
    public:
    eNode* head;
    eNode* tail;
    eventQ() : head(nullptr), tail(nullptr) {}

    // Minimal helpers used by scheduler implementation
    void pushEvent(event* e);
    event* popEvent();
};

struct readyNode{
    process* node;
    readyNode* next;
    readyNode(process* n=nullptr) : node(n), next(nullptr) {}
    ~readyNode() = default;
};

class readyQ{
    public:
    readyNode* head;
    readyNode* tail;
    int size;
    readyQ() : head(nullptr), tail(nullptr), size(0) {}
};

class CPU{
    // placeholder for core representation
};

class scheduler {
    protected:
    int processCounter = 0;
    int arrivalCounter = 0;
    int processCount = 0;
    double clock = 0.0;
    double arrRate = 1.0;
    double servTime = 1.0;
    readyQ rq;
    eventQ eq;
    bool preemptive = false;
    int coreCount = 1;
    int busyCores = 0;
    CPU * cores = nullptr;
    int contextSwitchCount = 0;
    metrics* metricsPtr = nullptr;
    double lastBusyStartTime = 0.0;

    public:
        // Virtual destructor for proper cleanup of derived classes
        virtual ~scheduler() = default;

        // handlers
        virtual void arrivalHandler(event* event);
        virtual void departHandler(event* event);

        // ready-queue operations (polymorphic)
        virtual void rq_push(process* process);
        virtual process* rq_top();
        virtual void rq_pop();

        // event generation hooks
        virtual void genDepart(process* proc);
        virtual void genArrival();

        // eventQ helpers (forwarders)
        void eq_push(event* e);
        event* eq_pop();

        // utility
        void freeCore();
        void runProcess();
        bool cpuIdle();
        bool processCompleted(event* e);

        void run();
        void reset();

        // simple configuration setters
        void setProcessCount(int c){ processCount = c; }
        void setArrRate(double r){ arrRate = r; }
        void setServTime(double s){ servTime = s; }
        void setCoreCount(int c){ coreCount = c; }
        void setMetrics(metrics* m){ metricsPtr = m; }
};

#endif
