#ifndef FLAVORS_H
#define FLAVORS_H
#include "scheduler.h"

class FCFS : public scheduler{
public:
    void rq_push(process* process) override;
};

#endif
