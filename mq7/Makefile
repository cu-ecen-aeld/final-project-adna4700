ifeq ($(CC),)
	CC = $(CROSS_COMPILE)gcc
endif
ifeq ($CFLAGS),)
	CFLAGS = -g -Wall -Werror
endif

ifeq ($(INCLUDES),)
	INCLUDES = -I/
endif

all: mq7


mq7.o: mq7.c
	$(CC) $(CFLAGS) -c mq7.c

mq7: mq7.o
	$(CC) $(CFLAGS) mq7.o -o mq7 -lgpiod

clean:
	rm -f *.o
	rm -f mq7
