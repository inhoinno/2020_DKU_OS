/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
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
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "include/lab1_sched_types.h"
#define _SIZEOF_TEST1 5
#define _SIZEOF_TEST2 5
volatile int _NEW_TASK_HAS_ARRIVED = 0;

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 * 
int init_sched();
int init_task_strct();
int init_workload();
int init_io(char * str);
int Working(task_strct * );
int Spin(int );

 */
int test1(int sched_policy)
{
    //
    char * task_scenario[] = 
    {   "A, 0, 3",
        "B, 2, 6",
        "C, 4, 5",
        "D, 6, 4",
        "E, 8, 2",};
    int scenario_length = _SIZEOF_TEST1; 
    //이게 안되면 struct로 토큰 따서 주면 됨
    struct sched_queue * Q [] = init_bitmap(sched_policy);
    if (run_workload(task_scenario [], scenario_length , Q [],s sched_policy)<0)
        return -1;
    return 1;    
}

int
time_to_fork(char * workload [], int length, int time , int* index)
{   
    /* workload는 시간순(현실)으로 추가된다고 가정함.*/
    /* -> scenario는 정해진 규칙에 따라 작성됨을 가정 : 취약점*/
    printf("time to fork()\n");
    int i;//=*index 성능 향상 대신  코드 꼬일수 있음
    int c=0;
    for(i=0; i<length; i++){
        if (time == atoi(workload[i][3])){ c++; *index = i;}
    }
    if (c!= 0) *index = *index - c -1;
    return c;
    // 만약 0 에 1개 c :1 index 0
    // [1]에 2개 c:2 index 2 index=  index - c-1
    // 같은시간 없으면 c:0 index =0;

}
task_strct * 
do_fork
(char * workload[] , int length, int * step ,int sched_policy)
{   
    printf("do fork()");
    int index = *step;
    task_strct * new = (task_strct *)malloc(sizeof(task_strct));
    char id = workload[index][0];
    new->pid = id;
    new->state= TASK_READY ;
    new->sched_policy = sched_policy;
    new->total_time = atoi( workload[index][6]);
    new->atime =  atoi(workload[index][3]);
    //new->ticket
    //new->stride
    *step = index+1;
    return new;
}

int 
update_bitmap
()
{}

