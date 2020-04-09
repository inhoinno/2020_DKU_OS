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

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
/*
<test1>         <test2>
pid r   a       pid r   a
A   3   0       A   4   0
B   6   2       B   16  1
C   4   4       C   8   2
D   5   6       D   6   2
E   2   8       E   4   4
*/
    // ./out FCFS test2
    // ./out RR test1
    if (strcmp(argv[1],"-help") != 0)
    {  help(); exit(0);}
    if (argc !=3)
    { write(FILENO_STDERR,"WRONG USAGE! usage should -> ./%s sched_type test1( or test2)\n Dont know how to use, type ./%s -help\n",argv[0],argv[0]);  exit(0); }
    
    
}
