%{
#include <stdio.h>
#include "y.tab.h"
%}
%option yylineno 
%option noyywrap

delim              [ \t\n]
digit              [0-9]
digits             {digit}+
letter             [A-Za-z]

whitespace         {delim}+

format             %[fds]

float_const        {digits}(\.{digits})?(E[+-]?{digits})?
int_const          {digits}
string             \".*\"

and                and
assign             :=
colon              :
comma              ,
def                def
div                \/
dot                \.
else               else
end                end
eq                 =
exitloop           exit
float              float
from               from
fun                fun
ge                 >=
global             global
gt                 >
id                 {letter}({letter}|{digit})*
if                 if
int                int
left_paren         \(
left_sq_bkt        \[
le                 <=
lt                 <
minus              \-
mod                mod
mult               \*
ne                 <>
not                not
nul                null
or                 or
plus               \+
print              print
product            product
read               read
return             return
returns            ->
right_paren        \)
right_sq_bkt       \]
semicolon          \;
skip               skip
step               step
to                 to
while              while

comment            \/\/.*

%%
{whitespace}       { }

{format}           { return FORMAT; }

{int_const}        { return INT_CONST; }
{float_const}      { return FLOAT_CONST; }
{string}           { return STRING; }

{and}              { return AND; }
{assign}           { return ASSIGN; }
{colon}            { return COLON; }
{comma}            { return COMMA; }
{def}              { return DEF; }
{div}              { return DIV; }
{dot}              { return DOT; }
{else}             { return ELSE; }
{end}              { return END; }
{eq}               { return EQ; }
{exitloop}         { return EXITLOOP; }
{float}            { return FLOAT; }
{from}             { return FROM; }
{fun}              { return FUN; }
{ge}               { return GE; }
{global}           { return GLOBAL; }
{gt}               { return GT; }
{if}               { return IF; }
{int}              { return INT; }
{left_paren}       { return LEFT_PAREN; }
{left_sq_bkt}      { return LEFT_SQ_BKT; }
{le}               { return LE; }
{lt}               { return LT; }
{minus}            { return MINUS; }
{mod}              { return MOD; }
{mult}             { return MULT; }
{ne}               { return NE; }
{not}              { return NOT; }
{nul}              { return NUL; }
{or}               { return OR; }
{plus}             { return PLUS; }
{print}            { return PRINT; }
{product}          { return PRODUCT; }
{read}             { return READ; }
{return}           { return RETURN; }
{returns}          { return RETURNS; }
{right_paren}      { return RIGHT_PAREN; }
{right_sq_bkt}     { return RIGHT_SQ_BKT; }
{semicolon}        { return SEMICOLON; }
{skip}             { return SKIP; }
{step}             { return STEP; }
{to}               { return TO; }
{while}            { return WHILE; }

{id}               { return ID; }
{comment}          { }
.                  { 
                        printf("- |%s|", yytext);
                        yyerror("Invalid character encountered!"); 
                        return -1;
                   }
%%