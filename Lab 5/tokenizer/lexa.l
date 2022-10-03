delim       [ \t\n]
digit       [0-9]
digits      {digit}+
letter      [A-Za-z]

id          {letter}({letter}|{digit})*
keyword     (and|then|else|end|while|if|int|do|float|not|or|print|prog|scan)
ic          {digits}
fc          {digits}(\.{digits})?(E[+-]?{digits})?
ws          {delim}+
op          (\+|-|\*|\/|=|<|>|\(|\)|\{|\}|:=|\;)


%%
{ws}            { }
{keyword}       { printf("%s\n", yytext); }
{id}            { printf("id\n"); }
{ic}            { printf("ic\n"); }
{fc}            { printf("fc\n"); }
{op}            { printf("%s\n", yytext); }
.               { printf("Unknown - %s\n", yytext); }
%%

int yywrap(){};
int main()
{
    yylex();
    return 0;
}