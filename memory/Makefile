TARGET = mem.o
CFLAGS = -c -g -Wall
OBJS = memory.o storage.o

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
.c.o:
	$(CC) $(CFLAGS) $*.c
memory.o: memory.c MEM.h
storage.o: storage.c MEM.h
