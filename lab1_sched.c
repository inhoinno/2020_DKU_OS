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

#define _RR_TIME_SLICE 2
#define _MAX_PROCESS_LIMIT 8
#define _MAX_WORKLOAD_TIME 32
int footprint [_MAX_PROCESS_LIMIT][_MAX_WORKLOAD_TIME];
void footprint_f(){
    int i;
    int j;
    for(i=1; i<6; i++){
        for(j=0; j<_MAX_WORKLOAD_TIME; j++)
            if(j!=0)
                if(footprint[i][j] == 1){
                    printf("■");
                    footprint[i][j] =0;
                }               
                else 
                    printf("□");
            else {
                char c = 'A';
                printf("%c ", c+i-1);
            }          
            
            
        
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
    List * strideList = malloc(sizeof(List));
    HeadList ->head =NULL;
    HeadList->last =NULL;
    strideList->head =NULL;
    strideList->last =NULL;

    if(init_tasklist(HeadList , scenario, scenario_length)<0 )
        exit(-111);
    int i;
    tasklist * tskl = HeadList->head;
    switch(sched_policy){
        case FCFS_SCHED:
        {//rQ 초기화
        for(i=0; tskl !=NULL &&i< 5; i++)
        {
            printf("tasklist [%d] : %c %d %d\n",i+1,tskl->current->pid ,tskl->arriv_T, tskl->current->total_time);        
            tskl = tskl->next_item;
        }
        Q = init_sched(sched_policy,0);
        cpu = init_cpu();
        printf("  ");

        if(_env_FCFS(Q, cpu, HeadList->head) < 0)
            return -1;
        printf(" FCFS sucess!\n");
        showList(HeadList->head);
        //결과에 대한 보고
        break;
        }
        case ROUND_ROBIN_SCHED:
        {//rQ 초기화
        for(i=0; tskl !=NULL &&i< 5; i++)
        {
            printf("tasklist [%d] : %c %d %d\n",i+1,tskl->current->pid ,tskl->arriv_T, tskl->current->total_time);        
            tskl = tskl->next_item;
        }
        int slice =1; //#define timeslice
        //scanf_s("%d", slice , sizeof(slice));
        Q = init_sched(sched_policy,slice);
        cpu = init_cpu();printf("  ");

        if(_env_RR(Q, cpu, HeadList->head) < 0)
            return -1;
        printf(" RR sucess! \n");
        showList(HeadList->head);
        //결과에 대한 보고
        break;
        }
        case MLFQ_SCHED:
        {
        //rQ 초기화
        for(i=0; tskl !=NULL &&i< 5; i++)
        {
            printf("tasklist [%d] : %c %d %d\n",i+1,tskl->current->pid ,tskl->arriv_T, tskl->current->total_time);        
            tskl = tskl->next_item;
        }
        sched_queue ** rQ = init_bitmap();
        cpu = init_cpu();
        if(_env_MLFQ(rQ, cpu, HeadList->head) < 0)
            return -1;
        printf(" MLFQ sucess! \n");
        showList(HeadList->head);
        //결과에 대한 보고
        break;
        }
        case STRIDE_SCHED:
        {//rQ 초기화
        // sched_queue * Q = init_sched(sched_policy,4);
            strideList = _init_STRIDE_ABC(); // A : 3 B :2 C :1
            tskl = strideList->head;
            for(i=0; tskl !=NULL &&i< 3; i++)
            {
                printf("tasklist [%d] : %c %d %d \n",i+1,tskl->current->pid ,tskl->arriv_T, tskl->current->total_time);        
                tskl = tskl->next_item;
            }
            printf("  ");
            cpu_state * cpu = init_cpu();
            heap_stride * heap = init_stride_heap(3); //heap 은 2^n -1 의 크기라 가정, 현재 1 -23-NIL
            if(_env_STRIDE(heap, cpu ,strideList->head) < 0)
                return -1;
            printf("STRIDE sucess! \n");
            showList(strideList->head);
        // //결과에 대한 보고
            break;
        }
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
    int BOOST =10;
    task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;

    int * step = (int *)malloc(sizeof(int));
    *step = 0;
    sched_queue * rq= NULL;
    printf("Q[0]->time_slice : %d \nQ[1]->time_slice %d\nQ[2]->time_slice: %d\n  ",Q[0]->time_slice,Q[1]->time_slice,Q[2]->time_slice);
    rq=Q[0];
    time_slice=rq->time_slice;

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
                if( tempslice >= time_slice){
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
        //Boosting 하려면 주석을 해제하시면 됩니다.(default t =10)
        // if(t == BOOST){
        //     MLFQ_boosting(Q);
        //     BOOST = BOOST *2;
        // }
        
        //2 : 기존의 태스크 확인
/*2 currtask에 스케쥴*/   
        if(!IsEmpty(Q)){
            if( time_to_schedule(tempslice, cpu_st, rq)){ //
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
        
        
//3 curr task에 수행    
/*3.1*/ //RULE 4 based on current time spent in rq, lower level
        
        if(curr_task != NULL){
/*3.2*/     cpu(cpu_st, curr_task,t);tempslice++;             
/*4 조건을 확인하고 lower -> enqueue*/ 
        if(!IsEmpty(Q))
        {//Q가 모두 empty라면 task를 계속 수행함
            if((curr_task->spent_time == curr_task->total_time) || tempslice == time_slice){
				//사실 ^ 여기 조건을 cpu->state == CPU_EMPTY로 해도컴터는  빼는 연산은 똑같이 수행되는거 아닌가 싶어서 안바꿈	
                context_save(curr_task); //원래는 현재 수행한 위치까지 저장하는 거. 지금은 state도 갱신
                if(curr_task->state == TASK_DONE){
                    curr_task->fin_time=t;
                    tempslice=0;
                    cpu_st->cpu_state=CPU_EMPTY;
                }
                else
                    prev_task = curr_task;
                curr_task = NULL; //현재 수행중인 Task가 없음
            
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
    }
    if(Assert(Q,cpu_st) <0) 
        return -1;
    
    //SUCCESS RR
    return 1; 

}
int 
MLFQ_boosting(sched_queue * Q[])
{
    task_strct * iter;
    int i;
    int topq = isEmpty(Q[0]);
    printf("<b>");
     // empty 1 : !empty 0
    //Boosting은 다른거 없구 모든 Q를 최상위 Q로 옮겨 준다/
    //task 자료구조 상에 myrq라는 필드가 있기 때문에 이것도 전부 바꿔주도록 한다.
    //boosting 되면 qtime이라는 현재 q에서의 시간이 불필요 하기 때문에 0으로 초기화 해도 문제 없을 것

    if(!isEmpty(Q[1]))
    {
        iter = Q[1]->front;
        if(isEmpty(Q[0])){
            Q[0]->front = Q[1]->front;
            Q[0]->rear =Q[1]->rear;
        }
        else{
        Q[0]->rear->next =  Q[1]->front;
        Q[1]->front->prev =Q[0]->rear;
        Q[0]->rear = Q[1]->rear;
        }
        do{
            iter->myrq = Q[0];
            iter->qtime =0;
            iter->sched_priority =HIGHEST_PRIORITY;
            iter = iter->next;
        }while(iter != Q[0]->rear && iter !=NULL);
        Q[1]->front =NULL;
        Q[1]->rear =NULL;
    }
    if(!isEmpty(Q[2]))
    {
        iter = Q[2]->front;
        if(isEmpty(Q[0])){
            Q[0]->front = Q[2]->front;
            Q[0]->rear =Q[2]->rear;
        }
        else{
            Q[0]->rear->next =  Q[2]->front;
            Q[2]->front->prev =Q[0]->rear;
            Q[0]->rear = Q[2]->rear;
        }
        do{
            iter->myrq = Q[0];
            iter->sched_priority =HIGHEST_PRIORITY;
            iter->qtime =0;
            iter = iter->next;
        }while(iter != Q[0]->rear && iter !=NULL);
        Q[2]->front =NULL;
        Q[2]->rear =NULL;
    }

    return 1;
}

int 
_env_STRIDE
(heap_stride * minHeap, cpu_state * cpu_st, tasklist * joblist)
{
    //farness 보기 위해서 ABC 를 0에 모두 fork시키고
    //각각의 비율을 3:2:1 로 미리 가정해 보았다
    //stride는 task strct상에 자신의 vruntime을 가지고 시작해야함
    int t =0;
    int i =1;
    //task_strct * prev_task =NULL;
    task_strct * curr_task =NULL;
    for(t=0; 1; t++){
        //1. t에 따른 joblist에서 빼온다
        do{
            task_strct *new_task = _Module_fork(joblist,t); //프로세스 생성 Heap 에 생성
            if(new_task == NULL)
                break;
            else joblist = joblist->next_item;
            //index = update_bitmap(new_task, ); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
            new_task->id=i++;
            set_STRIDE_task(new_task);
            //vruntime 을 설정하고(0) Heap에 넣는다
            new_task->vruntime=0;
            printf(" Process pid %c stride %d\n", new_task->pid, new_task->STRIDE);
            addMinHeap(new_task , minHeap);
            new_task =NULL;
        }while(1);

        if(Stride_endWorkload(minHeap, cpu_st, joblist) >0)
            break;//1. joblist 2. minheap empty 3. cpu state == cpudone
        
        //2. schedule: 가장 작은 vruntime을 pop한다
        //if(){
        curr_task = schedule_STRIDE(minHeap);
        //}
           
        //3. cpu에서 수행
        if(curr_task != NULL){
            cpu(cpu_st, curr_task, t);

            if(cpu_st->cpu_state == CPU_EMPTY ){
                //context_save()
                curr_task->state == TASK_DONE;
                curr_task->fin_time = t;
            }
            else{ 
                //4, task->vruntime += task->vruntime
                //5  Minheapfiy  
                curr_task->vruntime += curr_task->STRIDE;
                addMinHeap(curr_task , minHeap);
            }
        }else{
            printf(" end:%d ",t);
            break;
        }
       

    }
    //assert
    return 1; 

}
// //#Endif Scheduling Source code.
int 
IsEmpty(sched_queue ** Q) {
    if(isEmpty(Q[0]))
        if(isEmpty(Q[1]))
            if(isEmpty(Q[2]))
                return 1;
    return 0;
}
void showList(tasklist * head)
{
    int i;
    float res =0;
    float tur =0;
    for(i=0; head != NULL; i++){
        res += head->current->res_time;
        tur += head->current->fin_time - head->arriv_T;
        head = head->next_item;
    }
    res = res / i;
    tur = tur / i;
    printf(" avg response_t :  %.2f\n" ,res);
    printf(" avg turnaround_t : %.2f\n",tur);
}
void cpu
(cpu_state * state , task_strct * task, int timestamp)
{
    state->cpu_state = CPU_RUNNING;
    task->state = TASK_RUNNING;
    if(task->spent_time == 0)
        task->res_time = timestamp;    
    task->spent_time +=1 ;
    if(task->spent_time == task->total_time){
        state->cpu_state = CPU_EMPTY;
        context_save(task);
    }
    footprint[task->id][timestamp+1] = 1;
    printf("%c", task->pid); 
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

List * 
_init_STRIDE_ABC()
{
	List * ret = malloc(sizeof(List));
	char * scenario[3] = {
		"A 0 12", //3
		"B 0 8", //2
		"C 0 4" //1
	};
	int ratio =1;
    int i;
	for(i=0; i<3; i++){
        tasklist * item = (tasklist * )malloc(sizeof(tasklist));
        item->arriv_T=0;
        item->current =NULL;
        item->next_item=NULL;
        //tasklist

        task_strct * task = (task_strct *)malloc(sizeof(task_strct));
        do_fork(scenario[i],task);
		task->tickets = MAX_TICKETS / ratio;
		ratio +=1;
        //task 생성, tickets : 150 100 50
        item->current = task;
        item->arriv_T = task->arr_time;
        
        addList(ret, item);
        item =item->next_item;
    }
	return ret;
}

heap_stride *
init_stride_heap(int n)
{
	heap_stride * hnew = malloc(sizeof(heap_stride));
	hnew->add_point =0;
	int i;
	int maxsize =  (1<<n) -1 ; //8 -1

	hnew->maxsize = maxsize;
	hnew->arrOfTaskStrct = malloc(sizeof(task_strct *) * maxsize);
	for(i=0; i<maxsize; i++)
		hnew->arrOfTaskStrct[i] = malloc(sizeof(task_strct));
    
	return hnew;
}
int
set_STRIDE_task(task_strct * new)
{
	int base = new->total_time ;
	char buf[64];

	if(CM % base){
		sprintf(buf,"Common Multiple err : %c->total = %d\n", new->pid, base);
		write(STDERR_FILENO,buf,40);
		exit(-1);
	}
	
	//1. set stride based on Common Multiple
	// new->STRIDE  = CM / lottery?
	//fairness 를 지켜주려면 같은 시간 동안 대부분의 job들이 비슷한 시간에 끝나도록 함
	
	new->STRIDE = CM / new->tickets;
    return 1;
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
    if (enqueue(Q[task->sched_priority],task) >0) //BUFFER OVERFLOW 취약점
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

task_strct * dequeue(sched_queue * rq)
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

int
addMinHeap(task_strct  * task, heap_stride * heap)
{
	int addpoint = heap->add_point;

	if(addpoint > heap->maxsize)
		return -1;
	heap->arrOfTaskStrct[addpoint] = task;
	heap->add_point = addpoint+1;
	int i=addpoint;
	while
	(i>0 && heap->arrOfTaskStrct[i]->vruntime < heap->arrOfTaskStrct[(i-1)/2]->vruntime)
	//조건문 해석 : minheap에서 자식 노드가 부모노드보다 작다면~ (&& i가 root노드가 아니면~)
	{
		//swap [i] and [i/2]
		task_strct * temp = heap->arrOfTaskStrct[i];
		heap->arrOfTaskStrct[i] = heap->arrOfTaskStrct[(i-1)/2];
		heap->arrOfTaskStrct[(i-1)/2] =temp;
		i = (i-1)/2;
	}
} 
int 
isheapEmpty(heap_stride * heap)
{
    int max_size = heap->maxsize;
    task_strct ** arr = heap->arrOfTaskStrct;
    int i;
    for(i=0; i<max_size; i++)
        if(arr[i] != NULL)
            return 0;
    
    return 1;


}
int
Stride_endWorkload(heap_stride * minheap , cpu_state * cpu , tasklist * joblist)
{
	if(joblist==NULL)
		if (cpu->cpu_state == CPU_EMPTY)
			if(isheapEmpty(minheap))
				return 1;
	
	return 0;
}
task_strct *
schedule_STRIDE(heap_stride * minheap)
{
	task_strct * ret = minheap->arrOfTaskStrct[0];
	minheap->add_point -= 1;
	minheap->arrOfTaskStrct[0] = minheap->arrOfTaskStrct[minheap->add_point];
	minheap->arrOfTaskStrct[minheap->add_point] =NULL;
	MinHeapDown(minheap);
	return ret;
}
int MinHeapDown(heap_stride *heap)
//heapify
{
    task_strct ** arr = heap->arrOfTaskStrct;
    task_strct *tmp;
    int limit = heap->add_point;
    int i=0; //root to downward
    int k;
    int left = (i + 1) * 2 - 1;
    int right = (i + 1) * 2;
    while (left <limit && right <limit)
    {
        k = (arr[left]->vruntime > arr[right]->vruntime) ? right:left;
        if(arr[i]->vruntime > arr[k]->vruntime){
            tmp = arr[i];
            arr[i] =arr[k];
            arr[k] =tmp;
        }
            //swap arr[i] arr[k]
        i =k;
        left = (i + 1)*2 -1;
        right = (i + 1)*2;
    }
    if(right == limit){
        //right가 limit이면 left는 존재한다는 뜻
        if(arr[i]->vruntime > arr[left]->vruntime){
            tmp = arr[i];
            arr[i] =arr[left];
            arr[left] =tmp;
            i = left;
        }
    }

    return i;
}



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
            if( (rq->my_level - Q[0]->my_level )==0 )
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


