delim       [ \t\n]
digit       [0-9]
digits      {digit}+
letter      [A-Za-z]

id          {letter}({letter}|{digit})*
keyword     (if|then|else)
number      {digits}(\.{digits})?(E[+-]?{digits})?
ws          {delim}+
relop       (<|>|<=|>=|=|<>)


%%
{ws}            { }
{keyword}       { printf("( %s )\n", yytext); }
{id}            { printf("( id , %s )\n", yytext); }
{number}        { printf("( number , %s )\n", yytext); }
{relop}         { printf("( relop , %s )\n", yytext); }
.               { printf("Unknown - %s\n", yytext); }
%%

int yywrap(){};
int main()
{
    yylex();
    return 0;
}