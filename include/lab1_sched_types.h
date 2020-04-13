/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32152332
*	    Student name : 송인호
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H
//Header & funcs & structs
 
#define FCFS_SCHED 1
#define ROUND_ROBIN_SCHED 2
#define MLFQ_SCHED 3
#define STRIDE_SCHED 4 

struct io_context;
struct task_strct;
struct sched_queue;
struct cpu_state;

typedef io_context * poi_context;
typedef task_strct * ptask_strct;
typedef sched_queue * psched_queue;
typedef psched_queue ** ppsched_queue;
typedef cpu_state * pcpu_st;


typedef struct cpu_state{
	int cpu_state;
	//int cpu_no; for cache affinity , then should check cpu# to scheduling
	//u64 cpu_running; 얼마나 cpu가 수행되었는지에 대해
	//Exception		Divide_By_Zero
	//Exception		Interrupt_Bit
	//...
}cpu_state;

typedef struct io_context{
	char			*str;
	//io작업은 IO수행중이라는 메세지를 띄움
}io_context;
typedef struct sched_queue{
	//What if FCFS?
		// normal queue, task_struct *next; 
	//What if RR?
		// context save/restore, 
	//What if MLFQ?
		// Multi Queue.... -> After Processed, level switch
		// different TIME like 2**n ... -> Time slice init should be HERE
	int 			time_slice; //time slice of this queue. it might change when it is MLFQ
	int				policy;
	task_strct		*front;
	task_strct		*rear;
	task_strct		*next_task; //<- Where? queue? task?
	#ifdef MLFQ_SCHED
		int				my_level;
	#endif

}sched_queue;
#define TASK_DONE 0
#define TASK_RUNNING 1
#define TASK_READY 2
typedef struct task_strct{
	char 			pid;// A B C D and E
	int  			state;
	int 			sched_policy;
    //task_strct      *next_sched; //<- Where? queue? task?should it be here? 
	//cpu_state 	*my_cpu
	//void * 		stack;
	
	//Task's cpu time
	int				total_time;
	//Time that task will spent cpu
	int 			spent_time;
	//Time that has used cpu

	//Task's time
	int				arr_time; 
	//Time that task arrived 
    int				res_time;
	//Time that task dispatch time(first cpu use) - arrive time
	int             fin_time;
    //Time that task terminated

	int				qtime; 
	//how much time this task spent in Present Queue(MLFQ)
	
	//ttime //Time the task terminate
	io_context 		*IO; 
    sched_queue		*myrq;
	int				sched_priority;
	// some struct for I/OOperation
	#ifdef MLFQ_SCHED 
		//int 		_levels_of_feedback_queues;
		//멀티레벨Queue 의 자료구조 -> 배열or 리스트
		//struct sched_queue	*top;
		//struct sched_queue 	*present;
		//struct sched_queue		*where_is_my_Q;
		//내 스케쥴Queue의 위치... 이게 필요할까?
		//int 		pqi;//present-queue-index
	#endif

	#ifdef STRIDE_SCHED
		int 		ticket;
		int 		STRIDE; 
		//or (usigned long) , (unsigned int)
	#endif	 
}task_strct;


int init_sched();
int init_task_strct();
int init_workload();
int init_cpu();
int init_io(char * str);
int Working(task_strct * );
int Spin(int );



/*
 * You need to Declare functions in  here
 */


#endif /* LAB1_HEADER_H*/



