TARGET = mycalc
OBJS = \
  lex.yy.o\
  y.tab.o\
  create.o\
  eval.o\
  interface.o\
  util.o\
  error.o\
  main.o\
  ./memory/mem.o\
  ./debug/dbg.o
CFLAGS = -c -g -Wall
INCLUDES = \

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ -lreadline -lfl -lm
clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h calc.tab.c *~
y.tab.h : calc.y
	bison --yacc -dv calc.y
#	yacc -dv calc.y
y.tab.c : calc.y
	bison --yacc -dv calc.y
#	yacc -dv calc.y
lex.yy.c : calc.l calc.y y.tab.h
	flex calc.l
#	lex calc.l
y.tab.o: y.tab.c calc.h MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
lex.yy.o: lex.yy.c calc.h MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES)
############################################################
calc.tab.o: calc.tab.c calc.h MEM.h CLC.h
create.o: create.c DBG.h calc.h MEM.h CLC.h
error.o: error.c DBG.h calc.h MEM.h CLC.h
eval.o: eval.c MEM.h DBG.h calc.h CLC.h
interface.o: interface.c MEM.h calc.h CLC.h
main.o: main.c CLC.h
util.o: util.c calc.h MEM.h CLC.h
