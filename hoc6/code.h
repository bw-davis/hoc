#include "hoc.h"
#include "y.tab.h"

#define	NSTACK	256
static 	Datum	stack[NSTACK];		/* the stack */
static 	Datum	*stackp;		/* next free spot on stack */

#define NPROG	2000
Inst	prog[NPROG];		/* the machine */
Inst	*progp;			/* next free spot for code generation */
Inst	*pc;			/* program counter during execution */
Inst	*progbase = prog;	/* start of current subprogram */
int 	returning;		/* 1 if return stmt seen */

typedef struct Frame {		/* proc/func call stack frame */
	Symbol 	*sp;		/* symbol table entry */
	Inst	*retpc;		/* where to resume after return */
	Datum	*argn;		/* n-th argument on stack */
	int nargs;		/* number of arguments */
} Frame;
#define NFRAME 100
Frame	frame[NFRAME];
Frame	*fp;			/* frame pointer */

void initcode();
void push(Datum d);	
Datum pop() ;
Inst *code(Inst f);
void execute(Inst *p);
void constpush();
void varpush();
void add();
void sub();
void mul();
void div();
void negate();
void power();
void eval();
void assign();
void print();
void bltin();
void le();
void gt();
void lt();
void eq();
void ge();
void ne();
void and();
void or();
void not();
void whilecode();
void ifcode();
void prexpr();
void define(Symbol *sp);
void call();
void ret();
void funcret();
void procret();
double *getarg();
void arg();
void argassign();
void prstr();
void varread();