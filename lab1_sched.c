/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32152332
*	    Student name : 송인호  *>_<V
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
#define _RR_TIME_SLICE 2

int footprint [16][100];

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 * 
 * 
 
//주요 함수들
int Run_workload();
_env_FCFS()
_env_RR()
_env_MLFQ()
_env_STRIDE()

//API와 초기화 함수 
int init_sched();
int init_task_strct();
int init_workload();
int init_io(char * str);
int Working(task_strct * );
int Spin(int );
int assert(sched_queue * , cpu_state *);
int Assert(sched_queue * Q[], cpu_state *);
 */

//TO DO
    //cpu() 
    //API
    //Enqueue(shced_queue * Q[],task_strct task), enqueue(sched_queue * rq,task_strct task)
    //init
    //Scenario DS
    //MLFQ
    //STRIDE
    //STRIDE API

//#Define Scheduling Source Code
int Run_workload(char * scenario , int scenario_length ,int sched_policy){
    //arg1 시나리오 자료구조
    //arg2 스케쥴링 정책
    //struct sched_queue * Q [] = init_bitmap(sched_policy);

    //초기화 부분, 컴퓨터를 부팅했을때 운영체제가 테이블 초기화 하는 부분이라고 생각
    //1 cpu초기화 , runQ초기화<-sched policy 이용, 비트맵(Q위치) 초기화
    sched_queue * Q=NULL;
    cpu_state * cpu =NULL;
    switch(sched_policy){
        case FCFS_SCHED:
        {//rQ 초기화
        Q = init_sched(sched_policy,0);
        cpu = init_cpu();
        if(_env_FCFS(Q, cpu,) < 0)
            return -1;
        printf("FCFS sucess! \n ");
        //결과에 대한 보고
        break;
        }
        case ROUND_ROBIN_SCHED:
        {//rQ 초기화
        int slice ;
        scanf_s("%d", slice , sizeof(slice));
        Q = init_sched(sched_policy,slice);
        cpu = init_cpu();

        if(_env_RR(Q, cpu) < 0)
            return -1;
        printf("RR sucess! \n ");
        //결과에 대한 보고
        break;
        }
        case MLFQ_SCHED:
        {
        //rQ 초기화
        sched_queue ** rQ = init_bitmap();
        cpu = init_cpu();

        if(_env_MLFQ(Q[], cpu) < 0)
            return -1;
        printf("MLFQ sucess! \n ");
        //결과에 대한 보고
        break;
        }
        case STRIDE_SCHED:
        {//rQ 초기화
        sched_queue * Q = init_sched(sched_policy);
        cpu_state * cpu = init_cpu();

        if(_env_STRIDE() < 0)
            return -1;
        printf("STRDIE sucess! \n ");
        //결과에 대한 보고
        break;
        }
        default:
            printf("case : No such Sched %d",currq->policy);
            exit(-1);
            break;//will not execute 
    }
    printf("Workload Complete Successly . \n");
    return 1; //SUCESS

}
int 
_env_FCFS
(sched_queue * rq, cpu_state * cpu_st, char * workload)
{
    int i=0;
    int tempslice=1; 

    task_strct * new_task =NULL;
    //task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;

    int * step = (int *)malloc(sizeof(int));
    *step = 0;
   
    for(t =0; true; t++){        
        //흘러가는 시가안
        ///1: 새로운 태스크를 확인      
        int arriv = time_to_fork(workload,length ,t , step);
        while(arriv != 0){
            new_task = do_fork(workload,length, step ,sched_policy); //프로세스 생성 Heap 에 생성
            //index = update_bitmap(new_task, ); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
            new_task->id=i++;
            new_task->priority = HIGHEST_PRIORITY; // 프로세스의 우선순위 결정
            enqueue(rq,new_task); 
            arriv--;
            new_task =NULL;
        }// while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

        if(endWorkload(rq, cpu_st)) break;

        //3. FCFS scheduling time       
        //case : FCFS_SCHED
        //run till curr_task end
        if (curr_task->state == TASK_DONE){
            curr_task->fin_time =gettimeofday();
            curr_task = schdule(rq);
        }

        cpu(cpu_st , curr_task, t);
    }//end loop

    if(s_assert(cpu_st, rq) <0) 
        return -1;
    
    //SUCCESS FCFS
    return 1;     
}

