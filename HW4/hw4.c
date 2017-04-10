/* Joe Wong
   02216247
   Programming assignment 4: Elevator Problem
   Created 4/9/2017
*/

#include"queue.h"
queue* BFS(int,int,int,int,int);
void getInput(int*,int*,int*,int*,int*);
void printList(queue*);


void display(){
	puts("Use the form '<floors> <start> <goal> <up> <down>'");
	puts("Please enter a command");
}

queue* BFS(int floors, int start, int goal, int up, int down){
	queue* q = new_queue();		//queue for BFS
	queue* list = new_queue();	//list for printing
	enqueue(q,start);
	node* current = NULL;

	while(q->head){
		current = dequeue(q);
		enqueue(list, current->value);
		if (current->value == goal){
			destroy_queue(q);
			free(current);
			return list;
		}
		
		if(current->value < goal && current->value + up <= floors && !in_list(list, current->value + up)){
			enqueue(q, current->value + up);
		} else if (current->value - down > 0 && !in_list(list, current->value - down)){
			enqueue(q, current->value - down);
		}
		free(current);
	}
	destroy_queue(q);
	destroy_queue(list);
	return NULL;
}

void getInput(int* floors, int* start, int* goal, int* up, int* down){
	char buffer[100] = {0};
	fgets(buffer, 100, stdin);
	while(sscanf(buffer, "%d %d %d %d %d",floors, start, goal, up, down) != 5){
		puts("That is not a valid input");
		printf("Please try again: ");
		fgets(buffer, 100, stdin);
	}
}

void printList(queue* list){
	node* current = list->head;
	while(current->next){
		printf("%d => ",current->value);
		current = current->next;
	}
	printf("%d\n", current->value);
}

int main(){
	int floors = -1, start = 0, goal = 0, up = 0, down = 0;
	do{
		display();
		getInput(&floors, &start, &goal, &up, &down);
	} while(floors <= 0 || start > floors || goal > floors);
	
	queue* list = BFS(floors, start, goal, up, down);
	if(list){
		puts("Floors travelled in order:");
		printList(list);
	} else {
		puts("Use the stairs.");
	}
	destroy_queue(list);
	return 78;
}
