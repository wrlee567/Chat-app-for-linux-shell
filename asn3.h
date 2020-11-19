
#include "list.h"

typedef struct p_msg p_MSG;
struct p_msg {
	int destin;
	int src; 
	int type;
	char * content;
};

typedef struct sem SEM;
struct sem {
	int sid;
	int val;
	List *sem_list;
};

typedef struct pcb Pcb;
struct pcb {
    int pid;
    int priority;
	int state;
	p_MSG *msg;
};

