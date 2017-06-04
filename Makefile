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
	
all: central.c temperatura0.c temperatura1.c temperatura2.c economica.c executiva.c primeira.c reqtras.c reqmeio.c reqtras.c aeromoca.c socketsFunction.c
	gcc central.c socketsFunction.c -pthread $(FLAGS) -o central
	gcc temperatura0.c socketsFunction.c $(FLAGS) -o temperatura0
	gcc temperatura1.c socketsFunction.c $(FLAGS) -o temperatura1
	gcc temperatura2.c socketsFunction.c $(FLAGS) -o temperatura2
	gcc economica.c socketsFunction.c $(FLAGS) -o economica
	gcc executiva.c socketsFunction.c $(FLAGS) -o executiva
	gcc primeira.c socketsFunction.c $(FLAGS) -o primeira
	gcc reqfrente.c socketsFunction.c $(FLAGS) -o reqfrente
	gcc reqmeio.c socketsFunction.c $(FLAGS) -o reqmeio
	gcc reqtras.c socketsFunction.c $(FLAGS) -o reqtras
	gcc aeromoca.c socketsFunction.c $(FLAGS) -o aeromoca

clean: 
	@rm -rf *.o *~ central temperatura0 temperatura1 temperatura2 economica executiva primeira reqtras reqmeio reqtras aeromoca

run:
	./temperatura0 localhost $(PORTA0) & ./temperatura1 localhost $(PORTA1) & ./temperatura2 localhost $(PORTA2) & ./economica localhost $(PORTA3) & ./executiva localhost $(PORTA4) & ./primeira localhost $(PORTA5) & ./reqfrente localhost $(PORTA6) & ./reqmeio localhost $(PORTA7) & ./reqtras localhost $(PORTA8) & ./aeromoca localhost $(PORTA9)

	
	