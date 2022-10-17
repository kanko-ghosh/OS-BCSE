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
