
#include "list.h"

typedef struct p_msg p_MSG;
struct p_msg {
	int target;
	int source; 
	int type;
	char * content;
};

typedef struct sem SEMA;
struct sem {
	int semid;
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

