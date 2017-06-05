PORTA0 = 51110
PORTA1 = 51111
PORTA2 = 51112
PORTA3 = 51113
PORTA4 = 51114
PORTA5 = 51115
PORTA6 = 51116
PORTA7 = 51117
PORTA8 = 51118
PORTA9 = 51119
FLAGS = -o2 -Wall
CC = gcc
	
all: src/central.c src/temperatura0.c src/temperatura1.c src/temperatura2.c src/economica.c src/executiva.c src/primeira.c src/reqtras.c src/reqmeio.c src/reqtras.c src/aeromoca.c src/socketsFunction.c
	@mkdir -p bin/
	$(CC) src/central.c src/socketsFunction.c $(FLAGS) -o bin/central
	$(CC) src/temperatura0.c src/socketsFunction.c $(FLAGS) -o bin/temperatura0
	$(CC) src/temperatura1.c src/socketsFunction.c $(FLAGS) -o bin/temperatura1
	$(CC) src/temperatura2.c src/socketsFunction.c $(FLAGS) -o bin/temperatura2
	$(CC) src/economica.c src/socketsFunction.c $(FLAGS) -o bin/economica
	$(CC) src/executiva.c src/socketsFunction.c $(FLAGS) -o bin/executiva
	$(CC) src/primeira.c src/socketsFunction.c $(FLAGS) -o bin/primeira
	$(CC) src/reqfrente.c src/socketsFunction.c $(FLAGS) -o bin/reqfrente
	$(CC) src/reqmeio.c src/socketsFunction.c $(FLAGS) -o bin/reqmeio
	$(CC) src/reqtras.c src/socketsFunction.c $(FLAGS) -o bin/reqtras
	$(CC) src/aeromoca.c src/socketsFunction.c $(FLAGS) -o bin/aeromoca

clean: 
	@rm -rf ./bin/* 

run:
	@./bin/temperatura0 localhost $(PORTA0) & 
	@./bin/temperatura1 localhost $(PORTA1) & 
	@./bin/temperatura2 localhost $(PORTA2) & 
	@./bin/economica localhost $(PORTA3) & 
	@./bin/executiva localhost $(PORTA4) & 
	@./bin/primeira localhost $(PORTA5) & 
	@./bin/reqfrente localhost $(PORTA6) &
	@./bin/reqmeio localhost $(PORTA7) & 
	@./bin/reqtras localhost $(PORTA8) & 
	@./bin/aeromoca localhost $(PORTA9)

	
	