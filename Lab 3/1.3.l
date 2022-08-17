%%
exit            { printf("Terminating\n"); return 0; }
(0|1)*0         { printf("Accepted"); }
.*              { printf("Rejected"); }
%%

int yywrap(){};
int main()
{
    yylex();
    return 0;
}