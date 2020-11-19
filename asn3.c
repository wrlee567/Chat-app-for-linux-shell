#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "asn3.h"


int pcounter= 0;
int prcss_num = 0;

#define SUCCESS 1
#define FAIL -1 
List *prtyQ[3];
List* wait_send ;
List* wait_rec;
List* runq ;
List* semap ;

//functions
int create_p (int priority){
	Pcb *newPCB = malloc(sizeof(Pcb)); 
	
    newPCB -> pid = prcss_num++;
    newPCB -> priority= priority;
	newPCB -> state = 0;
	newPCB -> msg =NULL;
	pcounter++;

	newPCB->msg = (p_MSG*) malloc(sizeof (p_MSG));
	newPCB->msg->content = (char*) malloc(sizeof(char) * 50);

	
	if (newPCB == NULL ){
		return -1;
	}
	
	else{
		List_prepend(prtyQ[priority], newPCB);
		if (prtyQ[priority]>=0) {   //fix condition later
		
			printf("Success: pid%i\n", newPCB->pid);
			return 1;
		}

		else {
			
			printf("failed to create the PCB\n");
			
			return -1;
		}
		

	}
	
	
}

// int fork_p(void*q){
// 	if (q -> current )
// 	{
// 		/* code */
// 	}
//     return 1;
// }

// int kill_p(void*q, int pid){
// 	//List current semaphore 
// 	List_remove(q);
//     return 1;
// 	//List current semaphore 
// }
int exit_p(void*q, void * pItem){

	Pcb * prcss;
	prcss = List_last(runq);
	if (prcss->pid != 0 || prcss_num == 0  ) {
		
		prcss = List_trim(runq);
		printf("pid:%i is now removed\n", prcss->pid);
		free(prcss->msg->content);
		free(prcss->msg);
		free(prcss);
		prcss->msg->content = 0;
		prcss->msg = 0;
		prcss = 0;
		prcss_num--;
		// CPU_scheduler();
	}
	else {
		printf("There are still other processes, cannot exit now");
	}
	return -1;
}
// int quantum(void* q){
// 	return 1;
// }
// int sendf(int pid){
// 	return 1;
// } 
// char receivef(void*q){
// 	//if message not sent block receive

// 	char *m  = "hi";

// 	//instantly start the receive and unblock it once the thing 
// 	return &m;
// }
// int replyf(void*q, int pid, char*msg){
// 	return 1;

// }

// static int init(void * unused) {
// 	if (pr_1 == NULL || pr_2 == NULL|| pr_3 == NULL){

// 		init process;
// 	}
// }

int main(int argc, char const *argv[])
{
	
	prtyQ[0] = List_create();
	prtyQ[1] = List_create();
	prtyQ[2] = List_create();
	wait_send = List_create();
	wait_rec = List_create();
	runq = List_create();
	semap = List_create();
	char u_input; 
	printf(" Please enter your command \n ('i' for the information on the commands \n'e' to exit the program): ");
        scanf("%s", &u_input);

	if (u_input == 'I' || u_input =='i')
	{
		
		printf("e clicked\n");
	}
	if (u_input == 'C' || u_input == 'c')
	{
		int priority;
		printf("Please enter Priority --- (0, 1, 2): ");
		scanf("%d", &priority);
		int err_chk = create_p(priority);
		
	}
	// if (u_input == 'F'|| u_input == 'f')
	// {
	// 	break;
	// }
	// if (u_input == 'K' || u_input == 'k')
	// {
	// 	break;
	// }
	// if (u_input == 'E' || u_input == 'e')
	// {
	// 	break;
	// }
	// if (u_input == 'Q' || u_input == 'q')
	// {
	// 	break;
	// }
	// if (u_input == 'S' || u_input == 's')
	// {
	// 	break;
	// }
	// if (u_input == 'R' || u_input == 'r')
	// {
	// 	break;
	// }
	// if (u_input == 'Y' || u_input == 'y')
	// {
	// 	break;
	// }
	// if (u_input == 'N' || u_input == 'n')
	// {
	// 	break;
	// }
	// if (u_input == 'P' || u_input == 'p')
	// {
	// 	break;
	// }
	// if (u_input == 'V' || u_input == 'v')
	// {
	// 	break;
	// }
	// if (u_input == 'I' || u_input == 'i')
	// {
	// 	break;
	// }
	// if (u_input == 'T' || u_input == 't')
	// {
	// 	break;
	// }
	return 0;
}



