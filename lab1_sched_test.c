/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/
#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"
#define _SIZEOF_TEST1 5
#define _SIZEOF_TEST2 5



/*
 * you need to implement scheduler simlator test code.
 *
 */
void help(){
    printf("***** Welcome to Sched ******\n");
    printf("$ ./sched.out 'sched_policy' 'test case'\n");
    printf("in 'sched_policy : FCFS , RR ,MLFQ ,STRIDE'\n");
    printf("****************************\n");

}
int test1(int sched_policy)
{
    const char * task_scenario1[] = 
    { // 0 2 45 (6)
        "A 0 3",
        "B 2 6",
        "C 4 4",
        "D 6 5",
        "E 8 2",};
    int scenario_length = _SIZEOF_TEST1; 
    //이게 안되면 struct로 토큰 따서 주면 됨
    if (Run_workload(task_scenario1, scenario_length ,  sched_policy)<0)
        return -1;
    return 1;    
}
int test2(int sched_policy)
{
    const char * task_scenario2[] = 
    {   "A 0 3",
        "B 0 6",
        "C 4 5",
        "D 5 4",
        "E 8 2",};
    int scenario_length = _SIZEOF_TEST2; 
    //이게 안되면 struct로 토큰 따서 주면 됨
    if (Run_workload(task_scenario2, scenario_length , sched_policy)<0)
        return -1;
    return 1;    
}

int main(int argc, char *argv[]){
/*
<test1>                     <test2>
pid     runt    arivt       pid r   a
A       3       0           A   4   0
B       6       2           B   16  1
C       4       4           C   8   2
D       5       6           D   6   2
E       2       8           E   4   4
*/
    // ./out FCFS test2
    // ./out RR test1
    // if (strcmp(argv[1],"-help") != 0)
    // {  help(); exit(0);}
    // // if (argc !=3)
    // { write(FILENO_STDERR,"WRONG USAGE! usage should -> ./%s sched_type test1( or test2)\n Dont know how to use, type ./%s -help\n",argv[0],argv[0]);  exit(0); }
    //./arg0 arg1 arg2
    //       rr    test1
    int policy=0;
    int test=0;
    policy= FCFS_SCHED;
    //test1(policy);
    test2(policy);
    policy = ROUND_ROBIN_SCHED;
    //test1(policy);
    test2(policy);

    policy = MLFQ_SCHED;
    //test1(policy);
    test2(policy);    
}
