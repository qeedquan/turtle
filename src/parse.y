%{
#include "dat.h"
#include "fns.h"
%}

%define parse.error verbose
%define parse.lac full

%union
{
    char  name[BUFSIZ];
    long  val;
}

%left '|' '^' '&'
%left '+' '-' '*' '/' '%'

%type <val> con expr

%token LFUNCTION LMOVETO LMOVE LCALL LCOLOR LDRAW LROTATE LLOOP
%token <name> LIDENT
%token <val> LNUMBER LHEX

%start prog

%%
prog:
|
    stmts

funcdecl: 
    LFUNCTION LIDENT '{' 
    {
        setinsbuf(1);
        gfcall($2);
        rcall($2);
    }
    
    stmts '}'
    {
        emit(1, ORET);
        setinsbuf(0);
    }

stmts:
    stmt
|
    stmt stmts

stmt:
    LCALL LIDENT
    {
        Func *f;
    
        f = lookupfunc($2);
        if (f)
            emit(2, OCALL, f->adr);
        else
            gcall($2);
    }
|
    LDRAW expr
    {
        emit(2, ODRAW, $2);
    }
|
    LCOLOR LIDENT
    {
        emit(2, OCOLOR, lookupcolor($2));
    }
|
    LCOLOR LHEX
    {
        emit(2, OCOLOR, $2);
    }
|
    LROTATE expr
    {
        emit(2, OROTATE, $2);
    }
|
    LMOVE expr ',' expr
    {
        emit(3, OMOVE, $2, $4);
    }
|
    LMOVETO expr ',' expr
    {
        emit(3, OMOVETO, $2, $4);
    }
|
    LLOOP expr '{'
    {
        gbloop($2);
    }
    
    stmts '}'
    {
        geloop();
    }
|
    funcdecl

con:
    LNUMBER
|
    '-' con
    {
        $$ = -$2;
    }
|
    '+' con
    {
        $$ = $2;
    }
|
    '~' con
    {
        $$ = ~$2;
    }
|
    '(' expr ')'
    {
        $$ = $2;
    }

expr:
    con
|
    expr '+' expr
    {
        $$ = $1 + $3;
    }
|
    expr '-' expr
    {
        $$ = $1 - $3;
    }
| 
    expr '*' expr
    {
        $$ = $1 * $3;
    }
|
    expr '/' expr
    {
        $$ = $1 / $3;
    }
|
    expr '%' expr
    {
        $$ = $1 % $3;
    }
|
    expr '|' expr
    {
        $$ = $1 | $3;
    }
|
    expr '&' expr
    {
        $$ = $1 & $3;
    }
|
    expr '^' expr
    {
        $$ = $1 ^ $3;
    }
