/* Library for Queue Implementation */

#include"queue.h"

queue* new_queue(){
	queue* q = malloc(sizeof(queue));
	if(!q){
		perror("Out of memory");
		return q;
	}
	q->head = NULL;
	q->tail = NULL;
	return q;
}

node* new_node(int value){
	node* newNode = malloc(sizeof(node));
	if(!newNode){
		perror("Out of memory");
		return newNode;
	}
	newNode->value = value;
	newNode->next = NULL;
	return newNode;
}

void enqueue(queue* q, int value){
	if (!q){
		return;
	}

	node* newNode = new_node(value);
	if(newNode){
		if(!q->head){
			q->head = newNode;
			q->tail = newNode;
		} else {
			q->tail->next = newNode;
			q->tail = newNode;
		}
	} else {
		return;
	}
}

node* dequeue(queue* q){
	if(!q || !q->head){
		return NULL;
	}
	node* temp = q->head;
	q->head = temp->next;
	return temp;
}

bool in_list(queue* list, int value){
	if(!list){
		return false;
	}

	node* current = list->head;
	while(current){
		if(current->value == value){
			return true;
		}
		current = current->next;
	}
	return false;
}

void destroy_queue(queue* q){
	if(!q){
		return;
	}
	
	node* current = q->head;
	while(current){
		current = current->next;
		q->head->next = NULL;
		free(q->head);
		q->head = current;
	}
	q->tail = NULL;
	free(q);
}
