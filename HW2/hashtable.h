#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define MAX_LINE 100
#define MAX_NAME 20
#define G_RAT 0.618033
typedef enum{FALSE, TRUE}bool;

typedef struct friendlist{
	char name[MAX_NAME];
	bool friends;
	struct friendlist* next;
}friendlist;

typedef struct person{
	char name[MAX_NAME];
	struct person *next_friend;
	struct person *next;
}person;

void display_menu();
int get_input(char* command, char* name1, char* name2);
int hash(char* name);
int factorial(int);
person* new_friend(char* name);
int add_friend(person** table, friendlist** ftable, char* name1, char* name2, int table_size, int ftable_size);
int search(person** table, char* name, int table_size);
int remove_friend(person** table, char* name1, char* name2, int table_size);
int unfriend(friendlist** ftable, char* name1, char* name2, int ftable_sizei);
int query(friendlist** ftable, char* name1, char* name2, int ftable_size);
void free_table(person** table, int table_size);
void free_ftable(friendlist** table, int ftable_size);