int 
_env_RR
(sched_queue * rq, cpu_state * cpu_st)
{
    int i=0;
    int tempslice=0;
    int time_slice=rq->time_slice;
    
    task_strct * new_task =NULL;
    task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;

    int * step = (int *)malloc(sizeof(int));
    *step = 0;
    
    for(t =0; true; t++){        
        //흘러가는 시가안
        ///1: 새로운 태스크를 확인      
/*1*/   int arriv = time_to_fork(workload [],length ,t , step);
        while(arriv != 0){
            new_task = do_fork(workload[] , length, step ,sched_policy); //프로세스 생성 Heap 에 생성
            new_task->id=i++;
            new_task->priority = 0; // 프로세스의 우선순위 결정
            enqueue(rq,new_task); //해당 우선순위Q에 enqueue;
            arriv--;
            new_task =NULL;
        }// while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

        //2 : 기존의 태스크 확인
/*2*/   if(prev_task != NULL){
            if(prev_task -> state != TASK_DONE){
                //prev_task->priority += 1;
                enqueue(rq, prev_task); prev_task=NULL;
            }else{
                //prev_task is done
                
                prev_task =NULL;
            }
        }
        
        if(endWorkload(rq, cpu_st)) break;
        //QIsEmpty, cpu_st->cpu_state == 0

        // 1. 스케쥴러가 수행 되어야 한다면(time out) -> schedule, dequeue
        // 2. 스케쥴러가 지정한 태스크를 cpu가 수행, cpu(curr)
        // 3. 태스크가 종료되었는지, 아니면 문맥교환 해야하는지 check 프로세스의 state변경  
//3
/*3.1*/ if(time_to_schedule(tempslice, cpu_st ,rq)){      
            curr_task=schdule(rq); tempslice =0;}

/*3.2*/ cpu(cpu_st , curr_task, t);                
        tempslice++;
            
        //3  현재 workload 구현상 for문을 사용해서, 새로운 task가 들어오는걸 t의 갱신으로 알기 때문에 여기에 구현
/*3.3*/ if(cpu_st = TASK_DONE || tempslice == time_slice){
            context_save(curr_task);
            prev_task = curr_task;
            if(prev_task->state == TASK_DONE){
                tempslice=0;
            }
        }
    
    }
    
    if(s_assert(cpu_st, rq) <0) 
        return -1;
    
    //SUCCESS RR
    return 1;            
}

