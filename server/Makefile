



ifeq ($(CC),)
	CC = $(CROSS_COMPILE)gcc
endif
ifeq ($CFLAGS),)
	CFLAGS = -g -Wall -Werror
endif

ifeq ($(INCLUDES),)
	INCLUDES = -I/
endif

all: server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

server: server.o
	$(CC) $(CFLAGS) server.o -o server -lgpiod

clean:
	rm -f *.o
	rm -f server
