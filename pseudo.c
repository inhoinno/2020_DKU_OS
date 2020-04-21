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
        if( time_to_schedule(tempslice, cpu_st, rq) || isTopQueue(Q,rq) ){ 
            //선점 조건1. CPU EMPTY
            //2. myrq의 시간 조건 만료
            //rq가 topQ가 아니라면 true return(topQ가 empty가 아닐때)     
            rq = (sched_queue *)SelectScheduler(Q);
            curr_task=schedule(rq); 
            if(curr_task == NULL){
                write(STDERR_FILENO, "SCHEDULE : Q is empty dequeue error\n", 45);
                exit(-1);
            }
            time_slice = rq->time_slice;
            tempslice =0;
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
/*3.2*/     cpu(cpu_st, curr_task,t);tempslice++;             
/*4 조건을 확인하고 lower -> enqueue*/ 
        if( !IsEmpty(Q) ){//Q가 모두 empty라면 task를 계속 수행함
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
    }
    
    if(Assert(Q,cpu_st) <0) 
        return -1;
    
    //SUCCESS RR
    return 1; 

}

t=0;
while( t <7){
    //1. 프로세스 종료
    //.remain = 수행시간 .protime = 프로세스 수행할 시간
        if (pro[k].remain >= pro[k].pro_time)
            //end process
            //해당 인덱스로 k가 가지 말아야함
        else 
            //아직 종료 x
            pro[k].remain +=1;
    
    //2. 찍기
    check[pro[k].id][t]

    //3. 최소 찾기
    pro[k].pass_value += pro[k].stride
    for(){
        //최소 pass_value 를 찾고 그 인덱스를 k에 저장
        //k 에 인덱스 정보
    }
    t++;

}















































"A      0   3"
 ^pid   ^a  ^run
"B 2 6"
"A 0 3"
-> pid A
-> arrtime 0
-> total time 3
typedef struct Tasklist tasklist;
typedef struct List List;
typedef tasklist * ptasklist;
typedef struct List * pList;

typedef struct List{
    tasklist * head;
    tasklist * last;
}List;

typedef struct Tasklist{
    int arriv_T;//arr time for current pointing task
    task_strct * current;
    tasklist * next_item;

}tasklist;

int
init_tasklist
(List * return_list, const char * scenario[], int wlength)
{
    //초기화 하고 왔다고 가정하고 
    int i;
    tasklist * item = (tasklist * )malloc(sizeof(tasklist));
    task_strct * task = (task_strct)malloc(sizeof(task_strct));
    
    for(i=0; i<wlength; i++){
        do_fork(scenario[i],task);
        //task 생성
        item -> current = task;
        item->arriv_T = task->arr_time;
        addList(return_list, item);
        item =item->next_item;
    }
    return 0;


}
int do_fork(char * str, task_strct * t){
    if(str != NULL){
        t->pid = str[0];
        t->arr_time = str[2];
        t->total_time = str[4];
        return 1;
    }
    else return -1;
}
int 
addList( List * L, tasklist * tl){
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
    if((joblist->arriv_T - t) == 0)
        ret=joblist->current;
    return ret;
}


TODO
workload
length

int Run_workload(char * scenario , int scenario_length ,int sched_policy);
int _env_FCFS
(sched_queue * rq, cpu_state * cpu_st, char * workload);
int 
_env_RR
(sched_queue * rq, cpu_state * cpu_st);

int 
_env_MLFQ
(sched_queue *Q [] , cpu_state * cpu_st);
 
void cpu(cpu_state * state , task_strct * task, int timestamp);
int EndWorkload(sched_queue * Q[], cpu_state * cpu);
int endWorkload(sched_queue * q, cpu_state * cpu);
int time_to_fork(char * workload [], int length, int time , int* index);
task_strct * 
do_fork
(char * workload[] , int length, int * step ,int sched_policy);
sched_queue* init_sched(int policy, int slice);
sched_queue ** init_bitmap();
cpu_state * init_cpu();

