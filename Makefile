all: 
	gcc server.c socketsFunc.c -o2 -o server
	gcc client.c socketsFunc.c -o2 -o client

clean: 
	@rm -rf *.o *~ *.dat


	
