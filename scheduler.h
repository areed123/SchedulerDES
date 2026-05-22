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
	void newDepart(event* depart);
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
};
class CPU{
	
};
class scheduler {
	int processCounter;
       	int processCount;
	double clock;
	readyQ rq;
	public:
		virtual void arrivalHandler(event* event);
		virtual void departHandler(event* event);
		virtual void rq_push(process* process)
		void run();
};
#endif
