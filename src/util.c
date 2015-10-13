#include "dat.h"
#include "fns.h"

void *emalloc(size_t size)
{
    void *p;

    p = malloc(size);
    if (!p)
        abort();

    memset(p, 0, size);
    return p;
}

void *erealloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if (!ptr)
        abort();

    return ptr;
}

static struct 
{
    char *name;
    long val;
} colors[] =
{
    {"White",  0xFFFFFF},
    {"Black",  0x000000},
    {"Red",    0x0000FF},
    {"Green",  0x00FF00},
    {"Blue",   0xFF0000},
    {"Yellow", 0xFFFF00},
};

long lookupcolor(const char *c)
{
    char errmsg[BUFSIZ];
    unsigned i;

    for (i = 0; i < nelem(colors); i++)
    {
        if (strcasecmp(colors[i].name, c) == 0)
            return colors[i].val;
    }

    snprintf(errmsg, sizeof(errmsg), "unknown color: %s", c);
    yyerror(errmsg);
    return 0;
}

long
hex(const char *nptr)
{
    char *endptr;
    long v;

    errno = 0;
    v = strtol(nptr, &endptr, 16);
    if ((errno == ERANGE && (v == LONG_MAX || v == LONG_MIN)) || (errno != 0 && v != 0))
        yyerror(strerror(errno));
    else if (endptr == nptr)
        yyerror("empty hex value");
    else if (*endptr != '\0')
        yyerror("hex value contain invalid characters");
 
    return v;
}
