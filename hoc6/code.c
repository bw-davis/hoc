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

void initcode() {		/* initialize for code generation */
	progp = progbase;
	stackp = stack;
	fp = frame;
	returning = 0;
}

void push(Datum d) {		/* push d onto the stack */
	if (stackp >= &stack[NSTACK])
		execerror("stack overflow", (char *) 0);
	*stackp++ = d;
}	

Datum pop() {		/* pop and return top elem from stack */
	if (stackp <= stack)
		execerror("stack overflow", (char *) 0);
	return *--stackp;
}
Inst *code(Inst f) {		/* install one instruction or operand */
	Inst *oprogp = progp;
	if (progp >= &prog[NPROG])
		execerror("program too big", (char *) 0);
	*progp++ = f;
	return oprogp;
}
void execute(Inst *p) {		/* run the machine */
	for (pc = p; *pc != STOP && !returning; )
		(*(*pc++))();
}
void constpush() {		/* push constant onto the stack */
	Datum d;
	d.val = ((Symbol *) *pc++)->u.val;
	push(d);
}
void varpush(){			/* push variable onto the stack */
	Datum d;
	d.sym = (Symbol *)(*pc++);
	push(d);
}
void add() {		/* add top two elems on stack */
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val += d2.val;
	push(d1);
}
void sub() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val -= d2.val;
	push(d1);
}
void mul() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val *= d2.val;
	push(d1);
}
void div() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val /= d2.val;
	push(d1);
}
void negate() {
	Datum d1;
	d1 = pop();
	d1.val = d1.val * (-1);
	push(d1);
}
void power() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	int i;
	for (i = 1; i <= d2.val; i++){
		d1.val *= d1.val;
	}
	push(d1);
}
void eval() {		/* evaluate variable on stack */
	Datum d;
	d = pop();
	if (d.sym->type == UNDEF)
		execerror("undefined variable", d.sym->name);
	d.val = d.sym->u.val;
	push(d);
}
void assign() {		/* assign top value to next value */
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable", d1.sym->name);
	d1.sym->u.val = d2.val;
	d1.sym->type = VAR;
	push(d2);
}
void print() {		/*pop top value from stack, print it */
	Datum d;
	d = pop();
	printf("\t%.8g\n", d.val);
}
void bltin() {		/* evaluate the built-in on top of the stack */
	Datum d;
	d = pop();
	d.val = (*(double (*)())(*pc++))(d.val);
	push(d);
}
void le() {
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val <= d2.val);
	push(d1);
}
void gt() {
	Datum d1, d2;
        d2 = pop();
        d1 = pop();
        d1.val = (double)(d1.val > d2.val);
        push(d1);
}
void lt() {
	Datum d1, d2;
        d2 = pop();
        d1 = pop();
        d1.val = (double)(d1.val < d2.val);
        push(d1);
}
void eq() {
	Datum d1, d2;
        d2 = pop();
        d1 = pop();
        d1.val = (double)(d1.val = d2.val);
        push(d1);

}
void ge() {
	Datum d1, d2;
        d2 = pop();
        d1 = pop();
        d1.val = (double)(d1.val >= d2.val);
        push(d1);
}
void ne() {
	Datum d1, d2;
        d2 = pop();
        d1 = pop();
        d1.val = (double)(d1.val != d2.val);
        push(d1);
}
void and() {
	Datum d1, d2;
        d2 = pop();
        d1 = pop();
        d1.val = (double)(d1.val && d2.val);
        push(d1);
}
void or() {
	Datum d1, d2;
        d2 = pop();
        d1 = pop();
        d1.val = (double)(d1.val || d2.val);
        push(d1);
}
void not() {
	Datum d1;
	d1 = pop();
	d1.val = (double)( ! d1.val);
	push(d1);
}
void whilecode() {
	Datum d;
	Inst *savepc = pc;		/* loop body */
	
	execute(savepc+2);		/* condition */
	d = pop();
	while (d.val) {
		execute(*((Inst **)(savepc)));		/* body */
		if (returning)
			break;
		execute(savepc+2);
		d = pop();
	}
	if (!returning)
		pc = *((Inst **)(savepc+1));		/* next statement */
}
void ifcode() {
	Datum d;
	Inst *savepc = pc;		/* then part */
	
	execute(savepc+3);		/* condition */
	d = pop();
	if (d.val)
		execute(*((Inst **)(savepc)));
	else if (*((Inst **)(savepc+1)))	/* else part? */
		execute(*((Inst **)(savepc+1)));
	if (!returning)
		pc = *((Inst **)(savepc+2));		/* next stmt */
}
void prexpr() {		/* print numeric value */
	Datum d;
	d = pop();
	printf("%.8g\n", d.val);
}
void define(Symbol *sp) {		/* put func/proc in symbol table */
	sp->u.defn = (Inst)progbase;		/* start of code */
	progbase = progp;		/* next code starts here */
}
void call() {			/* call a function */
	Symbol *sp = (Symbol *)pc[0];		/* symbol for table entry */
						/* for function */
	if (fp++ >= &frame[NFRAME-1])
		execerror(sp->name, "call nested too deeply");
	fp->sp = sp;
	fp->nargs = (int)pc[1];
	fp->retpc = pc + 2;
	fp->argn = stackp - 1;		/* last argument */
	execute(sp->u.defn);
	returning = 0;
}
void ret() {		/* common return from func or proc */
	int i;
	for (i = 0; i < fp->nargs; i++)
		pop();		/* pop arguments */
	pc = (Inst *)fp->retpc;
	--fp;
	returning = 1;
}
void funcret() {		/* return from a function */
	Datum d;
	if (fp->sp->type == PROCEDURE)
		execerror(fp->sp->name, "(proc) returns a value");
	d = pop();	/* preserve a function return value */
	ret();
	push(d);
}
void procret() {		/* return from a procedure */
	if (fp->sp->type == FUNCTION)
		execerror(fp->sp->name, "(func) returns no value");
	ret();
}
double *getarg() {		/* return pointer to argument */
	int nargs = (int) *pc++;
	if (nargs > fp->nargs)
		execerror(fp->sp->name, "not enough arguments");
	return &fp->argn[nargs - fp->nargs].val;
}
void arg() {			/* push argument onto the stack */
	Datum d;
	d.val = *getarg();
	push(d);
}
void argassign() {		/* store top of stack in argument */
	Datum d;
	d = pop();
	push(d);		/* leave value on stack */
	*getarg() = d.val;
}
void prstr() {			/* print string value */
	printf("%s", (char *) *pc++);
}
/*void prexpr() {			/* print numeric value 
	Datum d;
	d = pop();
	printf("%.8g ", d.val);
}*/
void varread() {		/* read into variable */
	Datum d;
	extern FILE *fin;
	Symbol *var = (Symbol *) *pc++;
   Again:
	switch (fscanf(fin, "%lf", &var->u.val)) {
	case EOF:
		if (moreinput())
			goto Again;
		d.val = var->u.val = 0.0;
		break;
	case 0:
		execerror("non-number read into", var->name);
		break;
	default:
		d.val = 1.0;
		break;
	}
	var->type = VAR;
	push(d);
}












