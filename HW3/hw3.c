/*
 * Author: Joe Wong
 * 02216247
 * Created: 2/28/2017
 * Programming Assignment 3: Red Black Trees
 * Create a program that will read in a list of comma separated numbers and output the red black tree from top to bottom
*/

#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define T_NIL 0x7FFFFFFF

typedef struct node{	//Red Black Tree nodes
	int key;
	struct node* left;	
	struct node* right;
	struct node* parent;
	char color;
}node;

typedef struct list{	//nodes for queue used in print_breadth_first()
	node* treeNode;
	struct list *next;
}list;

typedef struct queue{
	list* head;
	list* tail;
}queue;


node* build_rb_tree(char* filename, node* t_nil);
node* build_node(int key, node* t_nil);
void left_rotate(node** root, node* x);
void right_rotate(node** root, node* x);
node* rb_insert(node** root, node* new_node);
void rb_insert_fixup(node** root, node* z);
queue* build_queue();
void enqueue(node* t_node, queue* q);
node* dequeue(queue* q);
void print_breadth_first(node* root);
void free_tree(node* root);

int main(int argc, char** argv){
	if(argc != 2){
		puts("Incorrect arguments, use: ./<program> <inputfile>");
		return -1;
	}
	puts("Building Red-Black Tree...\n");
	node* t_nil = build_node(T_NIL, NULL);
	t_nil->color = 'B';
	node* root = build_rb_tree(argv[1], t_nil); 

	//Print the red black tree
	puts("Nodes in the tree from top level to bottom are: ");
	print_breadth_first(root);

	puts("\nPerforming cleanup...\n");
	free_tree(root);
	free(t_nil);
	puts("Exiting the program...");
	return 0;
}

node* build_rb_tree(char* filename, node* t_nil){
	FILE* file = fopen(filename, "r");
	if(!file){
		puts("Could not open the file, please try a different file");
		return NULL;
	}
	char buffer[101] = "";
	const char delim[2] = ",";
	
	//Build the red black tree
	node* root = NULL; //initilize pointer to tree root;
	int key = T_NIL;
	char *temp = NULL;
	while(fgets(buffer, 100, file)){
		temp = strtok(buffer, delim);
		while(temp){
			key = atoi(temp);
			root = rb_insert(&root, build_node(key, t_nil));
			temp = strtok(NULL, delim);
		}
	}

	while(fclose(file));
	return root;
}

node* build_node(int key, node* t_nil){
	if (!key){
		return NULL;
	}
	node *new_node = malloc(sizeof(node));
	if (new_node){
		new_node->key = key;
		new_node->left = t_nil;
		new_node->right = t_nil;
		new_node->parent = NULL;
		new_node->color = 'R';
	}
	return new_node;
}