int 
_env_MLFQ
(sched_queue *Q [] , cpu_state * cpu_st)
{
    int tempslice=0;
    int time_slice;
    
    task_strct * new_task =NULL;
    task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;

    int * step = (int *)malloc(sizeof(int));
    *step = 0;
    sched_queue * rq= NULL;
    
    for(t =0; true; t++){        
        //흘러가는 시가안
        ///1: 새로운 태스크를 확인      
        //새로운 태스크가 왔을 때, curr_task 가 Running중이라면 
        // 우선순위에 의해서 new task가 수행되어야 한다
        //이때 curr_task를 저장해주고 Queue에 넣어주자
/*1*/   int arriv = time_to_fork(workload [],length ,t , step);
        while(arriv != 0){
            new_task = do_fork(workload[] , length, step ,sched_policy); //프로세스 생성 Heap 에 생성
            //index = update_bitmap(new_task, Q[]); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
             // 프로세스의 우선순위 결정
            //RULE 3 when new process, locate in highest
            new_task->sched_priority = HIGHEST_PRIORITY;
            Enqueue(Q,new_task); //해당 우선순위Q에 enqueue;
            if(curr_task!=NULL)
                if(curr_task->sched_priority > new_task->sched_priority){
                    context_save(curr_task);//change ready
                    prev_task = curr_task;
                    curr_task =NULL;
                }
            arriv--;
            new_task =NULL;
        }// while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

        //2 : 기존의 태스크 확인
/*2*/   if(prev_task != NULL){
            if(prev_task -> state != TASK_DONE){
                //prev_task->priority += 1;
                //RULE 4 based on current time spent in rq, lower level
                if(prev_task ->qtime >= rq->time_slice || tempslice == time_slice){//!(prev_task는 0현재 큐에서의 시간을 다 썼다)
                    lower_priority(prev_task); prev_task->qtime=0;}
                Enqueue(Q, prev_task); //enqueue to Q by task's priority
                prev_task=NULL;
            }else {
                //prev_task is done11
                prev_task =NULL;
            }
        }
        //RULE 5 booosting here 
        //init bitmap because 1. prev task done 2 . boosting
        if(EndWorkload(Q, cpu_st)) break;
        //QIsEmpty, cpu_st->cpu_state == 0
//3
/*3.1*/ //1. is Top Q empty?
            //1. is current rq empty?
                //yes then 
            //2. else then
                //schedule it (dequeue)
        //RULE 4 based on current time spent in rq, lower level
        if( time_to_schedule(tempslice, cpu_st, rq) || isTopQueue(Q,rq) ){ 
            //isTopQueue는 rq와 topQ의 priority 비교, rq가 더 높으면 1(true)return     
            rq = SelectScheduler(Q);//think about bitmap hereb
            curr_task=schdule(rq); 
            if(curr_task == NULL){
                write(STDERR_FILENO, "SCHEDULE : Q is empty dequeue error\n", 45);
                exit(-1);
            }
            time_slice = rq->time_slice;
            tempslice =0;
        }

/*3.2*/ cpu(curr_task,cpu_st,t);tempslice++; //cpu 에서 task->time ++ 
            
        //3  현재 workload 구현상 for문을 사용해서, 새로운 task가 들어오는걸 t의 갱신으로 알기 때문에 여기에 구현
/*3.3*/ if(cpu_st->cpu_state = TASK_DONE || tempslice == time_slice){
            context_save(curr_task); //원래는 현재 수행한 위치까지 저장하는 거. 지금은 state도 갱신
            if(curr_task->state == TASK_DONE){
                tempslice=0;
            }
            prev_task = curr_task;
            curr_task = NULL;
        }
    }
    
    if(Assert(Q,cpu_st) <0) 
        return -1;
    
    //SUCCESS RR
    return 1; 

}

// int 
// _env_STRIDE
// ()
// {}
// //#Endif Scheduling Source code.


void cpu(cpu_state * state , task_strct * task, int timestamp)
{
    state->cpu_state = CPU_RUNNING;
    task->state = TASK_RUNNING;
    if(task->spent_time == 0)
        task->res_time = gettimeofday();
    task->spent_time +=1 ;
    footprint[task->id][timestamp] = 1;
    //printf("%c ", task->pid);
    if(task->spent_time == task->total_time)
        state->cpu_state = CPU_EMPTY;

}

