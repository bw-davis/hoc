YFLAGS = -d
OBJS = hoc.o code.o init.o math.o symbol.o

hoc6:	$(OBJS)
	cc $(OBJS) -lm -o hoc6

x.tab.h:	y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h

pr:	hoc.y hoc.h code.c code.h init.c init.h math.c symbol.c symbol.h
	@pr $?
	@touch pr

clean:
	rm -f $(OBJS) [xy].tab.[ch]

code.o: code.h symbol.h hoc.h x.tab.h
init.o: symbol.h code.h y.tab.h
math.o: hoc.h
symbol.o: hoc.h symbol.h