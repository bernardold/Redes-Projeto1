all: 
	gcc server.c -Wall -o2 -o server
	gcc client.c -Wall -o2 -o client
	gcc client.c -Wall -o2 -o client1
	gcc client.c -Wall -o2 -o client2

clean: 
	@rm -rf *.o *~ *.dat client client1 client2 server

run:
	(./client1 localhost 8080) & (./client2 localhost 8080)

runcl:
	./client localhost 8080

runsv:
	./server 8080

	
