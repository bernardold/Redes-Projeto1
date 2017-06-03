PORTA0 = 51111
PORTA1 = 51112
PORTA2 = 51113
FLAGS = -o2 -Wall
	
all: server.c client0.c client1.c client2.c socketsFunction.c
	gcc server.c socketsFunction.c -pthread $(FLAGS) -o server
	gcc client0.c socketsFunction.c $(FLAGS) -o client0
	gcc client1.c socketsFunction.c $(FLAGS) -o client1
	gcc client2.c socketsFunction.c $(FLAGS) -o client2

clean: 
	@rm -rf *.o *~ *.dat

run:
	./client0 localhost $(PORTA0) & ./client1 localhost $(PORTA1) & ./client2 localhost $(PORTA2)

	
	