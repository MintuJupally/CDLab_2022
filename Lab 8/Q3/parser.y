%{
#define YYDEBUG 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int yyerror(char *s);

#include "y.tab.h"
#include "lex.yy.c"
%}

%token PLUS MINUS MULT DIV EXP NUM LEFT_PAREN RIGHT_PAREN END

%start exp

%union
{
    double val;
}

%%
exp:        A END                       { printf("%g\n", $<val>1); }
A:          A PLUS B                    { $<val>$ = $<val>1 + $<val>3; }
            | A MINUS B                 { $<val>$ = $<val>1 - $<val>3; }
            | B                         { $<val>$ = $<val>1; }
B:          B MULT C                    { $<val>$ = $<val>1 * $<val>3; }
            | B DIV C                   { $<val>$ = $<val>1 / $<val>3; }
            | C                         { $<val>$ = $<val>1; }
C:          PLUS C                      { $<val>$ = $<val>2; }
            | MINUS C                   { $<val>$ = $<val>2 * -1; }
            | D                         { $<val>$ = $<val>1; }
D:          E EXP C                     { $<val>$ = pow($<val>1, $<val>3); }
            | E                         { $<val>$ = $<val>1; }
E:          LEFT_PAREN A RIGHT_PAREN    { $<val>$ = $<val>2; }
            | NUM                       { $<val>$ = $<val>1; }
%%

extern int yylex(), yyparse();

int yyerror(char *s)
{
	fprintf(stderr, "%s in line no : %d => '%s'\n", s, yylineno, yytext);
	exit(-1);
}

int main()
{
#if YYDEBUG
    yydebug = 1;
#endif

    extern FILE *yyin;
    yyin = fopen("input.txt", "r");

    if(yyin==NULL)
    {
        printf("Could not open 'input.txt'\n");
        return -1;
    }

    yyparse();

    return 0;
}