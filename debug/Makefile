TARGET = dbg.o
CFLAGS = -c -g -Wall
OBJS = debug.o
INCLUDES = -I../

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $*.c
main.o: main.c DBG.h
test.o: test.c
