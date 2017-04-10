/* Utility functions used in heapsort program */
#include"heapsort.h"

/* Utility functions to convert array indexes for Heap */
int parent(int i){
	return i/2;
}

int left(int i){
	return 2*i;
}

int right(int i){
	return 2*i+1;
}

/* Utility function to check the file(s) in the command line args */
int check_file(char* filename){
	FILE* fptr = fopen(filename, "r");
	if (fptr == NULL){
		printf("Could not find the file %s\n", filename);
		return -1;
	}
	fclose(fptr);
	return 0;
}

/* Utility function calculates total wait */
int total_wait(node** array, int size){
	int total = 0, i = 0, j = size;
	for (i = 0; i < size; i++){
		total += array[i]->wait * j--;
	}
	return total;
}


void free_list(node* list){
	node* next = list;
	while (list != NULL){
		next = next->right;
		free(list->name);
		free(list);
		list = next;
	}
}

/* Utility function to output array contents to file */
void out_to_file(FILE* fptr, node** array, int size){
	int i = 0;
	for (i=0; i < size; ++i){
		fprintf (fptr,"%s	%d\n", array[i]->name, array[i]->wait);
	}
	fprintf (fptr, "\n");
}
