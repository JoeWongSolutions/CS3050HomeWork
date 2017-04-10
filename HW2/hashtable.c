#include"hashtable.h"

int hash(char* name){
	int key = 0, i=0;
	double name_value = 0;
	//convert name into integer
	while (name[i]){
		name_value += name[i++];
	}
	
	//Use Multiplication Method to obtain key value
	key = 128 * (name_value * G_RAT - (int)(name_value * G_RAT));
	return key;
}

person* new_friend(char* name){
	//Create new node for friend
	person* new = malloc(sizeof(person));
	if (!new){
		puts("Out of Memory, could not create new friend");
		exit(0);
	}

	strcpy(new->name, name);
return new;	
}

int factorial(int size){
	if(!size) return 1;
	int result = size * factorial(size - 1);
	return result;
}

int add_friend(person** table, friendlist** ftable, char* name1, char* name2, int table_size, int ftable_size){
	
	person* current = table[hash(name1)%table_size];
	person* nextf = NULL;
	//travel to correct person's record
	while(current->next && strcmp(current->name,name1))
		current = current->next;
	
	//Add new friend to list of person1
	nextf = current->next_friend;
	current->next_friend = new_friend(name2);
	if(!current->next_friend) {
		puts("Not enough memory to assign friend");
		return 0;
	}
	current->next_friend->next_friend = nextf;

	//reset current pointer to add friend to person2's list
	current = table[hash(name2)%table_size];
	//travel to correct person's record
	while(current->next && strcmp(current->name, name2))
		 current = current->next;

	
	//Add new friend to list of person2
	nextf = current->next_friend;
	current->next_friend = new_friend(name1);
	if(!current->next_friend) {
		puts("Not enough memory to assign friend");
		return 0;
	}
	current->next_friend->next_friend = nextf;
	
	//Now that we've added friend nodes to the lists of both people, we need to update the friends table
	char concat_name[2*MAX_NAME] = "";
	//alphebetize the order in which names will concatenate
	if(strcmp(name1,name2) <= 0){
		strcat(concat_name, name1);
		strcat(concat_name, name2);
	}
	else{
		strcat(concat_name, name2);
		strcat(concat_name, name1);
	}
	friendlist* curr = ftable[hash(concat_name)%ftable_size];

	//If no info exists add node
	if(!curr){
		curr = malloc(sizeof(friendlist));
		strcpy(curr->name, concat_name);
		curr->friends = TRUE;
		ftable[hash(concat_name)%ftable_size] = curr;
	}
	else{
		//travel to correct person's record in friendlist array
		while(curr->next && strcmp(curr->name,concat_name))
			curr = curr->next;
	
		//check if names already exist. If we want to implement a friend history, this would be needed.
//		if(!strcmp(curr->name,concat_name)){
//			curr->friends = TRUE;
//		}

		//add new node and set name and friend status	
		curr->next = malloc(sizeof(friendlist));
		strcpy(curr->next->name, concat_name);
		curr->next->friends = TRUE;
		
	}

	return 2;
}

int search(person** table, char* name, int table_size){
	person* current = table[hash(name)%table_size];
	while(current){
		if(!strcmp(current->name, name))
			return 1;
		else
			current = current->next;
	}
return 0;
}

int remove_friend(person** table, char* name1, char* name2, int table_size){
	person* current = table[hash(name1)%table_size];
	person* prev = NULL;
	//Travel to correct person's record
	while(current->next && strcmp(current->name,name1))
		current = current->next;

	if(current->next_friend){
		prev = current;
		current = current->next_friend;	
	}
	else return 0;

	//Travel to correct friend
	while(current->next_friend && strcmp(current->name, name2)){
		prev = current;
		current = current->next_friend;
	}

	//Link prev with next friend
	prev->next_friend = current->next_friend;
	free(current);

	//Repeat for name2
	current = table[hash(name2)%table_size];
	prev = NULL;
	//Travel to correct person's record
	while(current->next && strcmp(current->name,name2))
		current = current->next;

	if(current->next_friend){
		prev = current;
		current = current->next_friend;	
	}
	else return 0;

	//Travel to correct friend
	while(current->next_friend && strcmp(current->name, name1)){
		prev = current;
		current = current->next_friend;
	}

	//Link prev with next friend
	prev->next_friend = current->next_friend;
	free(current);
	
return 1;	
}

int unfriend(friendlist** ftable, char* name1, char* name2, int ftable_size){
	
	char concat_name[20] = "";
	//alphebetize the order in which names will concatenate
	if(strcmp(name1,name2) <= 0){
		strcat(concat_name, name1);
		strcat(concat_name, name2);
	}
	else{
		strcat(concat_name, name2);
		strcat(concat_name, name1);
	}
	friendlist* curr = ftable[hash(concat_name)%ftable_size];
	friendlist* prev = NULL;
	//travel to correct node
	while(curr && strcmp(curr->name, concat_name)){
		prev = curr;
		curr = curr->next;
	}
	
	//delete friend node
	if(curr && curr->next){
		prev->next = curr->next;
		free(curr);
	}
	else if (curr){
		if (prev)
		prev->next = NULL;
		free(curr);
	}
	else return 0;
return 1;
}


int query(friendlist** ftable, char* name1, char* name2, int ftable_size){ 
	char concat_name[2*MAX_NAME]="";
	if(strcmp(name1,name2) <= 0){
		strcat(concat_name, name1);
		strcat(concat_name, name2);
	}
	else{
		strcat(concat_name, name2);
	strcat(concat_name, name1);
	}
	friendlist* curr = ftable[hash(concat_name)%ftable_size];
	//travel to correct node
	while(curr && strcmp(curr->name, concat_name)){
		curr = curr->next;
	}
	if(curr && curr->friends == TRUE)	 
		return 1;
	else{
	if(curr)
		return -1;
	}
	return 0;
	
}
