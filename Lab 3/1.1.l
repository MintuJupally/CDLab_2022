%%
exit            { printf("Terminating\n"); return 0; }
0*(0*10*10*)*   { printf("Accepted"); }
.*              { printf("Rejected"); }
%%

int yywrap(){};
int main()
{
    yylex();
    return 0;
}