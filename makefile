SOURCE = tcp_server.c
OBJECTS = $(patsubst %.c, %.o, $(SOURCE))
CC = gcc
CFLAGS = -c -Wall -g3
OUT = sserver
OUTPUT_OPTION = -o $@

.PHONY : all clean run

all : $(OUT)

$(OUT) : $(OBJECTS)
	$(CC) $< -o $(OUT)

$(OBJECTS) : $(SOURCE)
	$(CC) $(CFLAGS) $< $(OUTPUT_OPTION)

clean :
	-rm -rf *.o

run :
	./$(OUT) 8888
