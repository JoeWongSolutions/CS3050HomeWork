/* Joe Wong
 * 02216247
 * Created 2/18/2017
 * Assignment: Homework 2: Design a mini facebook using hashtables.
 * 	Should include functions to add people, add/remove friends, and query if two people are friends
*/

#include"hashtable.h"

int main(int argc, char** argv) {
	if (argc != 2 || atoi(argv[1]) < 1){
		printf("Please specify number of people. Use ./hashtable <number>\n");
		return -1;
	}
	int record_counter = 0;

	//NOTE: table size is 2 times as large as number of entries to minimize collision
	int table_size = atoi(argv[1])*2;
	//NOTE: ftable size is the factorial of number of entries
	int ftable_size = table_size * table_size;
	if (table_size < 1) return 0;

	//build the two hash tables, 
	//p_table holds the records of each person, 
	//f_table holds the boolean value of each person's friendship with other records.
	person** p_table = malloc(sizeof(person*)*table_size);
	if(!p_table) exit(0);
	friendlist** f_table = malloc(sizeof(friendlist*)*ftable_size);
	if(!f_table){
		free(p_table);
		exit(0);
	}
	
	//create temp space to hold people's names
	char command[2] = "", name1[MAX_NAME] = "", name2[MAX_NAME] = "";

	person* current = NULL;
	int key = 0;
	
	display_menu();
	
	while(*command != 'X'){
		printf("cmd: ");
	
		//loop until input is correct
		while(!get_input(command, name1, name2)) printf("cmd: ");

		//switch statement to decide which command to execute
		switch(*command){
			//Command P inputs new person to p_table
			case 'P':if(record_counter >= table_size - 1){
					printf("ERROR!! You have exceeded the allowed records. Please restart the program to increase table size.\n");
					break;
				 }
				 key = hash(name1)%table_size;
				 current = p_table[key];
				 
				 //create a node in the front of the chain
				 p_table[key] = malloc(sizeof(person));
				 strcpy(p_table[key]->name, name1);
				 p_table[key]->next = current;
				
				 record_counter++;
				 break;

			//Command F adds friend node to person1 and person2 and creates a friendship node in f_table
			case 'F':if(!search(p_table, name1, table_size)){	//check if name1 exists
					printf("%s does not yet exist in this database\n",name1);
				 	break;
				 }
				 if(!search(p_table, name2, table_size)){	//check if name2 exists
					printf("%s does not yet exist in this database\n",name2);
				 	break;
				 }

				 //check to see if already friends
				 if(query(f_table, name1, name2, ftable_size) == 1){
					printf("%s and %s are already friends\n", name1, name2);
					break;
				 }

				 //add each other to friends list
				 if(!add_friend(p_table, f_table, name1, name2, table_size, ftable_size)){
					puts("Not enough memory to add friends\n");
				 }
				 else {
					printf("%s and %s are now friends\n", name1, name2);
				 }
				 break;
					
			//Command L lists all friends of person1
			case 'L':if(!search(p_table, name1, table_size)){	//check if name1 exists
					printf("%s does not yet exist in this database\n",name1);
				 	break;
				 }
				 //Check for prior collision and find appropriate node
				 key = hash(name1)%table_size;
				 current = p_table[key];
				 while (current->next && strcmp(current->name, name1)){
					current = current->next;
				 }
				 //Check for existing friend data and print
				 if(current->next_friend){
				  printf("friends of %s are: ",name1);
				  current = current->next_friend;
				  while(current->next_friend){
					printf("%s, ",current->name);
					current = current->next_friend;
				  }
				  printf("%s\n",current->name);
				 }
				 else {
					printf("%s has no friends\n",name1);
				 }
				 break;

			//Command U unfriends person1 and person2
			case 'U':if(!search(p_table, name1, table_size)){	//check if name1 exists
					printf("%s does not yet exist in this database\n",name1);
				 	break;
				 }
				 if(!search(p_table, name2, table_size)){	//check if name2 exists
					printf("%s does not yet exist in this database\n",name2);
				 	break;
				 }
				 if(!remove_friend(p_table, name1, name2, table_size)) {
					printf("%s and %s are no longer friends\n", name1, name2);
				 }
				 if(!unfriend(f_table, name1, name2, ftable_size)){
					printf("%s and %s were never friends\n", name1, name2);
					break;
				 }

			//Command Q queries a friendship between person1 and person2
			case 'Q':if(!search(p_table, name1, table_size)){	//check if name1 exists
					printf("%s does not yet exist in this database\n",name1);
				 	break;
				 }
				 if(!search(p_table, name2, table_size)){	//check if name2 exists
					printf("%s does not yet exist in this database\n",name2);
				 	break;
				 }
				 int status = query(f_table, name1, name2, ftable_size);
				 if (status == 1)
					printf("%s and %s are friends\n", name1, name2);
				 else if (status)
					printf("%s and %s are no longer friends\n", name1, name2);
				 else
				 	printf("%s and %s are not friends\n", name1, name2);
				 break;
		}//end of switch
	
	}//end program loop
	
	puts("Exiting the Program...");	
	free_table(p_table, table_size);
	free_ftable(f_table, ftable_size);
	return 0;
}

