%{
#define YYDEBUG 0

#include <stdio.h>
#include <stdlib.h>

int yyerror(char *s);

#include "lex.yy.c"
%}


%token AND ASSIGN COLON COMMA DEF DIV DOT ELSE END EQ EXITLOOP FLOAT FLOAT_CONST FORMAT FROM FUN GE GLOBAL GT ID IF INT INT_CONST LEFT_PAREN LEFT_SQ_BKT LE LT MINUS MOD MULT NE NOT NUL OR PLUS PRINT PRODUCT READ RETURN RETURNS RIGHT_PAREN RIGHT_SQ_BKT SEMICOLON SKIP STEP STRING TO WHILE

// Left Associativity
%left PLUS MINUS MULT DIV MOD
%left DOT
%left AND OR

// Right Associativity
%right NOT

%start prog

%%
prog :              GLOBAL declList stmtListO END
declList :          decl declList
                    | epsilon
decl :              DEF typeList END
                    | FUN funDef END
typeList :          typeList SEMICOLON varList COLON type
                    | varList COLON type
                    | typeDef
varList :           var COMMA varList
                    | var
var :               ID sizeListO
sizeListO :         sizeList
                    | epsilon
sizeList :          sizeList LEFT_SQ_BKT INT_CONST RIGHT_SQ_BKT
                    | LEFT_SQ_BKT INT_CONST RIGHT_SQ_BKT
type :              INT
                    | FLOAT
                    | STRING
                    | NUL
                    | typeDef
                    | ID
typeDef :           ID ASSIGN PRODUCT typeList END
funDef :            funID fparamListO RETURNS type funBody
funID :             ID
fparamListO :       fparamList
                    | epsilon
fparamList :        fparamList SEMICOLON pList COLON type
                    | pList COLON type
pList :             pList COMMA idP
                    | idP
idP :               ID sizeListO
funBody :           declList stmtListO
stmtListO :         stmtList
                    | epsilon
stmtList :          stmtList SEMICOLON stmt
                    | stmt
stmt :              assignmentStmt
                    | readStmt
                    | printStmt
                    | ifStmt
                    | whileStmt
                    | loopStmt
                    | callStmt
                    | returnStmt
                    | exitLoop
                    | skip
assignmentStmt :    dotId ASSIGN exp
dotId :             id
                    | id DOT dotId
readStmt :          READ FORMAT exp
printStmt :         PRINT STRING
                    | PRINT FORMAT exp
ifStmt :            IF bExp COLON stmtList elsePart END
elsePart :          ELSE stmtList
                    | epsilon
whileStmt :         WHILE bExp COLON stmtList END
loopStmt :          FROM id ASSIGN exp TO exp stepPart COLON stmtListO END
stepPart :          STEP exp
                    | epsilon
callStmt :          LEFT_PAREN ID COLON actParamList RIGHT_PAREN
returnStmt :        RETURN expO 
expO :              exp
                    | epsilon
exitLoop :          EXITLOOP
skip :              SKIP
id :                ID indxListO
indxListO :         indxList
                    | epsilon
indxList :          indxList LEFT_SQ_BKT exp RIGHT_SQ_BKT
                    | LEFT_SQ_BKT exp RIGHT_SQ_BKT
bExp :              bExp OR bExp
                    | bExp AND bExp
                    | NOT bExp
                    | LEFT_PAREN bExp RIGHT_PAREN
                    | exp relOP exp
relOP :             EQ
                    | LE
                    | LT
                    | GE
                    | GT
                    | NE
exp :               exp PLUS exp
                    | exp MINUS exp
                    | exp MULT exp
                    | exp DIV exp
                    | exp MOD exp
                    | MINUS exp
                    | PLUS exp
                    | exp DOT exp
                    | LEFT_PAREN exp RIGHT_PAREN
                    | id
                    | LEFT_PAREN ID COLON actParamListO RIGHT_PAREN
                    | INT_CONST
                    | FLOAT_CONST
actParamListO :     actParamList
                    | epsilon
actParamList :      actParamList COMMA exp
                    | exp 
epsilon :           
%%

extern int yylex(), yyparse();

int yyerror(char *s)
{
	fprintf(stderr, "%s in line no : %d\n", s, yylineno);
	exit(-1);
}


int main()
{
#if YYDEBUG
    yydebug = 1;
#endif

    extern FILE *yyin;
    yyin = fopen("input.txt", "r");

    yyparse();

    printf("Successfully parsed!\n");

    return 0;
}