#include "dat.h"
#include "fns.h"

static int resetpc;
static int inslen;
static int *ins;
static int stk[STK];
static int pc;
static int sp;

static int fetch(void)
{
	int v;

	v = ins[pc];
	pc = (pc + 1) & (INS - 1);
	return v;
}

static void push(int v)
{
	stk[sp--] = v;
	if (sp < 0)
		sp = STK - 1;
}

static int pop(void)
{
	sp = (sp + 1) & (STK - 1);
	return stk[sp];
}

void resetvm(void)
{
	memset(stk, 0, sizeof(stk));
	sp = STK - 1;
	pc = resetpc;
}

void setresetpc(int rpc)
{
	resetpc = rpc;
}

void setcode(int *buf, int start, int len)
{
	if (start + len > inslen) {
		inslen = start + len;
		ins = erealloc(ins, sizeof(int) * inslen);
	}

	memcpy(ins + start, buf, sizeof(int) * len);
}

static void pcode(int pc)
{
	int op;
	int a1, a2;

	op = ins[pc];
	a1 = ins[pc + 1];
	a2 = ins[pc + 2];

	printf("%04d ", pc);
	switch (op) {
	case OHLT:
		printf("hlt");
		break;

	case OCALL:
		printf("call %d", a1);
		break;

	case ORET:
		printf("ret");
		break;

	case OBGTR:
		printf("bgtr %d", a1);
		break;

	case ODECBRA:
		printf("decbra %d", a1);
		break;

	case ODRAW:
		printf("draw %d", a1);
		break;

	case OROTATE:
		printf("rotate %d", a1);
		break;

	case OCOLOR:
		printf("color #%06X", a1);
		break;

	case OMOVE:
		printf("move %d %d", a1, a2);
		break;

	case OMOVETO:
		printf("moveto %d %d", a1, a2);
		break;

	case OPUSH:
		printf("push %d", a1);
		break;

	case OPOP:
		printf("pop");
		break;

	default:
		printf("unknown opcode: %X\n", op);
		exit(1);
	}
	printf("\n");
}

void execute(void)
{
	int op, t;
	float ox, oy, rad;
	float sx, sy;

	for (;;) {
		if (tflag)
			pcode(pc);
		op = fetch();
		switch (op) {
		case OHLT:
			return;

		case OCALL:
			push(pc);
			pc = fetch();
			break;

		case ORET:
			pc = pop();
			fetch();
			break;

		case OBGTR:
			t = fetch();
			if (stk[sp + 1] <= 0)
				pc += t;
			break;

		case ODECBRA:
			stk[sp + 1]--;
			pc += fetch();
			break;

		case OPUSH:
			push(fetch());
			break;

		case OPOP:
			pop();
			break;

		case ODRAW:
			t = fetch();
			rad = deg2rad(angle * 1.0);
			ox = t * cos(rad);
			oy = t * sin(rad);
			sx = px;
			sy = py;
			SDL_SetRenderDrawColor(render, color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, 0xFF);
			SDL_RenderDrawLine(render, sx, sy, roundf(sx + ox), roundf(sy + oy));
			px = roundf(px + ox);
			py = roundf(py + oy);
			break;

		case OCOLOR:
			color = fetch();
			break;

		case OROTATE:
			angle += fetch();
			break;

		case OMOVE:
			px += fetch();
			py += fetch();
			break;

		case OMOVETO:
			px = fetch();
			py = fetch();
			break;
		}
	}
}
