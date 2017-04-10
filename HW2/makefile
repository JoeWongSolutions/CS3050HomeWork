All:hw2.o hashtable.o
	gcc hw2.o hashtable.o -g -Wall -Werror -o hashtable
hw2.0:hw2.c hashtable.h
	gcc -c hw2.c -g -Wall -Werror -o hw2.o
hashtable.o:hashtable.c hashtable.h
	gcc -c hashtable.c -g -Wall -Werror -o hashtable.o
clean:
	rm *.o hashtable
