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

//#include <aio.h>
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
//#include <asm/unistd.h>

#include "include/lab1_sched_types.h"

#define _RR_TIME_SLICE 2
#define _MAX_PROCESS_LIMIT 8
#define _MAX_WORKLOAD_TIME 32
int footprint [_MAX_PROCESS_LIMIT][_MAX_WORKLOAD_TIME];
void footprint_f(){
    int i;
    int j;
    for(i=1; i<6; i++){
        for(j=0; j<_MAX_WORKLOAD_TIME; j++)
            if(footprint[i][j] == 1){
                printf("■");
                footprint[i][j] =0;
            }               
            else 
                printf("□");
        printf("\n");
    }

            
}
/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 
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


//#Define Scheduling Source Code
// int init_workload(char * scenario[] , task_strct * ret){
    
// }
int Run_workload(const char * scenario[] , int scenario_length ,int sched_policy){
    //arg1 시나리오 자료구조
    //arg2 스케쥴링 정책
    //struct sched_queue * Q [] = init_bitmap(sched_policy);

    //초기화 부분, 컴퓨터를 부팅했을때 운영체제가 테이블 초기화 하는 부분이라고 생각
    //1 cpu초기화 , runQ초기화<-sched policy 이용, 비트맵(Q위치) 초기화
    
    sched_queue * Q= malloc(sizeof(sched_queue)); 
    cpu_state * cpu =malloc(sizeof(cpu_state));
    List * HeadList =malloc(sizeof(List));
    HeadList ->head =NULL;
    HeadList->last =NULL;

    if(init_tasklist(HeadList , scenario, scenario_length)<0 )
        exit(-111);
    int i;
    tasklist * tskl = HeadList->head;
    for(i=0; tskl !=NULL &&i< 5; i++)
    {
        printf("tasklist [%d] : %c %d %d\n",i+1,tskl->current->pid ,tskl->arriv_T, tskl->current->total_time);        
        tskl = tskl->next_item;
    }

    
    switch(sched_policy){
        case FCFS_SCHED:
        {//rQ 초기화
        Q = init_sched(sched_policy,0);
        cpu = init_cpu();
        if(_env_FCFS(Q, cpu, HeadList->head) < 0)
            return -1;
        printf("FCFS sucess!\n");
        //결과에 대한 보고
        break;
        }
        case ROUND_ROBIN_SCHED:
        {//rQ 초기화
        int slice =1; //#define timeslice
        //scanf_s("%d", slice , sizeof(slice));
        Q = init_sched(sched_policy,slice);
        cpu = init_cpu();

        if(_env_RR(Q, cpu, HeadList->head) < 0)
            return -1;
        printf("RR sucess! \n");
        //결과에 대한 보고
        break;
        }
        case MLFQ_SCHED:
        {
        //rQ 초기화
        sched_queue ** rQ = init_bitmap();
        cpu = init_cpu();
        if(_env_MLFQ(rQ, cpu, HeadList->head) < 0)
            return -1;
        printf("MLFQ sucess! \n");
        //결과에 대한 보고
        break;
        }
        // case STRIDE_SCHED:
        // {//rQ 초기화
        // sched_queue * Q = init_sched(sched_policy,4);
        // cpu_state * cpu = init_cpu();

        // if(_env_STRIDE() < 0)
        //     return -1;
        // printf("STRDIE sucess! \n ");
        // //결과에 대한 보고
        // break;
        //}
        default:
            printf("case : No such Sched %d",sched_policy);
            exit(-1);
            break;//will not execute 
    }
    footprint_f();
    //free(all)
    printf("Workload Complete Successly . \n");
    return 1; //SUCESS

}
int 
_env_FCFS
(sched_queue * rq, cpu_state * cpu_st, tasklist* joblist)
{
    int i=1;
    int t=0;
    int tempslice=1; 

    
    //task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;

    int * step = (int *)malloc(sizeof(int));
    *step = 0;
   
    for(t =0; 1; t++){        
        //흘러가는 시가안
        ///1: 새로운 태스크를 확인      
        do{
            task_strct *new_task = _Module_fork(joblist,t); //프로세스 생성 Heap 에 생성
            if(new_task == NULL)
                break;
            else joblist = joblist->next_item;

            //index = update_bitmap(new_task, ); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
            new_task->id=i++;
            ///new_task->sched_priority = HIGHEST_PRIORITY; // 프로세스의 우선순위 결정
            enqueue(rq,new_task); 
            new_task =NULL;
        }while(1);// while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

        if(endWorkload(rq, cpu_st,joblist)) break;

        //3. FCFS scheduling time       
        //case : FCFS_SCHED
        //run till curr_task end
        if(curr_task ==NULL)    
            curr_task =(task_strct *)schedule(rq);
        else if (curr_task->state == TASK_DONE){
            curr_task->fin_time =t;
            curr_task =(task_strct *)schedule(rq);
        }
        if(curr_task !=NULL){
            cpu(cpu_st , curr_task, t);
            if(curr_task->spent_time == curr_task->total_time){
                //context save
                curr_task->state = TASK_DONE;
                cpu_st->cpu_state =CPU_EMPTY;
            }
        }
        //else//time 만 소비...
        
    }//end loop

    if(s_assert(cpu_st, rq) <0) 
        return -1;
    
    //SUCCESS FCFS
    return 1;     
}

