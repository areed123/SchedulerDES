#include <iostream>
#include "scheduler.h"
void scheduler::arrivalHandler(event* arrival){
	if(cpuIdle()){
		runProcess(arrival->getProc());
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
		freeCpu();
	}
	else{
		eq_push(rq_top());
		rq_pop();
	}
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
