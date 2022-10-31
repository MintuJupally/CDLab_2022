%{
#define YYDEBUG 0

#include <stdio.h>
#include <stdlib.h>

int yyerror(char *s);

#include "tree.hpp"

#include "y.tab.h"
#include "lex.yy.c"
%}

%token PLUS MINUS MULT DIV NUM LEFT_PAREN RIGHT_PAREN END

%start exp

%union
{
    double val;
    Node* node;
}

%%
exp:        A END                       { 
                                            printf("%g\n", $<node>1->res); 
                                            $<node>1->printTree();
                                            $<node>1->printDAG();
                                            $<node>1->generate3AddressCode();

                                            system("dot -Tpng AST.dot -o AST.png");
                                            system("xdg-open AST.png");
                                        }
A:          A PLUS B                    { 
                                            double res = $<node>1->res + $<node>3->res;
                                            $<node>$ = new Node("+", res, {$<node>1, $<node>3}); 
                                        }
            | A MINUS B                 { 
                                            double res = $<node>1->res - $<node>3->res; 
                                            $<node>$ = new Node("-", res, {$<node>1, $<node>3}); 
                                        }
            | B                         { 
                                            $<node>$ = $<node>1; 
                                        }
B:          B MULT C                    { 
                                            double res = $<node>1->res * $<node>3->res; 
                                            $<node>$ = new Node("*", res, {$<node>1, $<node>3}); 
                                        }
            | B DIV C                   { 
                                            double res = $<node>1->res / $<node>3->res; 
                                            $<node>$ = new Node("/", res, {$<node>1, $<node>3}); 
                                        }
            | C                         { 
                                            $<node>$ = $<node>1; 
                                        }
C:          LEFT_PAREN A RIGHT_PAREN    { 
                                            double res = $<node>2->res; 
                                            $<node>$ = $<node>2; 
                                        }
            | PLUS C                    { 
                                            double res = $<node>2->res; 
                                            $<node>$ = new Node("+", res, {$<node>2}); 
                                        }
            | MINUS C                   { 
                                            double res = $<node>2->res * -1; 
                                            $<node>$ = new Node("-", res, {$<node>2}); 
                                        }
            | NUM                       { 
                                            $<node>$ = new Node($<val>1);
                                        }
%%

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