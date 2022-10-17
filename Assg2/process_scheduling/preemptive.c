#include<stdio.h>
#include<stdlib.h>
// typedef struct Queue {
// 	int front, rear, size;
// 	unsigned capacity;
// 	int* array;
// } Queue;

// Queue* createQueue(unsigned capacity) {
// 	Queue* queue = (Queue *)malloc(sizeof(Queue));
// 	queue->capacity = capacity;
// 	queue->front = queue->size = 0;
// 	queue->rear = capacity - 1;
// 	queue->array = (int *)malloc(queue->capacity*sizeof(int));
// 	return queue;
// }

// int isFull(Queue* queue) {
// 	return (queue->size == queue->capacity);
// }

// int isEmpty(Queue* queue) {
// 	return (queue->size == 0);
// }

// void enqueue(Queue* queue, int item) {
// 	if (isFull(queue)) return;
// 	queue->rear = (queue->rear + 1) % queue->capacity;
// 	queue->array[queue->rear] = item;
// 	queue->size = queue->size + 1;
// }

// int dequeue(Queue* queue) {
// 	if (isEmpty(queue))
// 		return -1;
// 	int item = queue->array[queue->front];
// 	queue->front = (queue->front + 1)
// 				% queue->capacity;
// 	queue->size = queue->size - 1;
// 	return item;
// }

// int front(Queue* queue) {
// 	if (isEmpty(queue))
// 		return -1;
// 	return queue->array[queue->front];
// }

// int rear(Queue* queue) {
// 	if (isEmpty(queue))
// 		return -1;
// 	return queue->array[queue->rear];
// }

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

    freopen("output_pp.txt", "r", stdin);
    freopen("output_preemptive.txt", "w", stdout);

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
    
    //sort by priority
    
    ////// make gant chart now!!

    int completed = 0;
    int time = 0;
    while (completed != no_of_processes){
        int i = 0;
        for (; i < no_of_processes; i++){
            if (ps[i].AT <= time){
                ps[i].bt[ps[i].ptr] --;

                printf("Time Stamp: %d, executed: %d\n", time, ps[i].id);
                if (ps[i].bt[ps[i].ptr] == 0){
                    if (ps[i].bt[(ps[i].ptr)+1] == -1 || ps[i].bt[(ps[i].ptr)+2] == -1){
                        completed++;
                        sum_tt += time;
                        ps[i].AT = __INT_MAX__;
                    } else {
                        int io_time = ps[i].bt[(ps[i].ptr)+1];
                        ps[i].AT = time + io_time;
                        ps[i].ptr += 2;
                    }
                }
                break;
            }
        }
        if (i == no_of_processes){
            printf("Time Stamp: %d: Idle CPU\n", time);
        }
        time++;
    }

    printf("Average Waiting Time: %f\n", ((float)(sum_tt-sum_wt))/no_of_processes);
    printf("Average Turnaround Time: %f\n", ((float)sum_tt)/no_of_processes);
}