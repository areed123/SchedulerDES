#ifndef SCHEDULER_H
#define SCHEDULER_H

class process{
        int pid, prio;
        double arr, ser, exc;
        public:
		

};

class event{
	public:
		int type;
		double time;
		process* proc;

};
struct eNode {
	eNode* next;
       	eNode* prev
        event* nodeEvent
	eNode(event* e);
};
class eventQ{
	public:
	eNode* head;
	eNode* tail
	void newArrival(int type); //generate the next event depending on its type
	void newDepart(process* depart);
	event* nextEvent(); //return the next event and remove it from the queue
};
struct readyNode{
       	process* node;
        readyNode* next;
        readyNode(process* n);
        ~readyNode();
};

class readyQ{
	public:
	readyNode* head;
	readyNode* tail;
	int size;	
};
class CPU{
	
};
class scheduler {
	protected:
	int processCounter;
       	int processCount;
	double clock;
	double arrRate;
	double servTime;
	readyQ rq;
	eventQ eq;
	bool preemptive;
	int coreCount;
	int busyCores;
	CPU * cores;
	public:
		virtual void arrivalHandler(event* event);
		virtual void departHandler(event* event);
		virtual void rq_push(process* process);
		virtual void genDepart(process* proc);
		virtual void genArrival(process* proc);
		process* rq_top();
		void freeCore();
		void rq_pop();
		void runProcess();

		void run();
};
#endif
