#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include "hierarchy.h"

typedef Tree Item;

typedef struct QueueNode{
	Item elem;
	struct QueueNode *next;
}QueueNode;

typedef QueueNode TcelQ;
typedef QueueNode *ACelQ;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

typedef Queue TCoada;
typedef Queue *AQ;

int sizeQ(Queue* q){

	QueueNode* temp = q->front;

	int size = 0;
	while (temp != NULL){
		size++;
		temp = temp->next;
	}
	return size;

}
Queue* createQueue(void){
	// TODO: Cerinta 2
	Queue* TQueue = (Queue*) malloc(sizeof(Queue));
	
	if (TQueue == NULL){
		printf("Error!!");
		exit(1);
	}

	TQueue->front = NULL;
	TQueue->rear = NULL;
	TQueue->size = 0;
	return TQueue;
} 

int isQueueEmpty(Queue *q){
	// TODO: Cerinta 2
	if (q->front == NULL) return 1;
	return 0;
}

void enqueue(Queue *q, Item elem){
	// TODO: Cerinta 2

	if (q->front == NULL){ // coada este vida
		QueueNode* qNode = (QueueNode*) malloc(sizeof(QueueNode));

		if (qNode == NULL){
		printf("Error!!");
		return;
		}

		qNode->elem = elem;
		q->front = qNode;
		q->rear = qNode;
		qNode->next = NULL;
		q->size++;
		return;
	}
	else{

		QueueNode* qNode = (QueueNode*) malloc(sizeof(QueueNode));

		if (qNode == NULL){
		printf("Error!!");
		return;
		}

		qNode->elem = elem;
		qNode->next = NULL;

		if (q->rear != NULL) q->rear->next = qNode;
		q->rear = qNode;
		q->size++;
		return;
	}
	
}

Item front(Queue* q){
	// TODO: Cerinta 2
	
	return q->front->elem;
	
}

void dequeue(Queue* q){
	// TODO: Cerinta 2

	if (isQueueEmpty(q) == 1){ 
		
		return ;
	}

	if (q->front == q->rear){
		QueueNode* auxNode = q->front;
		free(auxNode);
		q->front = NULL;
		q->rear = NULL;
		q->size--;
		return;
	}
	else{
		QueueNode* auxNode = q->front;
		q->front = q->front->next;
		free(auxNode);
		q->size--;
		return;
	}
}
void destroyQueue(Queue *q){
	// TODO: Cerinta 2
	
	if (isQueueEmpty(q) == 1){ // daca coada este vida
		
		return;
	}

	QueueNode* auxNode;
	
	auxNode = q->front;

	while (auxNode != NULL){
		QueueNode* auxNodeNext = auxNode->next;
        free(auxNode);
		auxNode = auxNodeNext;
	}
	free(q);
	return ;
}
#endif
