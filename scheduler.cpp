#include <iostream>
#include <assert.h>
#include "scheduler.h"

std::mt19937 gen(42034);    //ADD FIXED SEED
std::uniform_int_distribution<> dis(1, 10);
std::uniform_real_distribution<> rdis(0, 1);

void scheduler::arrivalHandler(event* arrival){
	if(cpuIdle()){
		runProcess();
	}
	else{
		rq_push(arrival->getProc());
	}
	genArrival();
}
void scheduler::departHandler(event* depart){
	if(processCompleted(depart)){
		processCounter++;
		delete depart;
	}
	else{
		contextSwitchCount++;
		rq_push(depart->getProc());
	}
	if(rq_empty){
		freeCore();
	}
	else{
		genDepart(depart->proc);
		rq_pop();
	}
}
void scheduler::genArrival(){
	process* newProc = new process;
	newProc->pid = processCounter;
	newProc->prio = 0;
	newProc-> arr = clock + (-1/arrRate*std::log(rdis(gen)));
	newProc -> ser = (-servTime*std::log(rdis(gen)));
	newProc -> exc = 0.f;
	eNode* arrNode = new eNode;
	arrNode->nodeEvent = new event;
	arrNode->nodeEvent->proc = newProc;
	arrNode->nodeEvent->type = 0;
	arrNode->nodeEvent->time = newProc->arr;
	eNode* temp = eq.head;
	if(arrNode->nodeEvent->time < eq.head->nodeEvent->time){
		arrNode->next=eq.head;
		eq.head=arrNode;
	}
	while(temp->nodeEvent->time < arrNode->nodeEvent->time && temp->next){
		arrNode->prev = temp;
		temp = temp->next;
	}
	if(temp->nodeEvent->time < arrNode->nodeEvent->time){
		arrNode->next = temp;
		temp->prev=arrNode;
	}
	else{
		temp->next = arrNode;
		arrNode->prev = temp;
	}

}
void scheduler::genDepart(process* proc){
	if(!preemptive){
		proc->exc = 
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
	return rq.head->node;
}
void scheduler::rq_pop(){
	readyNode* temp = rq.head;
	rq.head = rq.head->next;
	delete temp;
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

void scheduler::run(){
	eq_push(new event());
	while(processCounter < processCount){
		event* nextEvent = eq_pop();
		clock=nextEvent->getTime();
		if ( nextEvent->getType() == 0){
			arrivalHandler(nextEvent);
		}
		else {
			departHandler(nextEvent);
		}
	}
	std::printf("Simulation Completed %i processes ran, Clock Time: %f \n", processCounter, clock);	
}
