
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXLINE 100
#define MAXNAME 30

typedef enum sortBy {NAME,WAIT}sortBy;

typedef struct node{
	char* name;
	int wait;
	struct node* right;
}node;

/* Function prototypes */

int parent(int i);
int left(int i);
int right(int i);
int check_file(char* filename);
node* build_list(char* filename, int* count);
node** build_array(node* head, int count);
void max_heapify(node **array, int length, int i, sortBy key);
void heap_sort(node **array, int length, sortBy key);
int total_wait(node **array, int size);
void free_list(node* list);
void out_to_file(FILE* fptr, node** array, int size);
