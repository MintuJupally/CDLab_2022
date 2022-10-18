%{
#include<stdio.h>
%}

plus            \+
minus           \-
mult            \*
div             \/

digit           [0-9]
digits          {digit}+
number          {digits}(\.{digits})?(E[+-]?{digits})?

left_paren      \(
right_paren     \)

ws              [ \t]
newline         [\n]

%%

{plus}          { return PLUS; }
{minus}         { return MINUS; }
{mult}          { return MULT; }
{div}           { return DIV; }
{number}        {
                    yylval.val= atof(yytext);
                    return NUM; 
                }

{left_paren}    { return LEFT_PAREN; }
{right_paren}   { return RIGHT_PAREN; }

{ws}            { }
{newline}       { return END; }

.               {
                    yyerror("Invalid character encountered"); 
                    return -1;
                }
%%

int yywrap() {
    return 1;
}
