#include <iostream>
#include "scheduler.h"

void scheduler::run(){
	eq.push(new event(*this));
	while(processCounter < processCount){
		event* nextEvent = eq.pop();
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
