

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