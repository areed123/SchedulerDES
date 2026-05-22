#ifndef FLAVORS_H
#define FLAVORS_H
#include "scheduler.h"

class FCFS : public scheduler{
	void rq_push(process* process){
		if(rq.size() == 0){
			rq.head = new readyNode(process);
			rq.tail = rq.head;
		}
		else{
			rq.tail->next = new readyNode(process);
			rq.tail = rq.tail->next;
		}
			
	}
}


#endif	     
