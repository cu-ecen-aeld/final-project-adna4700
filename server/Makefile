CROSS_COMPILE=
CC=gcc
SRC=server.c
LIBS=-lgpiod
CFLAGS=

all:
	$(CROSS_COMPILE)$(CC) $(SRC) $(LIBS) $(CFLAGS) -o server

clean:
	rm -f *.o server
