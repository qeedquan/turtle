#include "dat.h"
#include "fns.h"

typedef struct Call Call;

struct Call {
	char name[BUFSIZ];
	Ins *ins;
	size_t linkpos;
};

#define HASH 8191
static Func *ftab[HASH];

static Call *callstk;
static size_t callstkpos;
static size_t callstklen;

static int *loopstk;
static size_t loopstkpos;
static size_t loopstklen;

static Ins fins;
static Ins mins;
static Ins *cins;

static void initins(Ins *i, size_t len)
{
	i->buf = emalloc(len);
	i->pos = 0;
	i->len = len;
}

static void resizeins(size_t n)
{
	Ins *i;

	i = cins;
	i->buf = erealloc(i->buf, n);
	i->len = n;
}

static unsigned fhash(char *s)
{
	unsigned hash;
	char c;

	hash = 5381;
	while ((c = *s++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

void initgins(void)
{
	initins(&fins, INS);
	initins(&mins, INS);
	cins = &mins;

	callstkpos = 0;
	callstklen = 256;
	callstk = emalloc(sizeof(Call) * callstklen);

	loopstkpos = 0;
	loopstklen = 256;
	loopstk = emalloc(sizeof(int) * loopstklen);
}

void setinsbuf(int b)
{
	cins = b ? &fins : &mins;
}

Func *lookupfunc(char *fname)
{
	Func *f;
	unsigned h;

	h = fhash(fname) % HASH;
	f = ftab[h];
	while (f) {
		if (strcmp(f->name, fname) == 0)
			return f;
	}

	return NULL;
}

void gfcall(char *fname)
{
	char errmsg[BUFSIZ];
	unsigned h;
	Func *pf, *f;

	h = fhash(fname) % HASH;

	pf = NULL;
	f = ftab[h];
	while (f) {
		if (strcmp(f->name, fname) == 0) {
			snprintf(errmsg, sizeof(errmsg), "function %s redefined", fname);
			yyerror(errmsg);
		}

		pf = f;
		f = f->link;
	}

	f = emalloc(sizeof(Func));
	snprintf(f->name, BUFSIZ, "%s", fname);
	f->adr = fins.pos;

	if (!pf)
		ftab[h] = f;
	else
		pf->link = f;
}

void gcall(char *fname)
{
	Call *c;
	size_t i;

	i = callstkpos;
	c = &callstk[i];

	if (++i >= callstklen) {
		callstk = erealloc(callstk, sizeof(Call) * i * 2);
		callstklen = i * 2;
	}

	snprintf(c->name, BUFSIZ, "%s", fname);

	emit(1, OCALL);
	c->ins = cins;
	c->linkpos = cins->pos;
	emit(1, 0);

	callstkpos = i;
}

void rcall(char *fname)
{
	Func *f;
	Call *c;
	size_t i;

	f = lookupfunc(fname);
	for (i = 0; i < callstkpos;) {
		c = &callstk[i];
		if (strcmp(c->name, fname) == 0) {
			c->ins->buf[c->linkpos] = f->adr;
			*c = callstk[callstkpos - 1];
			callstkpos--;
			continue;
		}
		i++;
	}
}

void gbloop(int n)
{
	emit(2, OPUSH, n);
	emit(2, OBGTR, 0);

	if (loopstkpos >= loopstklen) {
		loopstklen *= 2;
		loopstk = erealloc(loopstk, sizeof(int) * loopstklen);
	}

	loopstk[loopstkpos++] = cins->pos - 1;
}

void geloop(void)
{
	int p, l;

	p = loopstk[--loopstkpos];
	l = cins->pos - p;

	cins->buf[p] = l + 1;
	emit(2, ODECBRA, -(l + 3));
	emit(1, OPOP);
}

void emit(int n, ...)
{
	Ins *p;
	int i, c;
	va_list ap;

	p = cins;
	if (p->pos + n >= p->len)
		resizeins((p->pos + n) * 2);

	va_start(ap, n);
	for (i = 0; i < n; i++) {
		c = va_arg(ap, int);
		p->buf[p->pos++] = c;
	}
	va_end(ap);
}

void linkcode(void)
{
	size_t i;

	for (i = 0; i < callstkpos; i++)
		fprintf(stderr, "error: undefined reference to %s\n", callstk[i].name);

	if (i > 0)
		exit(1);

	setcode(fins.buf, 0, fins.pos);
	setcode(mins.buf, fins.pos, mins.pos);
	setresetpc(fins.pos);
}
