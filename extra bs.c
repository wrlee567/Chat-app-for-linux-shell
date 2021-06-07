// //functions
// int create_p (void*q, int priority){
// 	List_add();

// 	return 1;
// }

// int fork_p(void*q){
//     return 1;

// }
// int kill_p(void*q, int pid){
// 	//List current semaphore 
// 	List_remove(q);
//     return 1;
// 	//List current semaphore 
// }
// int exit_p(void*q, void * pItem){
// 	//semaphore for the queue
	
// 	//semaphore for the qure
// 	return 1;
// }
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

        // lock_kernel();
        // do_basic_setup();

        // prepare_namespace();

        // /*
        //  * Ok, we have completed the initial bootup, and
        //  * we're essentially up and running. Get rid of the
        //  * initmem segments and start the user-mode stuff..
        //  */
        // free_initmem();
        // unlock_kernel();

        // if (open("/dev/console", O_RDWR, 0) < 0)        // stdin
        //         printk("Warning: unable to open an initial console.\n");

        // (void) dup(0);                                  // stdout
        // (void) dup(0);                                  // stderr

        // /*
        //  * We try each of these until one succeeds.
        //  *
        //  * The Bourne shell can be used instead of init if we are
        //  * trying to recover a really broken machine.
        //  */

        // if (execute_command)
        //         execve(execute_command,argv_init,envp_init);
        // execve("/sbin/init",argv_init,envp_init);
        // execve("/etc/init",argv_init,envp_init);
        // execve("/bin/init",argv_init,envp_init);
        // execve("/bin/sh",argv_init,envp_init);
        // panic("No init found.  Try passing init= option to kernel.");