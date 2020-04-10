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