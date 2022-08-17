delim              [ \t\n]
digit              [0-9]
digits             {digit}+
letter             [A-Za-z]
separator          [(){}]
terminator         [;]

datatype           (int|INT|float|FLOAT)
keyword            (main|if|then|else|read|print|for|while)
id                 {letter}({letter}|{digit})*
number             {digits}(\.{digits})?(E[+-]?{digits})?
ws                 {delim}+
relop              (<|>|<=|>=|==|<>)
assignment         (=|\+=|-=|\*=|\/=)
arithmetic         (\+|-|\*|\/|\+\+|--)

%%
{ws}              { }
{separator}       { printf("( separator , %s )\n", yytext); }
{terminator}      { printf("( terminator , %s )\n", yytext); }
{keyword}         { printf("( keyword , %s )\n", yytext); }
{datatype}        { printf("( datatype , %s )\n", yytext); }
{id}              { printf("( id , %s )\n", yytext); }
{number}          { printf("( number , %s )\n", yytext); }
{relop}           { printf("( relop , %s )\n", yytext); }
{assignment}      { printf("( assignment , %s )\n", yytext); }
{arithmetic}      { printf("( arithmetic , %s )\n", yytext); }
.                 { printf("Unknown - %s\n", yytext); }
%%

int yywrap(){};
int main()
{
    extern FILE *yyin, *yyout;
 
    yyin = fopen("3.test", "r");

    yylex();
    return 0;
}