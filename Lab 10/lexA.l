%{
#include <stdio.h>
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
le                 \<=
lt                 \<
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

{format}           { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{format} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return FORMAT; 
                    }

{int_const}        { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{int_const} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return INT_CONST; 
                    }
{float_const}      { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{float_const} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return FLOAT_CONST; 
                    }
{string}           { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{string} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return STRING; 
                    }

{and}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{and} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return AND; 
                    }
{assign}           { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{assign} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return ASSIGN; 
                    }
{colon}            { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{colon} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return COLON; 
                    }
{comma}            { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{comma} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return COMMA; 
                    }
{def}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{def} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return DEF; 
                    }
{div}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{div} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return DIV; 
                    }
{dot}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{dot} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return DOT; 
                    }
{else}             { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{else} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return ELSE; 
                    }
{end}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{end} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return END; 
                    }
{eq}               { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{eq} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return EQ; 
                    }
{exitloop}         { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{exitloop} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return EXITLOOP; 
                    }
{float}            { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{float} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return FLOAT; 
                    }
{ge}               { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{ge} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return GE; 
                    }
{global}           { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{global} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return GLOBAL; 
                    }
{gt}               { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{gt} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return GT; 
                    }
{if}               { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{if} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return IF; 
                    }
{int}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{int} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return INT; 
                    }
{left_paren}       { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{left_paren} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return LEFT_PAREN; 
                    }
{left_sq_bkt}      { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{left_sq_bkt} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return LEFT_SQ_BKT; 
                    }
{le}               { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{le} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return LE; 
                    }
{lt}               { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{lt} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return LT; 
                    }
{minus}            { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{minus} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return MINUS; 
                    }
{mod}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{mod} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return MOD; 
                    }
{mult}             { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{mult} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return MULT; 
                    }
{ne}               { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{ne} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return NE; 
                    }
{not}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{not} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return NOT; 
                    }
{nul}              { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{nul} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return NUL; 
                    }
{or}               { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{or} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return OR; 
                    }
{plus}             { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{plus} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return PLUS; 
                    }
{print}            { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{print} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return PRINT; 
                    }
{product}          { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{product} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return PRODUCT; 
                    }
{read}             { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{read} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return READ; 
                    }
{right_paren}      { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{right_paren} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return RIGHT_PAREN; 
                    }
{right_sq_bkt}     { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{right_sq_bkt} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return RIGHT_SQ_BKT; 
                    }
{semicolon}        { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{semicolon} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return SEMICOLON; 
                    }
{skip}             { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{skip} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return SKIP; 
                    }
{while}            { 
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{while} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return WHILE; 
                    }

{id}               {
                         char *temp = (char*)malloc((strlen(yytext)+1)*sizeof(char));
                         strcpy(temp, yytext);
                         // printf("{id} - %s\n", temp); 
                         yylval.code_str = temp; 
                         return ID; 
                    }
{comment}          { }
.                  { 
                        printf("- |%s|", yytext);
                        yyerror("Invalid character encountered!"); 
                        return -1;
                   }
%%