void display_menu() {
	puts("Command List:");
	puts("P <name> 		Create person record");
	puts("F <name1> <name2>	Record two people as friended");
	puts("U <name1> <name2>	Record two people as unfriended");
	puts("L <name>		List all friends of person");
	puts("Q <name1> <name2>	Query if two people are friends");
	puts("X			Terminate program");
}

//Input function parses input to make sure format is correct
int get_input(char* command, char* name1, char* name2) {

	char temp[MAX_LINE] = "";
	char check_command[MAX_NAME] = "", check_name1[MAX_NAME] = "", check_name2[MAX_NAME] = "";
	
	fgets(temp, MAX_LINE-1, stdin);
	sscanf(temp,"%s %s %s",check_command, check_name1, check_name2);
	//check for invalid command length
	if (*(check_command+1)){
		puts("Must enter a valid command");
		return 0;
	}
	if (*check_command) *check_command = toupper(*check_command);
	
	//check for command input which is required
	if (!*check_command){
		return 0;
	}	
	if (*check_command != 'P'&&
	    *check_command != 'F'&&
	    *check_command != 'U'&&
	    *check_command != 'L'&&
	    *check_command != 'Q'&&
	    *check_command != 'X' )
	{
		puts("Must input a valid command");
		return 0;	
	}
		
	//check for name1 which is required for all commands except X
	if (strcmp(check_command,"X") && !*check_name1) {
		puts("Must input a name");
		return 0;
	}

	if ((!strcmp(check_command, "F") || !strcmp(check_command, "U") || !strcmp(check_command, "Q")) && !*check_name2){
		puts("Must input a second name");
		return 0;
	}

	//add command and names to variables in main
	strcpy(command, check_command);
	strcpy(name1, check_name1);
	strcpy(name2, check_name2);

return 1;
}


void free_table(person** table, int table_size){
	int i = 0;
	person* current = NULL, *nextfriend = NULL, *next = NULL;
	while (i<table_size){
		current = table[i];
		next = current;		

		while(current){
			nextfriend = current->next_friend;
			current = nextfriend;
			while(current){
				nextfriend = current->next_friend;
				free(current);
				current = nextfriend;
			}
			current = next;
			next = current->next;
			free(current);
			current = next;
		}
		i++;
	}
	free(table);
}

void free_ftable(friendlist** table, int ftable_size){
	int i = 0;
	friendlist* current = NULL;
	while (i<ftable_size){
		for(current = table[i]; current != NULL; current = current->next)
			free(current);
		i++;
	}
	free(table);
}
