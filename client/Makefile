	
CC := $(CROSS-COMPILE)gcc
CFLAGS := -g -Wall -Werror
SRC := client.c
TARGET := client
LDFLAGS := -pthread -lrt

all: $(TARGET)
	 $(CROSS_COMPILE)$(CC) $(CFLAGS) ${SRC} -o ${TARGET} ${LDFLAGS}

clean:
	rm -f *.o $(TARGET) *.elf *.map