int //return type?
Enqueue(sched_queue * Q[], task_strct * task);
int //return type?
enqueue(sched_queue * rq , task_strct * task);
task_strct* dequeue(sched_queue * rq);
task_strct* schedule(sched_queue * rq);
int context_save(task_strct * before_task);
int isEmpty(sched_queue * rq);
int isTopQueue(sched_queue ** Q, sched_queue * rq);
int s_assert(cpu_state * cpu_st, sched_queue * rq);
int Assert(sched_queue * Q[], cpu_state * cpu);


 case : ROUND_ROBIN_SCHED
            // 1. 스케쥴러가 수행 되어야 한다면(time out) -> schedule, dequeue
            // 2. 스케쥴러가 지정한 태스크를 cpu가 수행, cpu(curr)
            // 3. 태스크가 종료되었는지, 아니면 문맥교환 해야하는지 check 프로세스의 state변경  
            //currq = SelectRunqueue(); MLFQ
            if(time_to_schedule(tempslice, cpu_state ,currq)){      //1
                curr_task=schdule(currq); tempslice =0;
            }
            cpu_state = cpu(curr_task);         //2                  
            
            tempslice++;
            
            //3
            if(cpu_state = TASK_DONE || tempslice == currq->){
                context_save(curr_task);
                if(curr_task->state == TASK_DONE){
                    tempslice=0;
                }
            }
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
            
            /*typedef struct cpu_state{
                task_strct *    run;
                int             state
            } */


int 
_env_MLFQ
(sched_queue *Q [] , cpu_state * cpu_st, char * workload [] )
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
/*1*/   int arriv = time_to_fork(workload [],length ,t , step);
        while(arriv != 0){
            new_task = do_fork(workload[] , length, step ,sched_policy); //프로세스 생성 Heap 에 생성
            index = update_bitmap(new_task, Q[]); // 프로세스 생성에 대해 비트맵 갱신(사실은 Q맵)
            new_task->priority = index; // 프로세스의 우선순위 결정
            Enqueue(Q,new_task); //해당 우선순위Q에 enqueue;
            arriv--;
            new_task =NULL;
        }// while문 안에서는 실제로 shell이 어느정도 하는 일을 한다

        //2 : 기존의 태스크 확인
/*2*/   if(prev_task != NULL){
            if(prev_task -> state != TASK_DONE){
                //prev_task->priority += 1;
                if()//!(prev_task는 현재 큐에서의 시간을 다 썼다)
                    lower_priority(prev_task);
                Enqueue(Q, prev_task); //enqueue to Q by task's priority
                prev_task=NULL;
            }else{
                //prev_task is done
                prev_task =NULL;
            }
        }
        
        if(EndWorkload(Q, cpu_st)) break;
        //QIsEmpty, cpu_st->cpu_state == 0
//3
/*3.1*/ //1. is Top Q empty?
            //1. is current rq empty?
                //yes then 
            //2. else then
                //schedule it (dequeue)
    
        if( time_to_schedule(tempslice, cpu_st, rq) || rq->my_level != TopQueue(bitmap) ){      
            rq = SelectScheduler(Q);//think about bitmap hereb
            curr_task=schdule(rq); 
            time_slice = rq->time_slice;
            tempslice =0;
        }

/*3.2*/ cpu(curr_task,cpu_st);                
        tempslice++;
            
        //3  현재 workload 구현상 for문을 사용해서, 새로운 task가 들어오는걸 t의 갱신으로 알기 때문에 여기에 구현
/*3.3*/ if(cpu_st->cpu_state = TASK_DONE || tempslice == time_slice){
            context_save(curr_task);
            if(curr_task->state == TASK_DONE){
                tempslice=0;
            }
        }

    
    }
    
    if(Assert(Q,cpu_st) <0) 
        return -1;
    
    //SUCCESS RR
    return 1; 

}