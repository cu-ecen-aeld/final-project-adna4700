CROSS_COMPILE=
ifeq ($(CC),)
	CC = $(CROSS_COMPILE)gcc
endif
ifeq ($(CFLAGS),)
	CFLAGS = -g -Wall -Werror
endif
ifeq ($(LDFLAGS),)
	LDFLAGS = -pthread -lrt -lgpiod
endif
all:server

clean:
	rm -f *.o server *.elf *.map

server: server.o
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)