int 
run_workload
(char * workload [], int length ,struct sched_queue * Q [] , int sched_policy)
{   
    int i=0;
    int tempslice=1;
    init_workload()
    init_sched(sched_policy);       
    // = find_top_pr(bitmap)
    task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;
    int * step = (int *)malloc(sizeof(int));
    *step = 0;
    sched_queue * currq= NULL;
    for(t =0; true; t++){        
        //흘러가는 시가안
        ///1: 새로운 태스크를 확인      
        int arriv = time_to_fork(workload [],length ,t , step);
        while(arriv != 0){
            task_strct new_process = do_fork(workload[] , length, step ,sched_policy); //프로세스 생성
            index = update_bitmap(new_process, Q[]); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
            new_process->priority = index; // 프로세스의 우선순위 결정
            Enqueue(Q,new_process); //해당 우선순위Q에 enqueue; 
            arriv--;
        }// while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

        //2 : 기존의 태스크 확인
        if(prev_task != NULL){
            if(prev_task -> state != TASK_DONE){
                //prev_task->priority += 1;
                Enqueue(Q, prev_task); prev_task=NULL;
            }
        }
        //if(isEmpty()) break; // if Queue empty then Workload over

        //3. scheduling time
        /*
        Need to put some func if this is MLFQ
        if(sched_policy = MLFQ_SCHED)
        */
       
        if(sched_policy != MLFQ_SCHED)
            currq = Q[0];
    //switch case
    // FCFS(currq, *tempslice)
    // RR(currq , &tempslice)
    // MLFQ(Q[] , &tempslice)
        //if(tempslice -1 < currq -> timeslice){
            //time slice no over;
            case : FCFS_SCHED
            curr_task =  FCFS(currq, &tempslice)  //RR()//MLFQ()//schedule();
            cpu(curr_task);
            tempslice++;
            break;

            case : ROUND_ROBIN_SCHED
            if(time_to_schedule()) //cpu_struct 자료구조가 필요한가? 필요할것도 같음
            /*typedef struct cpu_state{
                task_strct *    run;
                int             state
            } */
            //CPU is empty 이전의 task가 Terminate or Ready이라면, 스케쥴링이 되어야 하는거다~!
                //curr = Dequeue()
                curr = Dispatch(currq);
            cpu(curr); //Running
            tempslice++;
            keep_continue_(); 
            // 1. time out 이라면 : timeout -> change curr task to READY(RUNNING -> READY)
            // 2. task terminated라면 : termination -> change curr task to TERMINATE
            // 3. 둘다 아니라면, 다시 enqueue

            /*
            if(curr->state == DONE)
                terminate(curr);
            만약 이 이후에 , slice +1 = time quantum이라면
            curr -> state =READY 
            if ready prev_task = curr;
            */
            break;
            //cpu(prev_task);
        //}
        //if (tempslice == rq->timesilce){
            //time slice has over time to reschedule
         //   prev_task = Dequeue(rq);
          //  context_store(prev_task);
        //}
        
        

    }
    /*
    //Initialize workload & process & scheduler
    loop till scenario done// i is time
    {
        //1. 새로 생성된 task에 대해 비트맵수정, 그리고 enQueue작업
        if(time_for_fork(scenario))
        {
            task_strct new_process= fork_process() with sched policy
            //비트맵에 해당하는 우선순위 -> 우선순위 Queue에 삽입
            index = change_bitmap()
            //Q[index].Enqueue( task_strct, new_process)
        }
        //1.1 기존의 task enque

        //2. 스케줄링 하는 시점 timeslice = 8이면? MLFQ면? 
        //1. find top-proirity rq //(bitmap)
        //2. based on bitmap & sched_policy, 
        if( timeslice = Q.timeslice){
            schedule
            //check task enqueue priority
        }

    }end loop
    */

}

// task_strct*
// RR
// (sched_queue * rq, int *currtime)
// {
//     task_strct * curr = Peek(rq);
//     int tempslice = *currtime;
//     if time to schedule then 
//         Dequeue
//     if(tempslice -1 < rq -> time_slice){
//         //time slice no over;  
//         context_restore(curr); //change Running
//         return curr;
//     }
//     if (tempslice == rq->time_slice){
//         //time slice has over time to reschedule
//         curr =Dequeue(rq);
//         tempslice =1;
//         context_save(curr); // chage READY
//     }
// }

void MLFQ
(sched_queue * Q [] ,int * currtime)
{}
task_strct * FCFS
(sched_queue * rq, int * currtime)
{
    int temptime = *currtime;
    task_strct * curr =  Peek(rq);
        //then process DONE
    if(curr->total_time == temptime){
        curr = Dequeue(rq); temptime =0;
    }
    *currtime = temptime;
    return curr;
}

/*



    for(;;){
        if(deque?)
            break
    }



*/

/*
return NEXT TASK time_out argument SCHED_QUEUE
    Now end of time slice of task 
    //FCFS
    if process's time has left??
    //RR
    prev_task = dequeue from Q
    if(is Q empty?)
        then if prev_task's time has left?
            then execute prev_task
        else 
            then Q is empty,Workload done
            no task to sched. WAIT for further workload 
            SAVE CONTEXT , prev
            return Nothing
        endif
    else if then Workload left TODO
        check any process that should be sched(like new task)
        if there are new process
            then enqueue that process
        else no new process
        endif
        if there are some time to execute prev_task
            then enqueue prev_task to this sched? or lower_level_q
        else then prev_task->state = DONE
    endif

    return this->front
*/