int EndWorkload(sched_queue * Q[], cpu_state * cpu){
    int i;
    if (cpu->cpu_state == CPU_EMPTY){
        for(i=0; i<3; i++) {
            if(isEmpty(Q[i]) < 0)
                return 0;
        }
        return 1;
    }
    else return 0;
}
int endWorkload(sched_queue * q, cpu_state * cpu){
    if (cpu->cpu_state == CPU_EMPTY )
        if(isEmpty(q)) return 1;//true
    return 0;
}
//사용한 API와 초기화 함수 소스코드
//#Define __init__ 
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
    int index = *step; 
    printf("do fork() %d", index);

    task_strct * new = (task_strct *)malloc(sizeof(task_strct)); //메모리가 부족한경우 
    if(new ==NULL){ 
        write(STDERR_FILENO, "ERROR in malloc, process exit with state -1", 45);
        exit(-1);
    }
    char id = workload[index][0];
    new->pid = id;
    new->state= TASK_READY ;
    new->sched_policy = sched_policy;
    new->total_time = atoi( workload[index][6]);    //vulnerable
    new->atime =  atoi(workload[index][3]);         //vulnerable
    //new->ticket
    //new->stride
    *step = index+1;
    //step은 다음 workload[][]라는 자료구조의 fork할 프로세스("str로 표현된")의 인덱스[step][]를 나타낸다
    printf("end_fork %d", index+1);
    return new;
}
sched_queue* init_sched(int policy, int slice){
    sched_queue * q = (sched_queue *)malloc(sizeof(sched_queue));
    q->front =NULL; q->rear =NULL;q->next_task=NULL;
    q->policy=policy;
    if(policy != FCFS_SCHED)
        q->time_slice = slice;

    return q;
}
sched_queue ** init_bitmap(){
    sched_queue ** MLFQ = (sched_queue **)malloc(sizeof(sched_queue *) * 3);
    int i;
    for(i=0; i<3 ; i++){
        MLFQ[i] = (sched_queue * )malloc(sizeof(sched_queue));
        MLFQ[i]->my_level = i;
        MLFQ[i]->time_slice = 2^i;
        MLFQ[i]->front =NULL;
        MLFQ[i]->rear = NULL;
        MLFQ[i]->policy = ROUND_ROBIN_SCHED;
    }
    return MLFQ;
}

int 
update_bitmap
(task_strct * , )
{
    return 0;
}
cpu_state * init_cpu(){
    cpu_state * cpu = (cpu_state *) malloc(sizeof(cpu_state));
    cpu->cpu_state = CPU_EMPTY;
    return cpu;
}
//#Endif __init__

//#Define __schedAPI__
//1. Queue & Scheduler
int //return type?
Enqueue(sched_queue * Q[], task_strct * task)
{
    //여기에서 priority를 수정할까?
    //1. task의 
    if(task->qtime < task->myrq->time_slice) //RULE 4 regardless of CPU given
        task->qtime +=1;     

    if (enqueue(Q[task->sched_priority],task) >0)
        return 1;
    char buf[64];
    sprintf("error in 'enqueue' task : %c spent_time : %d\n", task->pid, task->spent_time);
    write(STDERR_FILENO, buf, 64);
    return -1;
}
int //return type?
enqueue(sched_queue * rq , task_strct * task)
{
    if(isEmpty(rq)){
        rq->front = task;
        rq->rear = task;
    }else{
        task->myrq = rq;
        task->prev = rq->rear;
        rq->rear-> next = task;
        rq->rear = task;
    }
    return 1;

}

// task_strct* Dequeue()
// {

// }

task_strct* dequeue(sched_queue * rq)
{
    task_strct * ret; 
    if(isEmpty(rq))
        return NULL;//try catch
    
    if(rq->front == rq->rear){
        ret = rq->front;
        rq->front = NULL;
        rq->rear = NULL;
    }
    else{
        ret = rq->front;
        ret->next = rq->front;
        ret->next =NULL;
        rq->front->prev = NULL;     
    }
    return ret;
}

// task_strct*
// Schedule(sched_queue * Q[])
// {

// }

task_strct*
schedule(sched_queue * rq)
{
    task_strct * ret = dequeue(rq);
    return ret;
}

int
context_save(task_strct * before_task){
    int check = before_task->total_time - before_task->spent_time;
    //spent time 은 cpu()에서 증감됨. total time과 cpu time은 실제 시간이 아니고 quantum시간임
    //따라서 연산시 정수가 나옴.(0 =TASKDONE)
    if(check == 0){
        before_task->state = TASK_DONE;
        before_task->fin_time = gettimeofday();
    }
    else{
        before_task->state = TASK_READY;
        //if(before_task->myrq->policy == MLFQ_SCHED)
        //    before_task->sched_priority += 1; //Downward priority
    }
}
int
isEmpty(sched_queue * rq){
    if(rq->front != NULL)
        return 0;
    return 1;
}

int
isTopQueue(sched_queue ** Q, sched_queue * rq){
    if(rq!= NULL){
        if(!isEmpty(Q[0]))//then top prirority queue exist
            return (rq->my_level - Q[0]->my_level);
        else if (!isEmpty(Q[1]))
            return (rq->my_level - Q[1]->my_level);
        else 
            return 0;

    }
    return 0;
}