void left_rotate(node** root, node* x){
	node* y = x->right; //Set y
	//Turn y's left subtree into x's right subtree
	x->right = y->left;
	if(y->left){
		y->left->parent = x;
	}
	//Link x's Parent to y
	y->parent = x->parent;
	if(!x->parent){
		*root = y;
	} else 
		if(x == x->parent->left){
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
	//Put x on y's left
	y->left = x;
	x->parent = y;
}

void right_rotate(node** root, node* x){
	node* y = x->left; //Set y
	//Turn y's right subtree into x's left subtree
	x->left = y->right;
	if(y->right){
		y->right->parent = x;
	}
	//Link x's Parent to y
	y->parent = x->parent;
	if(!x->parent){
		*root = y;
	} else 
		if(x == x->parent->right){
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
	//Put x on y's right
	y->right = x;
	x->parent = y;
}

node* rb_insert(node** root, node* new_node){
	node* y = NULL; //y holds the last non null node
	node* x = *root; //x is a utility pointer indicating current node
	//Traverse to correct position in tree
	while(x && x->key != T_NIL){
		y = x;
		if (new_node->key < x->key){
			x = x->left;
		} else {
			x = x->right;
		}
	}
	//Set new_node's parent to y and set child of y to new_node
	new_node->parent = y;
	if(!y){
		*root = new_node;
	} else {
		if(new_node->key < y->key){
			y->left = new_node;
		} else {
			y->right = new_node;
		}
	}
	rb_insert_fixup(root, new_node);
	return *root;
}

void rb_insert_fixup(node** root, node* z){
	node* y = NULL; //y is the uncle of z
	while(z->parent && z->parent != *root && z->parent->color == 'R'){
		if(z->parent == z->parent->parent->left){ //z's parent is left child
			y = z->parent->parent->right;
			if(y->color == 'R'){			//uncle is red
				z->parent->color = 'B'; 	//case 1
				y->color = 'B';			//case 1
				z->parent->parent->color = 'R'; //case 1
				z = z->parent->parent;
			} else {				//uncle is black
				if(z == z->parent->right){	//z is inner child
					z = z->parent;		//case 2
					left_rotate(root, z);	//case 2
				}
				z->parent->color = 'B';		//case 3
				z->parent->parent->color = 'R'; //case 3
				right_rotate(root, z->parent->parent);
			}
		}
		else {
			y = z->parent->parent->left; //z's parent is right child
			if(y->color == 'R'){			//uncle is red
				z->parent->color = 'B'; 	//case 1
				y->color = 'B';			//case 1
				z->parent->parent->color = 'R'; //case 1
				z = z->parent->parent;
			} else {				//uncle is black
				if(z == z->parent->left){	//z is inner child
					z = z->parent;		//case 2
					right_rotate(root, z);	//case 2
				}
				z->parent->color = 'B';		//case 3
				z->parent->parent->color = 'R'; //case 3
				left_rotate(root, z->parent->parent);
			}
		}
	}
	(*root)->color = 'B';
}

queue* build_queue(){
	queue* q = malloc(sizeof(queue));
	if (!q){
		puts("Not enough memory to create queue");
		return NULL;
	}
	q->head = NULL;
	q->tail = NULL;
	return q;
}

void enqueue(node* t_node, queue* q){
	list* new_node = malloc(sizeof(list));
	if(!new_node){
		puts("Out of memory, could not create queue");
		return;
	}
	new_node->treeNode = t_node;
	new_node->next = NULL;
	if(!q->head){
		q->head = new_node;
		q->tail = q->head;
		return;
	} else {
		q->tail->next = new_node;
		q->tail = new_node;
	}
}

node* dequeue(queue* q){
	if(!q->head){
		return NULL;
	}

	node* r_node = q->head->treeNode;
	list* prev = q->head;

	//Move q's head to next node in list
	q->head = q->head->next;

	//free the old head
	prev->next = NULL;
	prev->treeNode = NULL;
	free(prev);
	return r_node;
}

void print_breadth_first(node* root){
	node* current = root;
	queue* q = build_queue();
	while (current && current->key != T_NIL){
		printf("%d->%c", current->key, current->color);

		//enqueue the left tree node
		if(current->left && current->left->key != T_NIL){
			enqueue(current->left, q);
		}
		//enqueue the right tree node
		if(current->right && current->right->key != T_NIL){
			enqueue(current->right, q);
		}
		//dequeue the next tree node and make it current
		current = dequeue(q);

		//printf formatting
		if(current){
			printf("; ");
		} else {
			printf("\n");
		}
	}
	free(q);
}

void free_tree(node* root){
	node* current = root;
	queue* q = build_queue();
	while (current && current->key != T_NIL){

		//enqueue the left tree node
		if(current->left && current->left->key != T_NIL){
			enqueue(current->left, q);
		}

		//enqueue the right tree node
		if(current->right && current->right->key != T_NIL){
			enqueue(current->right, q);
		}
		
		free(current);

		//dequeue the next tree node and make it current
		current = dequeue(q);
	}
	free(q);
}
