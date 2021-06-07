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

#define RUNNING 0
#define READY 1 
#define BLOCKED 2

List *prtyQ[3];
List* wait_send ; // send queue
List* wait_rec; // receive queue
List* runq ;
List* semap ; //semaphore process queue 
List*mesg_q;



bool User_Comparator(void* pItem, void* pComparisonArg){
   if (*((int*)(pItem)) == *((int*)(pComparisonArg))){
       return 1;
   }
   else{
       return 0;
   }
}
void msg_and_sourceprint(p_MSG *m) {
	printf(" ");
	printf("The messae is from pid: %i - ", m->source);
	printf("%s\n", m->content);
	printf(" ");
}

void CPUswitcher(){
	Pcb *p;
	int err_chk;
	for (int i=0; i<3; i++) {
		if (List_count(prtyQ[i])>0) {
			p = List_trim(prtyQ[i]);
			// add to runQ
			if (p) {
				err_chk = List_append(runq, p);
				p->state = 0;
                // display status messages
				if (err_chk == 0) {
					printf("CPU process switch was successful");
				}
				else {
					printf("CPU process switch has failed");
				}
			} // exit loop
			break;
		}
	}
	// set init proc state
	if (p = List_last(runq)) {
		if (p->pid == 0) {
			p->state = 0;
		}
		printf("the process with pid: %i is now running. \n", p->pid);
		// if (strlen(p->msg->content) != 0) {
		// 	display_pm(p->msg);
		// }
		// reset_pm(p->msg);
	}
}

// maybe a list search function 
void* search_singleq(int target, List*ls){
	Node* search;
	List_first(ls);
	search = List_search(ls, &User_Comparator, &target);
		if (search != NULL  ){ //maybe fix later on the pointer
			return search -> pItem;
		}
		else{
			return NULL;
		}	
}

List*  Determine_queue (int trget ){
	Node*ref;
	int ind = 1;
	for (int i = 0; i < 3; i++){
		if (ref = List_search(prtyQ[i], &User_Comparator, (void*)&trget)) {
			return prtyQ[i];
		}
	}
	if (ref = List_search(wait_send, &User_Comparator, (void*)&trget)) {
		return wait_send;
	}
	if (ref = List_search(wait_rec, &User_Comparator, (void*)&trget)) {
		return wait_rec;
	}
	if (ref= List_search(runq, &User_Comparator, (void*)&trget)) {
		return runq;
	}
	if (List_count(semap)){
		SEMA *s;
		int c; 
		int cnt;
		cnt = List_count(semap);
		List_prev(semap);
		for (c=0; c<cnt; c++) {
			// start from first node of semQ and s->list
			s = List_next(semap);
			List_first(s-> sem_list);
			if (ref = List_search(s->sem_list, &User_Comparator , (void*)&trget)){
				c = cnt;
				return s-> sem_list;
			}
		}
	}	
}


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
			printf("Success: pid%i \n", newPCB->pid);
			return 1;
		}
		else {
			printf("failed to create the PCB\n");
			return -1;
		}
	}	
}

int fork_p(){
	if (runq )
	{
		Pcb * old_proc  =  List_last(runq);
		if (create_p(old_proc->pid) ==1){
			return 1;
		}
		else{
			return -1;
		}
	}
}

int kill_p(int pid){
	Pcb * prcss;
	List*pidq;
	
	pidq = Determine_queue(pid);

	if (pidq)
	{
		List_remove(pidq);
	}
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
	}
    return 1;
	
}

int exit_p(){
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
		CPUswitcher();

	}
	else {
		printf("There are still other processes, cannot exit now");
	}
	return -1;
}