int s_assert
(cpu_state * cpu_st, sched_queue * rq)
{
    return 0;
}
int Assert(sched_queue * Q[], cpu_state * cpu)
{
    if(isEmpty(Q[0]) && isEmpty(Q[1] && isEmpty(Q[2]) && cpu->cpu_state == CPU_EMPTY))
        return 1;
    write(STDERR_FILENO, "Assert err \n", 16);
    return -1;
}
//#Endif __schedAPI__




// int 
// run_workload
// (char * workload [], int length ,struct sched_queue * Q [] , int sched_policy)
// {   
//     int i=0;
//     int tempslice=1;
//     init_workload()
//     init_sched(sched_policy);       
//     // = find_top_pr(bitmap)
//     task_strct * new_task =NULL;
//     task_strct * prev_task =NULL;
//     task_strct * curr_task =NULL;
//     cpu_state * cpu_st =NULL;
//     init_cpu(cpu_st);
//     int * step = (int *)malloc(sizeof(int));
//     *step = 0;
//     sched_queue * currq= NULL;
//     //cpu task queue 자료구조 초기화
//     //switch case policy
//         //case fcfs
//             //_env_fcfs()
//         //case rr
//             //_env_rr()
//         //case mlfq
//             //_env_mlfq()
//         //case stride 
//             //_env_stride()
//     for(t =0; true; t++){        
//         //흘러가는 시가안
//         ///1: 새로운 태스크를 확인      
//         int arriv = time_to_fork(workload [],length ,t , step);
//         while(arriv != 0){
//             new_task = do_fork(workload[] , length, step ,sched_policy); //프로세스 생성 Heap 에 생성
//             index = update_bitmap(new_task, Q[]); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
//             new_task->priority = index; // 프로세스의 우선순위 결정
//             Enqueue(Q,new_task); //해당 우선순위Q에 enqueue;
//             arriv--;
//             new_task =NULL;
//         }// while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

//         //2 : 기존의 태스크 확인
//         if(prev_task != NULL){
//             if(prev_task -> state != TASK_DONE){
//                 //prev_task->priority += 1;
//                 Enqueue(Q, prev_task); prev_task=NULL;
//             }else{
//                 //prev_task is done
//                 prev_task =NULL;
//             }
//         }
//         /*
//         if(EndWorkload(Q[], cpu_st)){//QIsEmpty, cpu_st->cpu_state == 0
//             break;
//         }*/

//         //3. scheduling time       
//         if(sched_policy != MLFQ_SCHED)
//             currq = Q[0];
//         else currq = SelectScheduler(Q[]);
//     //switch case
//     // FCFS(currq, *tempslice)
//     // RR(currq , &tempslice)
//     // MLFQ(Q[] , &tempslice)
//         //if(tempslice -1 < currq -> timeslice){
//             //time slice no over;
//         switch (currq->policy)
//         {
//             case : FCFS_SCHED
//             curr_task =  FCFS(currq, &tempslice)  //RR()//MLFQ()//schedule();
//             cpu(curr_task);
//             tempslice++;
//             break;

//             case : ROUND_ROBIN_SCHED
//             // 1. 스케쥴러가 수행 되어야 한다면(time out) -> schedule, dequeue
//             // 2. 스케쥴러가 지정한 태스크를 cpu가 수행, cpu(curr)
//             // 3. 태스크가 종료되었는지, 아니면 문맥교환 해야하는지 check 프로세스의 state변경  
//             //currq = SelectRunqueue(); MLFQ
//             if(time_to_schedule(tempslice, cpu_st ,currq)){      //1
//                 curr_task=schdule(currq); tempslice =0;
//             }
//             cpu_st = cpu(curr_task);         //2       
//             tempslice++;
            
