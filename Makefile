
snn: snn.o
	gcc -o snn snn.o -lm

snn.o: snn.c
	gcc -Wall -Wextra -O2 -g -c snn.c 

clean:
	rm -f snn snn.o
