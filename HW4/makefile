all: queue.o hw4.o
	gcc queue.o hw4.o
queue.o: queue.c queue.h
	gcc -Wall -Werror -g -c queue.c
hw4.o: hw4.c
	gcc -Wall -Werror -g -c hw4.c
clean: 
	rm *.o *.out
