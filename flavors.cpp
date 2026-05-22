#include "flavors.h"
void FCFS::rq_push(process* process){
                if(rq.size() == 0){
                        rq.head = new readyNode(process);
                        rq.tail = rq.head;
                }
                else{
                        rq.tail->next = new readyNode(process);
                        rq.tail = rq.tail->next;
                }

}

