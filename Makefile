
default: snn-l2 snn-l3

snn-l2: snn-l2.o
	gcc -o snn-l2 snn-l2.o -lm

snn-l3: snn-l3.o
	gcc -o snn-l3 snn-l3.o -lm

snn-l2.o: snn-l2.c
	gcc -Wall -Wextra -O2 -g -c snn-l2.c 

snn-l3.o: snn-l3.c
	gcc -Wall -Wextra -O2 -g -c snn-l3.c 

clean:
	rm -f snn-l2 snn-l2.o snn-l3 snn-l3.o 