int 
_env_RR
(sched_queue * rq, cpu_state * cpu_st, tasklist* joblist)
{
    int i=1;
    int t=0;
    int tempslice=0;
    int time_slice=rq->time_slice;
    
    task_strct * new_task =NULL;
    task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;

    int * step = (int *)malloc(sizeof(int));
    *step = 0;
    
    for(t =0; 1; t++){        
        //흘러가는 시가안
        ///1: 새로운 태스크를 확인      
/*1*/   do{
            task_strct *new_task = _Module_fork(joblist,t); //프로세스 생성 Heap 에 생성
            if(new_task == NULL)
                break;
            else joblist = joblist->next_item;
            //index = update_bitmap(new_task, ); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
            new_task->id=i++;
            ///new_task->sched_priority = HIGHEST_PRIORITY; // 프로세스의 우선순위 결정
            enqueue(rq,new_task); 
            new_task =NULL;
        }while(1);// while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

        //2 : 기존의 태스크 확인s
/*2*/   if(prev_task != NULL){
            if(prev_task -> state != TASK_DONE){
                //prev_task->priority += 1;
                enqueue(rq, prev_task); prev_task=NULL;
            }else{
                //prev_task is done
                prev_task =NULL;
            }
        }
        
        if(endWorkload(rq, cpu_st,joblist)) break;
        //QIsEmpty, cpu_st->cpu_state == 0

        // 1. 스케쥴러가 수행 되어야 한다면(time out) -> schedule, dequeue
        // 2. 스케쥴러가 지정한 태스크를 cpu가 수행, cpu(curr)
        // 3. 태스크가 종료되었는지, 아니면 문맥교환 해야하는지 check 프로세스의 state변경  
//3
/*3.1*/ if(time_to_schedule(tempslice, cpu_st ,rq)>0){      
            curr_task =schedule(rq); tempslice =0;}
        if(curr_task !=NULL){
/*3.2*/     cpu(cpu_st , curr_task, t);                
            tempslice++;
        //3  현재 workload 구현상 for문을 사용해서, 새로운 task가 들어오는걸 t의 갱신으로 알기 때문에 여기에 구현
/*3.3*/     if((curr_task->spent_time == curr_task->total_time) || (tempslice == time_slice)){
                context_save(curr_task); //change to TASK DONE or TASK READY
                prev_task = curr_task;
                if(prev_task->state == TASK_DONE){
                    cpu_st->cpu_state =CPU_EMPTY;
                    prev_task->fin_time = t;
                    tempslice=0;
                }
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
(sched_queue *Q [] , cpu_state * cpu_st, tasklist * joblist)
{
    int tempslice=0;
    int time_slice;
    int t;
    int i=1;
    
    task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;

    int * step = (int *)malloc(sizeof(int));
    *step = 0;
    sched_queue * rq= NULL;
    printf("Q[0]->time_slice : %d \nQ[1]->time_slice %d\nQ[2]->time_slice: %d\n",Q[0]->time_slice,Q[1]->time_slice,Q[2]->time_slice);
    rq=Q[0];

    for(t =0; 1; t++){        
/*1 새로운 태스크*/   
        do{
            task_strct *new_task = _Module_fork(joblist,t); //프로세스 생성 Heap 에 생성
            if(new_task == NULL)
                break;
            else joblist = joblist->next_item;
            //index = update_bitmap(new_task, ); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
            new_task->id=i++;
            new_task->sched_priority = HIGHEST_PRIORITY; // 프로세스의 우선순위 결정
            Enqueue(Q,new_task); //해당 우선순위Q에 enqueue;
            if(curr_task!=NULL)
                if(curr_task->spent_time >= curr_task->myrq->time_slice){
                    context_save(curr_task);//change ready
                    //prev_task = curr_task;
                    lower_priority(curr_task); curr_task->qtime=0;
                    Enqueue(Q,curr_task);
                    curr_task =NULL;
                }
            new_task =NULL;
        }while(1);
        // while문 안에서는 실제로 shell이 어느정도 하는 일을 한다
        if(EndWorkload(Q, cpu_st, joblist)) break;
        //2 : 기존의 태스크 확인
/*2 currtask에 스케쥴*/   
        if(!IsEmpty(Q)){
            if( time_to_schedule(tempslice, cpu_st, rq)|| !isTopQueue(Q,rq) ){ //
                //선점 조건1. CPU EMPTY
                //2 . myrq의 시간 조건 만료
                //rq가 topQ가 아니라면 true return(topQ가 empty가 아닐때)    
                //if(curr_task->qtime >= curr_task->myrq->time_slice){
                    rq = (sched_queue *)SelectScheduler(Q);
                    curr_task=schedule(rq); 
                    if(curr_task == NULL){
                        write(STDERR_FILENO, "SCHEDULE : Q is empty dequeue error\n", 45);
                        exit(-1);
                    }
                time_slice = rq->time_slice;
                tempslice =0;
                //}
            }
        }
        

        // if(prev_task != NULL){
        //     if(prev_task -> state != TASK_DONE){
        //         //prev_task->priority += 1;
        //         //RULE 4 based on current time spent in rq, lower level
        //         if(prev_task ->qtime >= rq->time_slice || tempslice == time_slice){//!(prev_task는 0현재 큐에서의 시간을 다 썼다)
        //             lower_priority(prev_task); prev_task->qtime=0;}
        //         Enqueue(Q, prev_task); //enqueue to Q by task's priority
        //         prev_task=NULL;
        //     }else {
        //         //prev_task is done11
        //         prev_task =NULL;
        //     }
        // }
        //RULE 5 booosting here 
        //init bitmap because 1. prev task done 2 . boosting
        
        //QIsEmpty, cpu_st->cpu_state == 0
//3 curr task에 수행    
/*3.1*/ //RULE 4 based on current time spent in rq, lower level
        
        if(curr_task != NULL){
            printf("\n%c ", curr_task->pid);
/*3.2*/     cpu(cpu_st, curr_task,t);tempslice++;             
/*4 조건을 확인하고 lower -> enqueue*/ 
        if(!IsEmpty(Q))
        {//Q가 모두 empty라면 task를 계속 수행함
            if((curr_task->spent_time == curr_task->total_time) || tempslice == time_slice){
                context_save(curr_task); //원래는 현재 수행한 위치까지 저장하는 거. 지금은 state도 갱신
                if(curr_task->state == TASK_DONE){
                    curr_task->fin_time=t;
                    tempslice=0;
                }
                else
                    prev_task = curr_task;
                curr_task = NULL; //현재 수행중인 Task가 없음
            }
            if(prev_task != NULL){
                if(prev_task -> state != TASK_DONE){
                    //prev_task->priority += 1;
                    //RULE 4 based on current time spent in rq, lower level
                    if(prev_task ->qtime >= rq->time_slice || tempslice == time_slice){//!(prev_task는 0현재 큐에서의 시간을 다 썼다)
                        lower_priority(prev_task); prev_task->qtime=0;}
                    Enqueue(Q, prev_task); //enqueue to Q by task's priority
                    prev_task=NULL;
                }else
                    prev_task =NULL;//prev_task is done11
                
            }
        }
        }
    }
    if(Assert(Q,cpu_st) <0) 
        return -1;
    
    //SUCCESS RR
    return 1; 

}
/*
int 
_env_MLFQ
(sched_queue *Q [] , cpu_state * cpu_st, tasklist * joblist)
{
    int tempslice=0;
    int time_slice;
    int t;
    int i=1;
    
    task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;

    int * step = (int *)malloc(sizeof(int));
    *step = 0;
    sched_queue * rq= NULL;
    printf("Q[0]->time_slice : %d \nQ[1]->time_slice %d\nQ[2]->time_slice: %d\n",Q[0]->time_slice,Q[1]->time_slice,Q[2]->time_slice);
    rq=Q[0];

    for(t =0; 1; t++){        
        //흘러가는 시가안
        ///1: 새로운 태스크를 확인      
        //새로운 태스크가 왔을 때, curr_task 가 Running중이라면 
        // 우선순위에 의해서 new task가 수행되어야 한다
        //이때 curr_task를 저장해주고 Queue에 넣어주자
  
        do{
            task_strct *new_task = _Module_fork(joblist,t); //프로세스 생성 Heap 에 생성
            if(new_task == NULL)
                break;
            else joblist = joblist->next_item;
            //index = update_bitmap(new_task, ); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
            new_task->id=i++;
            new_task->sched_priority = HIGHEST_PRIORITY; // 프로세스의 우선순위 결정
            Enqueue(Q,new_task); //해당 우선순위Q에 enqueue;
            if(curr_task!=NULL)
                if(curr_task->sched_priority > new_task->sched_priority){
                    context_save(curr_task);//change ready
                     prev_task = curr_task;
                     curr_task =NULL;
                     //그냥 q에 넣어주고 null로 만들까?**************
                     //그냥 q에 넣어주고 null로 만들까?**************
                     //그냥 q에 넣어주고 null로 만들까?**************

            }
            new_task =NULL;
        }while(1);
        // while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

        //2 : 기존의 태스크 확인
/2/   if(prev_task != NULL){
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
        if(EndWorkload(Q, cpu_st, joblist)) break;
        //QIsEmpty, cpu_st->cpu_state == 0
//3     
/*3.1/ //RULE 4 based on current time spent in rq, lower level
        if( time_to_schedule(tempslice, cpu_st, rq) || isTopQueue(Q,rq) ){ 
            //isTopQueue는 rq와 topQ의 priority 비교, rq가 더 높으면 1(true)return     
            rq = (sched_queue *)SelectScheduler(Q);//think about bitmap hereb
            curr_task=schedule(rq); 
            if(curr_task == NULL){
                write(STDERR_FILENO, "SCHEDULE : Q is empty dequeue error\n", 45);
                exit(-1);
            }
            time_slice = rq->time_slice;
            tempslice =0;
        }

    cpu(cpu_st, curr_task,t);tempslice++; //cpu 에서 task->time ++ 
            
        //3  현재 workload 구현상 for문을 사용해서, 새로운 task가 들어오는걸 t의 갱신으로 알기 때문에 여기에 구현
        if((curr_task->spent_time == curr_task->total_time) || tempslice == time_slice){
            context_save(curr_task); //원래는 현재 수행한 위치까지 저장하는 거. 지금은 state도 갱신
            if(curr_task->state == TASK_DONE){
                curr_task->fin_time=t;
                tempslice=0;
            }
            else
                prev_task = curr_task;
            curr_task = NULL; //현재 수행중인 Task가 없음
        }
    }
    
    if(Assert(Q,cpu_st) <0) 
        return -1;
    
    //SUCCESS RR
    return 1; 

}
*/
// int 
// _env_STRIDE
// ()
// {}
// //#Endif Scheduling Source code.
int 
IsEmpty(sched_queue ** Q) {
    if(isEmpty(Q[0]))
        if(isEmpty(Q[1]))
            if(isEmpty(Q[2]))
                return 1;
    return 0;
}
void cpu
(cpu_state * state , task_strct * task, int timestamp)
{
    state->cpu_state = CPU_RUNNING;
    task->state = TASK_RUNNING;
    if(task->spent_time == 0)
        task->res_time = timestamp;    
    if(task->spent_time == task->total_time){
        state->cpu_state = CPU_EMPTY;
        context_save(task);
    }
    task->spent_time +=1 ;
    footprint[task->id][timestamp] = 1;
    printf("%c ", task->pid);
}

int EndWorkload(sched_queue * Q[], cpu_state * cpu, tasklist * joblist){
    int i;
    if (cpu->cpu_state == CPU_EMPTY){
        if(joblist ==NULL){    
            for(i=0; i<3; i++) {
                if(isEmpty(Q[i]) != 1) //1 true, q is empty
                    return 0;
            }
            return 1;
        }//else
        return 0;
    }
    else return 0;
}
int endWorkload(sched_queue * q, cpu_state * cpu, tasklist*joblist){
    if (cpu->cpu_state == CPU_EMPTY )
        if (joblist ==NULL)
            if(isEmpty(q)) return 1;//true
    return 0;
}
//사용한 API와 초기화 함수 소스코드
//#Define __init__ 
void lower_priority(task_strct * _task){
    if(_task->sched_priority < LOWEST_PRIORITY) 
        _task->sched_priority +=1;
    else 
        return;
}
int
time_to_schedule(int currslice, cpu_state* cpu , sched_queue * rq){
    if( currslice >= rq->time_slice)
        return 1;
    else if(cpu->cpu_state == CPU_EMPTY)
        return 1;
    else //curr slice <= timeslice, cpu != empty..
        return 0;
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
        if (time == atoi(&workload[i][3])){ c++; *index = i;}
    }
    if (c!= 0) *index = *index - c -1;
    return c;
    // 만약 0 에 1개 c :1 index 0
    // [1]에 2개 c:2 index 2 index=  index - c-1
    // 같은시간 없으면 c:0 index =0;

}
// task_strct * 
// do_fork
// (char * workload[], int * step)
// {   
//     int index = *step; 
//     printf("do fork() %d", index);

//     task_strct * new = (task_strct *)malloc(sizeof(task_strct)); //메모리가 부족한경우 
//     if(new ==NULL){ 
//         write(STDERR_FILENO, "ERROR in malloc, process exit with state -1", 45);
//         exit(-1);
//     }
//     char id = workload[index][0];
//     new->pid = id;
//     new->state= TASK_READY ;
//     //new->sched_policy = sched_policy;
//     new->total_time = atoi(&workload[index][6]);    //vulnerable
//     new->arr_time =  atoi(&workload[index][3]);         //vulnerable
//     //new->ticket
//     //new->stride
//     *step = index+1;
//     //step은 다음 workload[][]라는 자료구조의 fork할 프로세스("str로 표현된")의 인덱스[step][]를 나타낸다
//     printf("end_fork %d", index+1);
//     return new;
// }
//start
int
init_tasklist
(List * return_list,const char * scenario[], int wlength)
{
    //초기화 하고 왔다고 가정하고 
    int i;    
    for(i=0; i<wlength; i++){
        tasklist * item = (tasklist * )malloc(sizeof(tasklist)+1);
        item->arriv_T=0;
        item->current =NULL;
        item->next_item=NULL;
        //tasklist

        task_strct * task = (task_strct *)malloc(sizeof(task_strct)+1);
        do_fork(scenario[i],task);
        //task 생성
        item->current = task;
        item->arriv_T = task->arr_time;
        
        addList(return_list, item);
        item =item->next_item;
    }
    return 0;


}
int do_fork(const char * str, task_strct * t)
{
    if(str != NULL){
        t->pid = str[0];
        t->arr_time = atoi(&str[2]);
        t->total_time = atoi(&str[4]);
        return 1;
    }
    else return -1;
}
int 
addList(List * L, tasklist * tl){
    if(L->head == NULL){
        L->head = tl;
        L->last = tl;
    }
    else{
        L->last->next_item =tl;
        L->last= tl; 
    }
    return 1;

}
task_strct *
_Module_fork(tasklist * joblist, int t)
{   
    //in Run_workload
    //_env ( ... ,HeadList ->head, ...)
        //in _env(tsklist)
        //task_strct * new_task = _Module_fork(tsklist ,t)
    task_strct * ret =NULL;
    //printf("joblist %p joblist->current->pid %c\n",joblist, joblist->current->pid);
    //printf("joblist->next_item %p\n", joblist->next_item);
    if(joblist == NULL)
        return ret;
    if((joblist->arriv_T - t) == 0)
        ret=joblist->current;
    return ret;
}
//end
sched_queue* 
init_sched(int policy, int slice){
    sched_queue * q = (sched_queue *)malloc(sizeof(sched_queue));
    q->front =NULL; q->rear =NULL;q->next_task=NULL;
    q->policy=policy;
    if(policy != FCFS_SCHED)
        q->time_slice = slice;

    return q;
}
sched_queue **
init_bitmap(){
    sched_queue ** MLFQ = (sched_queue **)malloc(sizeof(sched_queue *) * 3 +1);
    int i;
    for(i=0; i<3 ; i++){
        MLFQ[i] = (sched_queue * )malloc(sizeof(sched_queue));
        MLFQ[i]->my_level = i;
        MLFQ[i]->time_slice = (int)(1<<i);
        MLFQ[i]->front =NULL;
        MLFQ[i]->rear = NULL;
        MLFQ[i]->policy = ROUND_ROBIN_SCHED;
    }
    return MLFQ;
}

// int 
// update_bitmap
// (task_strct * , )
// {
//     return 0;
// }
cpu_state * init_cpu(){
    cpu_state * cpu = (cpu_state *) malloc(sizeof(cpu_state));
    cpu->cpu_state = CPU_EMPTY;
    return cpu;
}
//#Endif __init__

//#Define __schedAPI__
//1. Queue & Scheduler
sched_queue *
SelectScheduler(sched_queue * Q[]){
    if( isEmpty(Q[0]) == 0 )
        return Q[0];
    else if ( isEmpty(Q[1]) ==0 ) 
        return Q[1];
    else 
        return Q[2];
}
int //return type?
Enqueue(sched_queue * Q[], task_strct * task)
{
    //여기에서 priority를 수정할까?
    //1. task의 
    if(task->myrq != NULL)
        if(task->qtime < task->myrq->time_slice) //RULE 4 regardless of CPU given
            task->qtime +=1;
    if (enqueue(Q[task->sched_priority],task) >0)
        return 1;
    
    char buf[64];
    sprintf(buf,"error in 'enqueue' task : %c spent_time : %d\n", task->pid, task->spent_time);
    write(STDERR_FILENO, buf, 64);
    return -1;
}
int //return type?
enqueue(sched_queue * rq , task_strct * task)
{
    task->myrq= rq;
    if(isEmpty(rq)){
        rq->front = task;
        rq->rear = task;
    }else{
        task->myrq = rq;
        task->prev = rq->rear;
        (rq->rear)->next = task;
        rq->rear = task;
    }
    task->sched_policy = rq->policy;
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
        rq->front = ret->next;
        ret->next =NULL;
        (rq->front)->prev = NULL;     
    }
    return ret;
}

// task_strct*
// Schedule(sched_queue * Q[])
// {

// }
task_strct *
schedule(sched_queue * rq)
{
    task_strct * ret = (task_strct *)dequeue(rq);
    return ret;
}

int
context_save(task_strct * before_task){
    int check = before_task->total_time - before_task->spent_time;
    //spent time 은 cpu()에서 증감됨. total time과 cpu time은 실제 시간이 아니고 quantum시간임
    //따라서 연산시 정수가 나옴.(0 =TASKDONE)
    if(check == 0){
        before_task->state = TASK_DONE;
        //before_task->fin_time = gettimeofday();
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
    if(rq != NULL){
        if(!isEmpty(Q[0]))//then top prirority queue exist
            if( (rq->my_level - Q[0]->my_level ==0) )
                return 1;
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
    if(isEmpty(Q[0]) && isEmpty(Q[1]) && isEmpty(Q[2]) && (cpu->cpu_state == CPU_EMPTY))
        return 1;
    write(STDERR_FILENO, "Assert err \n", 16);
    return -1;
}
//#Endif __schedAPI__


