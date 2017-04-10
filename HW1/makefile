All: heapsort.o utility.o
	gcc -o heapsort heapsort.o utility.o -Wall -Werror -g
heapsort.o: heapsort.c heapsort.h
	gcc -o heapsort.o -c heapsort.c 
utility.o: utility.c heapsort.h
	gcc -o utility.o -c utility.c
clean:
	rm *.o heapsort
