
default: snn snn-l3

snn: snn.o
	gcc -o snn snn.o -lm

snn-l3: snn-l3.o
	gcc -o snn-l3 snn-l3.o -lm

snn.o: snn.c
	gcc -Wall -Wextra -O2 -g -c snn.c 

snn-l3.o: snn-l3.c
	gcc -Wall -Wextra -O2 -g -c snn-l3.c 

clean:
	rm -f snn snn.o snn-l3 snn-l3.o 
