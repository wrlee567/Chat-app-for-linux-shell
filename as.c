#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "list.h"

typedef struct p_msg p_MSG;
struct p_msg {
	int dest;
	int src; 
	int type;
	char * content;
};

typedef struct pcb Pcb;
struct pcb {
    int pid;
    int priority;
	int state;
	p_MSG *msg;
};

typedef struct sem SEM;
struct sem {
	int sid;
	int value;
	List *sem_list;
};