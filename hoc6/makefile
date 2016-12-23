YFLAGS = -d
CFLAGS = -W -Wall
CC=gcc
OBJS = hoc.o code.o init.o mymath.o symbol.o

hoc6:	$(OBJS)
	$(CC) $(OBJS) -lm -o hoc6

x.tab.h:	y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h

pr:	hoc.y hoc.h code.c init.c mymath.c symbol.c
	@pr $?
	@touch pr

clean:
	rm -f $(OBJS) [xy].tab.[ch]

y.tab.h: hoc.y
	yacc $(YFLAGS) hoc.y

hoc.o: hoc.y hoc.h code.h symbol.h
code.o: code.c code.h x.tab.h hoc.h
init.o: init.c init.h symbol.h mymath.h code.h x.tab.h
mymath.o: mymath.c mymath.h hoc.h
symbol.o: symbol.c symbol.h hoc.h
