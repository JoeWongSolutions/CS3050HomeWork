/* Joe Wong
 * jlwxz8
 * 02216247
 * Created 1/25/2017
 * Heapsort Program for CS3050
 * Objective: Learn to implement a heapsort on simple customer wait list
*/

#include"heapsort.h"

node* build_list(char* filename, int* count){
/* Function for reading a file and building a linked list for storing those files 
 * Note that this linked list will not change, the array built later will sort address pointers
 * The count is passed to record the size that the array needs to be	
*/	
	node *head = NULL, *current = NULL;
	FILE* fptr = fopen(filename,"r");
	char* temp = malloc(sizeof(char)*MAXLINE);
		/* Error checking for memory */
		if (temp == NULL){
			puts("You are out of memory");
			free(temp);
			return head;
		}
	/* Loop for reading in each line in file */
	while (fgets(temp, MAXLINE, fptr) != NULL){
		
		/* Create a new node */
		node* new_node = malloc(sizeof(node));
		
		/* Error checking for memory */
			if (new_node == NULL){
				puts("You are out of memory");
				free(new_node);
				return head;
			}
		
		/* allocate space for the name in the new node*/
		new_node->name = malloc(sizeof(char)*MAXNAME);
		
		/* Error checking for memory */
			if (new_node->name == NULL){
				puts("You are out of memory");
				free(new_node->name);
				free(new_node);
				return head;
			}
		
		/* Scan the temp array to see if data is usable, insert data to new node */
		if(sscanf(temp,"%s %d", new_node->name, &new_node->wait) < 2){
	
		/* Error checking for file inconsistencies */
			printf("File has inconsistent formatting on line %d, skipping to next line\n", *count);
			free(new_node->name);
			free(new_node);
			continue;			
		}

	/* Populate the linked list with new node */
	if (head == NULL){
		head = new_node;
		current = head;
	}
	else {
		current->right = new_node;
		current = new_node;
	}
	*count += 1;
	}
	fclose(fptr);
	free(temp);
	return head;
}

node** build_array(node* head, int count){
/* This function creates and populates an array of node pointers */
	node** array = malloc(sizeof(node*)*count);
	node* current = head;
	int i;
	for(i = 0; i < count; ++i){
		array[i] = current;
		current = current->right;
	}
	return array;
}

void max_heapify(node** array, int length, int i, sortBy key){
	
	int l = left(i), r = right(i), largest = 0;
	node* temp = NULL;
	
	if (key == NAME){
		if (l < length && strcmp(array[l]->name, array[i]->name) > 0)
			largest = l;
		else
			largest = i;
		if (r < length && strcmp(array[r]->name, array[largest]->name) > 0)
			largest = r;
		/* Base case for recursion */
		if (largest == i)
			return;
	}
	else if(key == WAIT){

		/* This part basically finds the largest wait time and puts the index into temporary variable */
		if (l < length && array[l]->wait > array[i]->wait)
			largest = l;
		else
			largest = i;
		if (r < length && array[r]->wait > array[largest]->wait)
			largest = r;
		/* Base case for recursion */
		if (largest == i)
			return;
	}

	/* Swap the contents of largest index with parent index */
	temp = array[i];
	array[i] = array[largest];
	array[largest] = temp;
	
	max_heapify(array, length, largest, key);
}

void heap_sort(node **array, int last, sortBy key){
	node* temp = NULL;

	while(last > 0){
		/* Swap root element with last array element */
		temp = *array;
		*array = array[last];
		array[last] = temp;

		/* Rebuild heap at root index*/
		max_heapify(array, last, 0, key);

		/* Need to remove last element from array by decrementing last */
		last -= 1;
	}
}


int main (int argc, char** argv){

	/* Standard error checks */
	if (argc != 3){
		puts("Incorrect usage. Please use ./heapsort <input-file> <output-file>");
		return 0;
	}

	if (check_file(argv[1]) == EOF)
		return 0;

	if (check_file(argv[2]) == EOF)
		puts("A new output file will be created");
	else
		puts("The output file already exists and will be overwritten");


	int count = 0, i = 0;
	node* list = build_list(argv[1],&count);
	node** heap = build_array(list,count);
	FILE* outputFile = fopen(argv[2],"w");

#ifdef DEBUG
			/* print for debugging */
			puts("starting array");
			for (i = 0; i < count; i++){
				printf("%s	%d\n", heap[i]->name, heap[i]->wait);
			}
#endif

	/* Heapify the array by name*/
	for (i = count/2; i >= 0; i--){
		max_heapify(heap, count, i, NAME);
	}

#ifdef DEBUG
			/* print for debugging */
			puts("\n\nHeapified array");
			for (i = 0; i < count; i++){
				printf("%s	%d\n", heap[i]->name, heap[i]->wait);
			}
#endif
	heap_sort(heap, count - 1, NAME);
	
	/* Print the sorted names to output file */
	fprintf(outputFile,"We have the following people in alphabetical order:\n");
	out_to_file(outputFile, heap, count);

#ifdef DEBUG
			/* print for debugging */
			puts("\n\nSorted array");
			for (i = 0; i < count; i++){
				printf("%s	%d\n", heap[i]->name, heap[i]->wait);
			}
#endif

	/* Heapify the array by wait*/
	for (i = count/2; i >= 0; i--){
		max_heapify(heap, count, i, WAIT);
	}

#ifdef DEBUG
			/* print for debugging */
			puts("\n\nHeapified array");
			for (i = 0; i < count; i++){
				printf("%s	%d\n", heap[i]->name, heap[i]->wait);
			}
#endif

	heap_sort(heap, count - 1, WAIT);

	/* Print the sorted names to output file */
	fprintf(outputFile,"They should be serviced in this order:\n");
	out_to_file(outputFile, heap, count);

#ifdef DEBUG
			/* print for debugging */
			puts("\n\nSorted array");
			for (i = 0; i < count; i++){
				printf("%s	%d\n", heap[i]->name, heap[i]->wait);
			}
#endif

	fprintf(outputFile,"\nThe total wait time for all customers will be %d minutes.\n", total_wait(heap, count));
	
	puts("Program executed successfully.");
	puts("Terminating...");

free_list(list);
free(heap);
fclose(outputFile);
return 0;
}
