#include "dat.h"
#include "fns.h"
#include "y.tab.h"

static void skipcom (void);
static void skipws  (void);
static void getch   (void);
static void putch   (void);
static void getword (void);
static void getnum  (void);
static void gethex  (void);
static int  lookup  (char*);

static char *ion;
static FILE *iof;

static int   ch;
static char  wbuf[BUFSIZ];
static int   wpos;

static long  xline, xcolumn;
static long  line, column;

void newfile(char *f)
{
    ion = f;
    iof = fopen(f, "rb");
    if (!iof)
        die("%s: %s", ion, strerror(errno));

    xline = line = 1;
    xcolumn = column = 1;
    getch();
}

void yyerror(const char *error)
{
    die("%s:%ld:%ld: %s", ion, xline, xcolumn, error);
}

int yylex(void)
{
    char errmsg[80];
    int keyword, c;

loop:
    skipws();

    xline = line;
    xcolumn = column;
    if (ch == '/')
    {
        getch();
        if (ch == '/')
        {
            skipcom();
            goto loop;
        }

        return '/';
    }

    if (ch == EOF)
        return EOF;

    if (isalpha(ch))
    {
        getword();
        keyword = lookup(wbuf);
        wpos = 0;

        if (keyword >= 0)
            return keyword;

        snprintf(yylval.name, BUFSIZ, "%s", wbuf);
        return LIDENT;
    }

    if (isdigit(ch))
    {
        getnum();
        yylval.val = atoi(wbuf);
        wpos = 0;
        return LNUMBER;
    }

    c = ch;
    getch();

    if (c == '#')
    {
        gethex();
        yylval.val = hex(wbuf+1);
        wpos = 0;
        return LHEX;
    }

    switch (c)
    {
        case '+':
        case '-':
        case '{':
        case '}':
        case ',':
        case '*':
        case '%':
        case '|':
        case '&':
        case '^':
        case '~':
        case '(':
        case ')':
            return c;
    }

    snprintf(errmsg, sizeof(errmsg), "unknown symbol: %c", c);
    yyerror(errmsg);
    return 0;
}

static void skipws(void)
{
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        getch();
}

static void skipcom(void)
{
    while (ch != '\n' && ch != EOF)
        getch();
}

static void getch(void)
{
    ch = fgetc(iof);
    if (ch == '\n')
    {
        line++;
        column = 1;
    }
    else
        column++;
}

static void putch(void)
{
    if (wpos >= BUFSIZ)
        die("wbuf overflowed");

    wbuf[wpos++] = ch;
}

static void getword(void)
{
    do
    {
        putch();
        getch();
    } while (isalpha(ch) || isdigit(ch));
    wbuf[wpos] = '\0';
}

static void gethex(void)
{
    do
    {
        putch();
        getch();
    } while (isdigit(ch) || ('A' <= ch && ch <= 'F') || ('a' <= ch && ch <= 'f'));
    wbuf[wpos] = '\0';
}

static void getnum(void)
{
    do
    {
        putch();
        getch();
    } while (isdigit(ch));
    wbuf[wpos] = '\0';
}

static struct
{
    char *name;
    int   token;
} keywords[] =
{
    {"Function", LFUNCTION},
    {"MoveTo",   LMOVETO},
    {"Move",     LMOVE},
    {"Call",     LCALL},
    {"Draw",     LDRAW},
    {"Rotate",   LROTATE},
    {"Loop",     LLOOP},
    {"Color",    LCOLOR},
};

static int lookup(char *s)
{
    size_t i;

    for (i = 0; i < nelem(keywords); i++)
    {
        if (strcmp(s, keywords[i].name) == 0)
            return keywords[i].token;
    }

    return -1;
}