int quantmf(){
	Pcb *p;
	p = List_last(runq);
	if (p->pid != 0) {
		p = List_trim(runq);
		if (p->priority < 2) {
			p->priority++;
		} //this si to readjust the priorities 
		p->state = 1;
		if (p != NULL) {
			List_prepend(prtyQ[p->priority], p);
			printf("pid: %i  is now on the ready queue.\n", p->pid);
			//printf("priority: %i\n", p->priority);
		}
	}
	else {
		p->state = 1;
	}
	CPUswitcher();
	//set new porcess 
}

// int sendf(int pid){
// 	return 1;
// } 
// char receivef(void*q){
// 	//if message not sent block receive
// 	char *m  = "hi";
// 	//instantly start the receive and unblock it once the thing 
// 	return &m;
// }

int recf(){
	int target;
	Pcb* p;
	p_MSG *m;

	p = List_last(runq);
	target = p->pid;
	if (m = search_singleq ( target, mesg_q)){
		msg_and_sourceprint(m);
		List_remove(mesg_q);
	}
	// block
	else {
		// put process on wait_rec
		if (p->pid != 0) {
			p = List_trim(runq);
			p->state = BLOCKED;
			List_prepend(wait_rec, p);
			CPUswitcher();
		}
	}
	return 1;
}

// int recplyf(void*q, int pid, char*msg){
// 	return 1;
// }
void new_semap(int sid1){
	SEMA *s;
	int eror = 1;
	if ( sid1 <= 4 &&  sid1 >= 0) {
		if (!search_singleq(sid1, semap)) {
			s = malloc(sizeof(SEMA));
			s->semid = sid1;
			s->val = 0;
			s->sem_list  = List_create();
			List_add(semap, s);
			eror = 0;
		}
	}
	if (eror =0) {
		printf("SEMAPHORE was successfully %i initialized\n", sid1);
	}
	else {
		printf("FAIL to create a SEMAPHORE");
	}

}
void P_sem(int sid1) {
	SEMA *s;
	Pcb *p;
	int f = 1;
	int blcked = 0;
	p = runq->pLastNode->pItem;
	s = search_singleq(sid1, semap);	
	if (p->pid != 0 && s ) {
		s->val--;
		if (s->val < 0) {
			p = List_trim(runq);
			p->state = 2;
			List_prepend(s->sem_list, p);
			f = 0;
			blcked = 1;
		}
	}
	if (f) {
		printf ("It was unsuccessful in the P semaphore \n");
	}
	else {
		printf("The P semphore very succesful\n");

		if (blcked = 1 ) {
			printf("This pid: %i is unfortunately blocked\n", p->pid);
		}
		CPUswitcher();
	}		
}
void V_sem(int sid1) {
	int movechk;
	Pcb *p;

	SEMA *s;
	int f = 1;
	s = search_singleq(sid1, semap);
	
	if(s) {
		s->val++;
		if (s->val <= 0) {
			if (p = List_trim(s->sem_list)) {
				List_prepend(prtyQ[p->priority], p);
				p->state = 1;
				movechk = 1;
			}
		}
		f = 0;		
	}
	if (f) {
		printf("The thing with the V sem was unsuccesful ");
	}
	else {
		printf("THe V semaphore was implemented successfully");
		if (movechk) {
			printf("The following pid: %i is now ready \n", p->pid);
		}
	}
}
			
// void procinfo(int pid) {
// 	Pcb *p;
// 	char *s;
// 	// p = (Pcb*) findpid(pid, );
// 	if (p)
// 	{
// 		printf(" ");
// 		printf("Process Information for the pid is: %i\n", p->pid);
// 		printf("The priority is: %i\n", p->priority);
// 		if (p-> state == 0)
// 		{
// 			s = "RUNNING";
// 			printf("The current State: %s\n", s);
// 		}
// 		if (p-> state == 1)
// 		{
// 			s = "READY";
// 			printf("The current State: %s\n", s);
// 		}
// 		if (p-> state == 0)
// 		{
// 			s = "BLOCKED";
// 			printf("The current State: %s\n", s);
// 		}
// 		printf("Msg: %s\n", p->msg->content);
// 		printf(" ");
// 	}
// 	else {
// 		printf("Process info is unavailable");
// 	}
// }

