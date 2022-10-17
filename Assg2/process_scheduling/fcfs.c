#include<stdio.h>
#include<stdlib.h>
typedef struct Queue {
	int front, rear, size;
	unsigned capacity;
	int* array;
} Queue;

Queue* createQueue(unsigned capacity) {
	Queue* queue = (Queue *)malloc(sizeof(Queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;

	queue->rear = capacity - 1;
	queue->array = (int *)malloc(queue->capacity*sizeof(int));
	return queue;
}

int isFull(Queue* queue) {
	return (queue->size == queue->capacity);
}

int isEmpty(Queue* queue) {
	return (queue->size == 0);
}

void enqueue(Queue* queue, int item) {
	if (isFull(queue)) return;
	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->array[queue->rear] = item;
	queue->size = queue->size + 1;
}

int dequeue(Queue* queue) {
	if (isEmpty(queue))
		return -1;
	int item = queue->array[queue->front];
	queue->front = (queue->front + 1)
				% queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

int front(Queue* queue) {
	if (isEmpty(queue))
		return -1;
	return queue->array[queue->front];
}

int rear(Queue* queue) {
	if (isEmpty(queue))
		return -1;
	return queue->array[queue->rear];
}

typedef struct Process {
    int id;
    int priority;
    int AT;
    int bt[100];

    int ptr;
} Process;




int main(){

	int sum_wt = 0;
	int sum_tt = 0;	

    freopen("output.txt", "r", stdin);
    freopen("output_fcfs.txt", "w", stdout);
    

    int no_of_processes;
    scanf("%d", &no_of_processes);

    Process ps[no_of_processes];
    for (int i = 0; i < no_of_processes; i++){
        scanf("%d", &ps[i].id); 
        scanf("%d", &ps[i].priority); 
        scanf("%d", &ps[i].AT); 
        
        sum_tt -= ps[i].AT;
        
        int cnt = -1;
        do {
            cnt++;
            int temp;
            scanf("%d", &temp);
            ps[i].bt[cnt] = temp;
            sum_wt -= temp;
            
        } while (ps[i].bt[cnt] != -1);
        ps[i].ptr = 0;
    }


    ////// input done

    ////// make gant chart now!!

    Queue * FCFS = createQueue(no_of_processes);

    for (int i = 0; i < no_of_processes; i++){
        if (ps[i].AT == 0) {
            enqueue(FCFS, i);
        }
    }

    int completed = 0;
    int time = 0;
    while (completed != no_of_processes){

        if (isEmpty(FCFS)){
            printf("Time Stamp: %d: Idle CPU\n", time);
            time++;
            continue;
        }

        int pid = front(FCFS);
        ps[pid].bt[ps[pid].ptr] --;
        printf("Time Stamp: %d, executed: %d\n", time, pid);

        if (ps[pid].bt[ps[pid].ptr] == 0){
            dequeue(FCFS);
            if (ps[pid].bt[(ps[pid].ptr)+1] == -1 || ps[pid].bt[(ps[pid].ptr)+2] == -1){
                completed++;
                printf("Completed %d\n", completed);
                sum_tt += time;
                ps[pid].AT = __INT_MAX__;
            } else {
                int io_time = ps[pid].bt[(ps[pid].ptr)+1];
                ps[pid].AT = time + io_time;
                ps[pid].ptr+=2;
            }
        }

        time++;

        for (int i = 0; i < no_of_processes; i++){
            if (ps[i].AT == time) {
                enqueue(FCFS, i);
            }
        }

    }
    
    
    printf("Average Waiting Time: %f\n", ((float)(sum_tt-sum_wt))/no_of_processes);
    printf("Average Turnaround Time: %f\n", ((float)sum_tt)/no_of_processes);
}