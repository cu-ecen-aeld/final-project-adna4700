CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -g -Wall -Werror
INCLUDES = -I/

all: server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

server: server.o
	$(CC) $(CFLAGS) server.o -o server -lgpiod

clean:
	rm -f *.o
	rm -f server
