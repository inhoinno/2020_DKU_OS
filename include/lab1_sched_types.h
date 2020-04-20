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
#define MAX_TICKETS 300
#define CM 15000
typedef struct Task_strct task_strct;
typedef struct Sched_queue sched_queue;
typedef struct Cpu_state cpu_state;
typedef struct Tasklist tasklist;
typedef struct List List;
typedef struct STR_Heap heap_stride;

typedef struct List * pList;
typedef task_strct * ptask_strct;
typedef sched_queue * psched_queue;
typedef psched_queue ** ppsched_queue;
typedef cpu_state * pcpu_st;
typedef tasklist * ptasklist;
typedef heap_stride * pheap_stride;


// typedef io_context * poi_context;
/*
	minHeap 자료구조 설명부분
	minHeap []
	l : left왼쪽 자식
	r : right오른쪽자식
	ll : 루트의 왼쪽자식의 왼쪽자식(4)
	..

	[0] [1] [2] [3] [4] [5] [6]
    root l   r  ll  lr
*/


#define CPU_EMPTY 0 
#define CPU_RUNNING 1
typedef struct Cpu_state{
	int cpu_state;
	//int cpu_no; for cache affinity , then should check cpu# to scheduling
	//u64 cpu_running; 얼마나 cpu가 수행되었는지에 대해
	//Exception		Divide_By_Zero
	//Exception		Interrupt_Bit
	//...
}cpu_state;

// typedef struct io_context{
// 	char			*str;
// 	//io작업은 IO수행중이라는 메세지를 띄움
// }io_context;


typedef struct Sched_queue{
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
		#define HIGHEST_PRIORITY 0
		#define LOWEST_PRIORITY 2
		int				my_level;
	#endif

}sched_queue;

typedef struct List{
    tasklist * head;
    tasklist * last;
}List;

typedef struct Tasklist{
    int arriv_T;
	//arr time for current pointing task
    task_strct * current;
    tasklist * next_item;

}tasklist;
#define TASK_DONE 0
#define TASK_RUNNING 1
#define TASK_READY 2

typedef struct Task_strct{
	char 			pid;// A B C D and E
	int 			id;
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
	
	task_strct *	next; //latter
	task_strct * 	prev; //former
	//ttime //Time the task terminate
	//io_context 		*IO; 
    sched_queue		*myrq;
	int				sched_priority;
	// some struct for I/OOperation
	//#ifdef MLFQ_SCHED 
		//int 		_levels_of_feedback_queues;
		//멀티레벨Queue 의 자료구조 -> 배열or 리스트
		//struct sched_queue	*top;
		//struct sched_queue 	*present;
		//struct sched_queue		*where_is_my_Q;
		//내 스케쥴Queue의 위치... 이게 필요할까?
		//int 		pqi;//present-queue-index
	//#endif

	#ifdef STRIDE_SCHED
		int 		vruntime;
		int 		STRIDE; 
		int			tickets;
		//or (usigned long) , (unsigned int)
	#endif	 
}task_strct;

typedef struct STR_Heap{
	int maxsize;
	int add_point;
	task_strct ** arrOfTaskStrct;
}heap_stride;




sched_queue * init_sched(int policy, int t);
sched_queue ** init_bitmap();
cpu_state* init_cpu();
int init_workload(char ** scenario, task_strct * ret);
/*
 * You need to Declare functions in  here
 */
int MLFQ_boosting(sched_queue ** Q  );
int Run_workload
(const char * scenario[] , int scenario_length ,int sched_policy);

int _env_FCFS
(sched_queue * rq, cpu_state * cpu_st, tasklist * joblist);

int _env_RR
(sched_queue * rq, cpu_state * cpu_st, tasklist * joblist);

int _env_MLFQ
(sched_queue *Q [] , cpu_state * cpu_st, tasklist * joblist);

int _env_STRIDE
(heap_stride * minheap, cpu_state * cpu , tasklist * joblist);



int 
IsEmpty(sched_queue ** Q);
void showList(tasklist * head);

void cpu
(cpu_state * cpu , task_strct * task,int timestamp);

void lower_priority(task_strct * _task);

int time_to_schedule
(int currslice, cpu_state* cpu , sched_queue * rq);

int EndWorkload
(sched_queue * Q[], cpu_state * cpu, tasklist*joblist);

int endWorkload
(sched_queue * q, cpu_state * cpu , tasklist * joblist);

int time_to_fork
(char ** workload , int length, int time , int* index);

// task_strct *
// do_fork
// (char ** workload , int * step);
//start 
int
init_tasklist
(List * return_list, const char * scenario[], int wlength);

int 
do_fork(const char * str, task_strct * t);

int
MinHeapDown(heap_stride * heap);


int 
addList( List * L, tasklist * tl);

task_strct *
_Module_fork(tasklist * joblist, int t);

//STRIDE
List * 
_init_STRIDE_ABC();

heap_stride * 
init_stride_heap(int n);

int
set_STRIDE_task(task_strct * new);

int
addMinHeap(task_strct  * task, heap_stride * heap);
int 
isheapEmpty(heap_stride * heap);
int
Stride_endWorkload
(heap_stride * minheap , cpu_state * cpu , tasklist * joblist);
task_strct *
schedule_STRIDE(heap_stride * minheap);
int MinHeapDown(heap_stride *heap);

//STRIDE end
//end
sched_queue* init_sched
(int policy, int slice);

sched_queue ** 
init_bitmap();

cpu_state * 
init_cpu();

sched_queue *
SelectScheduler(sched_queue ** Q);

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



#endif /* LAB1_HEADER_H*/



