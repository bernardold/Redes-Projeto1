CC = gcc
CFLAGS = -Wall
PROG = projeto1
SRCS = projeto1.c
#LIBS = -lGLU -lGL -lglut -lSOIL -lm -pthread

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -o $(PROG)

clean:
	rm -f $(PROG)
