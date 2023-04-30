CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -g -Wall -Werror
INCLUDES = -I/
SRC := client.c
TARGET := client
LDFLAGS := -pthread -lrt

all: $(TARGET) 
	$(CC) $(CFLAGS) ${SRC} -o ${TARGET} ${LDFLAGS}

clean:
	rm -f *.o $(TARGET) *.elf *.map

