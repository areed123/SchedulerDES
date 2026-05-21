class process{
        int pid, prio;
        double arr, ser, exc;
        public:


}

class event{
	public:
		int type;
		double time;
		process* proc;

}

class eventQ{
	struct eNode {
		eNode* next;
		eNode* prev
		event* nodeEvent
		eNode(event* e);
	}
	event* head;
	event* tail
	void newEvent(int type); //generate the next event depending on its type
	event nextEvent(); //return the next event and remove it from the queue
}
class readyQ{

}
class CPU{
	
}
class scheduler {
	public:
		void 
}