void totalinf(){
	return;
}
// 	p = (PCB*) findpid(pid, RET_PCB);
// 	if (p) {

// 		state = translate_state(p->state);
// 		display("=========");
// 		printf("Proc Info for pid: %i\n", p->pid);
// 		printf("Priority: %i\n", p->priority);
// 		printf("State: %s\n", state);
// 		printf("Msg: %s\n", p->msg->body);
// 		display("=========");
// 	}
// 	else {
// 		printf("Cannot get Proc Info");
// 	}
// }


// static int init(void * unused) {
// 	if (pr_1 == NULL || pr_2 == NULL|| pr_3 == NULL){

// 		init process;sem
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
	int priority;
	int err_chk;
	int pid;
	int sem_id;

	Pcb* initp = malloc(sizeof(Pcb));
	initp -> pid = 1000;
	initp -> priority =0;
	initp -> state = 0;
	initp->msg = (p_MSG*) malloc(sizeof (p_MSG));
	initp->msg->content = (char *) malloc(sizeof(char) * 50);
	List_add(runq, initp);
	
	while (List_count(runq))
	{
		printf(" Please enter your command \n ('i' for the information on the commands \n'e' to exit the program): ");
			scanf("%s", &u_input);
		// printf("This is the runq: %d", List_count(runq));
		if (u_input == 'I' || u_input =='i')
		{
			printf("Please enter pid: ");
			scanf("%i", &pid);
			//procinfo(pid);
		}
		if (u_input == 'C' || u_input == 'c'){
			printf("Please enter Priority --- (0, 1, 2): ");
			scanf("%d", &priority);
			if (priority > 2 || priority < 0)
			{
				printf("bad priority");
			}
			err_chk = create_p(priority);
		}
		if (u_input == 'F'|| u_input == 'f')
		{
			err_chk = fork();
			
		}
		if (u_input == 'K' || u_input == 'k'){
			printf("Please Enter pid: ");
			scanf("%d", &pid);
			if (prcss_num != 0 &&  pid == 0 ) {
				printf("This is the init so you cannot kill it.");
			
			}
			err_chk = kill_p(pid);
			CPUswitcher;
			
		}

		if (u_input == 'E' || u_input == 'e')
		{
			exit_p();
		
		}
		if (u_input == 'Q' || u_input == 'q')
		{
			quantmf();
		}

		if (u_input == 'S' || u_input == 's'){
			printf("Enter pid: ");
			scanf("%i", &pid);
			printf("Enter message: ");
			//scanf(" %s", buf);
			// scanf(" %[^\n]", buf);
			// send(pid, buf);
		
		}
		if (u_input == 'R' || u_input == 'r'){
			recf();
		}
		if (u_input == 'Y' || u_input == 'y')
		{
			printf("Enter pid: ");
			scanf("%i", &pid);
			printf("Enter reply: ");
			// scanf(" %[^\n]", buf);
			//reply_cmd(pid, buf);
			
		}
		if (u_input == 'N' || u_input == 'n')
		{
			printf("Please enter SEMAPHORE id: ");
			scanf("%i", &sem_id);
			new_semap(sem_id);
		
		}
		if (u_input == 'P' || u_input == 'p')
		{
			printf("Please enter SEMAPHORE id: ");
			scanf("%i", &sem_id);
			P_sem(sem_id);			
		
		}
		if (u_input == 'V' || u_input == 'v')
		{
			printf("Please enter SEMAPHORE id: ");
			scanf("%i", &sem_id);
			 V_sem(sem_id);
		
		}
		if (u_input == 'T' || u_input == 't')
		{
			totalinf();
		
		}
		scanf("%c", &u_input);
		memset(&u_input, 0, sizeof u_input);
	}

	
	return 0;
}



