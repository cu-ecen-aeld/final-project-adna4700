ifeq ($(CC),)
	CC = $(CROSS_COMPILE)gcc
endif
ifeq ($CFLAGS),)
	CFLAGS = -g -Wall -Werror
endif

ifeq ($(INCLUDES),)
	INCLUDES = -I/
endif

all: led 


led.o: led.c
	$(CC) $(CFLAGS) -c mq7.c

led: led.o
	$(CC) $(CFLAGS) mq7.o -o mq7 -lgpiod

clean:
	rm -f *.o
	rm -f mq7
