all: 
	gcc server.c -o2 -o server
	gcc client.c -o2 -o client1
	gcc client.c -o2 -o client2

clean: 
	@rm -rf *.o *~ *.dat client1 client2 server

run:
	(./client1 localhost 8080) & (./client2 localhost 8080)

	
