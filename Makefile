PORTA0 = 51111
PORTA1 = 51112
FLAGS = -o2
	
all: server.c client0.c client1.c socketsFunction.c
	gcc server.c socketsFunction.c -pthread $(FLAGS) -o server
	gcc client0.c socketsFunction.c $(FLAGS) -o client0
	gcc client1.c socketsFunction.c $(FLAGS) -o client1

clean: 
	@rm -rf *.o *~ *.dat client0 client1 server

run:
	./client0 localhost $(PORTA0) & ./client1 localhost $(PORTA1)

	
	