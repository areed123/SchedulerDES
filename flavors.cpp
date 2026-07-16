#include "flavors.h"

void FCFS::rq_push(process* process){
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

// Shortest Job First (non-preemptive): insert into ready queue by ascending service time
void SJF::rq_push(process* process){
    if (rq.size == 0){
        rq.head = new readyNode(process);
        rq.tail = rq.head;
    } else {
        // If new process has service time <= head, insert at head
        if (process->ser <= rq.head->node->ser){
            readyNode* n = new readyNode(process);
            n->next = rq.head;
            rq.head = n;
        } else {
            // Find insertion point: first node whose next has ser >= process->ser
            readyNode* cur = rq.head;
            while (cur->next != nullptr && cur->next->node->ser < process->ser){
                cur = cur->next;
            }
            readyNode* n = new readyNode(process);
            n->next = cur->next;
            cur->next = n;
            if (n->next == nullptr) {
                rq.tail = n;
            }
        }
    }
    rq.size++;
}