//             //3  현재 workload 구현상 for문을 사용해서, 새로운 task가 들어오는걸 t의 갱신으로 알기 때문에 여기에 구현
//             if(cpu_st = TASK_DONE || tempslice == currq->){
//                 context_save(curr_task);
//                 if(curr_task->state == TASK_DONE){
//                     tempslice=0;
//                 }
//             }
//             // 1. time out 이라면 : timeout -> change curr task to READY(RUNNING -> READY)
//             // 2. task terminated라면 : termination -> change curr task to TERMINATE
//             // 3. 둘다 아니라면, 다시 enqueue

//             /*
//             if(curr->state == DONE)
//                 terminate(curr);
//             만약 이 이후에 , slice +1 = time quantum이라면
//             curr -> state =READY 
//             if ready prev_task = curr;
//             */
//             break;
            
//             /*typedef struct cpu_state{
//                 task_strct *    run;
//                 int             state
//             } */
//             //cpu(prev_task);
//         //}
//         //if (tempslice == rq->timesilce){
//             //time slice has over time to reschedule
//          //   prev_task = Dequeue(rq);
//           //  context_store(prev_task);
//         //}
//             default : 
//                 printf("case : No such Sched %d",currq->policy);
//                 exit(-1);
//                 break;//will not execute 
//         }
        

//     }
//     /*
//     //Initialize workload & process & scheduler
//     loop till scenario done// i is time
//     {
//         //1. 새로 생성된 task에 대해 비트맵수정, 그리고 enQueue작업
//         if(time_for_fork(scenario))
//         {
//             task_strct new_process= fork_process() with sched policy
//             //비트맵에 해당하는 우선순위 -> 우선순위 Queue에 삽입
//             index = change_bitmap()
//             //Q[index].Enqueue( task_strct, new_process)
//         }
//         //1.1 기존의 task enque

//         //2. 스케줄링 하는 시점 timeslice = 8이면? MLFQ면? 
//         //1. find top-proirity rq //(bitmap)
//         //2. based on bitmap & sched_policy, 
//         if( timeslice = Q.timeslice){
//             schedule
//             //check task enqueue priority
//         }

//     }end loop
//     */

// }

// int 
// run_workload
// (char * workload [], int length ,struct sched_queue * Q [] , int sched_policy)
// {   
//     int i=0;
//     int tempslice=1;
//     init_workload()
//     init_sched(sched_policy);       
//     // = find_top_pr(bitmap)
//     task_strct * new_task =NULL;
//     task_strct * prev_task =NULL;
//     task_strct * curr_task =NULL;
//     cpu_state * cpu_st =NULL;
//     init_cpu(cpu_st);
//     int * step = (int *)malloc(sizeof(int));
//     *step = 0;
//     sched_queue * currq= NULL;
//     //cpu task queue 자료구조 초기화
//     //switch case policy
//         //case fcfs
//             //_env_fcfs()
//         //case rr
//             //_env_rr()
//         //case mlfq
//             //_env_mlfq()
//         //case stride 
//             //_env_stride()
// }
// void MLFQ
// (sched_queue * Q [] ,int * currtime)
// {}
// task_strct * FCFS
// (sched_queue * rq, int * currtime)
// {
//     int temptime = *currtime;
//     task_strct * curr =  Peek(rq);
//         //then process DONE
//     if(curr->total_time == temptime){
//         curr = Dequeue(rq); temptime =0;
//     }
//     *currtime = temptime;
//     return curr;
// }

// /*



//     for(;;){
//         if(deque?)
//             break
//     }



// */

// /*
// return NEXT TASK time_out argument SCHED_QUEUE
//     Now end of time slice of task 
//     //FCFS
//     if process's time has left??
//     //RR
//     prev_task = dequeue from Q
//     if(is Q empty?)
//         then if prev_task's time has left?
//             then execute prev_task
//         else 
//             then Q is empty,Workload done
//             no task to sched. WAIT for further workload 
//             SAVE CONTEXT , prev
//             return Nothing
//         endif
//     else if then Workload left TODO
//         check any process that should be sched(like new task)
//         if there are new process
//             then enqueue that process
//         else no new process
//         endif
//         if there are some time to execute prev_task
//             then enqueue prev_task to this sched? or lower_level_q
//         else then prev_task->state = DONE
//     endif

//     return this->front
// */




