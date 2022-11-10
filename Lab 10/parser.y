%{
#define YYDEBUG 0

#include <iostream>

using namespace std;

int yyerror(char *s);

#include "block.hpp"

#include "y.tab.h"
#include "lex.yy.c"

int var_index = 0;
int label_index = 0;

string new_variable()
{
    string v = "v";
    v+=to_string(var_index++);
    return v;
}

string new_label()
{
    string label="_L";
    label+=to_string(label_index++);
    
    return label;
}

Block* newline()
{
    Block* block = new Block();
    Code* curr = new Code();
    curr->append("\n");
    block->code=curr;

    return block;
}

Block* colon()
{
    Block* block = new Block();
    Code* curr = new Code();
    curr->append(" :");
    block->code=curr;

    return block;
}
%}


%token AND ASSIGN COLON COMMA DEF DIV DOT ELSE END EQ EXITLOOP FLOAT FLOAT_CONST FORMAT 
%token GE GLOBAL GT ID IF INT INT_CONST LEFT_PAREN LEFT_SQ_BKT LE LT MINUS MOD MULT NE NOT NUL OR PLUS 
%token PRINT PRODUCT READ RIGHT_PAREN RIGHT_SQ_BKT SEMICOLON SKIP STRING WHILE

// Left Associativity
%left PLUS MINUS 
%left MULT DIV MOD
%left DOT
%left AND OR

// Right Associativity
%right NOT

%start code

%union{
    Block* block;
    char* code_str;
}

%type <block> prog stmtListO stmtList epsilon stmt
%type <block> assignmentStmt readStmt printStmt whileStmt ifStmt exitLoop skip
%type <block> exp id indxListO indxList dotId elsePart bExp

%%
code                :   prog                                    {
                                                                    cout<<"---------------"<<endl; 
                                                                    $1->printCode(); 
                                                                    cout<<"---------------"<<endl; 
                                                                }
prog                :   GLOBAL declList stmtListO END           {
                                                                    $$ = $3;
                                                                }
declList            :   decl declList
                    |   epsilon
decl                :   DEF typeList END
typeList            :   typeList SEMICOLON varList COLON type
                    |   varList COLON type
                    |   typeDef
varList             :   var COMMA varList
                    |   var
var                 :   ID sizeListO
sizeListO           :   sizeList
                    |   epsilon
sizeList            :   sizeList LEFT_SQ_BKT INT_CONST RIGHT_SQ_BKT
                    |   LEFT_SQ_BKT INT_CONST RIGHT_SQ_BKT
type                :   INT
                    |   FLOAT
                    |   STRING
                    |   NUL
                    |   typeDef
                    |   ID
typeDef             :   ID ASSIGN PRODUCT typeList END
stmtListO           :   stmtList
                    |   epsilon
stmtList            :   stmtList SEMICOLON stmt                 {
                                                                    Block *b = new Block();
                                                                    b->nextBlock = new Block();
                                                                    $$ = b;

                                                                    if($1->nextBlock==NULL)
                                                                    {
                                                                        $1->nextBlock = new Block();
                                                                    }
                                                                    $1->nextBlock->code->append(new_label());
                                                                    $3->nextBlock = b->nextBlock;

                                                                    b->concat($1);

                                                                    Block *_1_next = new Block();
                                                                    _1_next->labelBlock = &($1->nextBlock);

                                                                    b->concat(_1_next);
                                                                    b->concat(colon());
                                                                    b->concat(newline());
                                                                    b->concat($3);
                                                                }
                    |   stmt                                    {
                                                                    if($1->nextBlock == NULL)
                                                                    {   
                                                                        $1->nextBlock = new Block();
                                                                    }
                                                                    $1->nextBlock->code->append(new_label());

                                                                    Block *b = new Block();
                                                                    b->concat($1);

                                                                    Block *_1_next = new Block();
                                                                    _1_next->labelBlock = &($1->nextBlock);
                                                                    b->concat(_1_next);
                                                                    b->concat(colon());
                                                                    b->concat(newline());

                                                                    $$ = b;
                                                                }
stmt                :   assignmentStmt
                    |   readStmt
                    |   printStmt
                    |   ifStmt
                    |   whileStmt
                    |   exitLoop                        { $$ = new Block(); }
                    |   skip                        { $$ = new Block(); }
assignmentStmt      :   dotId ASSIGN exp            {
                                                        string c = $1->var;
                                                        c+=" = ";
                                                        c+=$3->var;
                                                        Code* curr = new Code();
                                                        curr->append(c);

                                                        Block *b = new Block();
                                                        b->code = curr;

                                                        b->concat(newline());
                                                        $3->concat(b);
                                                        $$ = $3;
                                                    }           
dotId               :   id                          
                    |   id DOT dotId                {
                                                        string c = $1->var;
                                                        c+=".";
                                                        c+=$3->var;
                                                        Code* curr = new Code();
                                                        curr->append(c);

                                                        $$ = new Block();
                                                        $$->code = curr;
                                                        $$->var = c;
                                                    }   
readStmt            :   READ FORMAT exp             {
                                                        string c = $<code_str>1;
                                                        c+=" ";
                                                        c+=$<code_str>2;
                                                        c+=" ";
                                                        c+=$3->code->code;

                                                        Code* curr = new Code();
                                                        curr->append(c);

                                                        $$ = new Block();
                                                        $$->code = curr;
                                                    }
printStmt           :   PRINT STRING                {
                                                        string c = $<code_str>1;
                                                        c+=" ";
                                                        c+=$<code_str>2;

                                                        Code* curr = new Code();
                                                        curr->append(c);

                                                        $$ = new Block();
                                                        $$->code = curr;
                                                    }
                    |   PRINT FORMAT exp            {
                                                        string c = $<code_str>1;
                                                        c+=" ";
                                                        c+=$<code_str>2;
                                                        c+=" ";
                                                        c+=$3->code->code;

                                                        Code* curr = new Code();
                                                        curr->append(c);

                                                        $$ = new Block();
                                                        $$->code = curr;
                                                    }
ifStmt              :   IF bExp COLON stmtList elsePart END     {
                                                                    $2->trueBlock->code->append(new_label());

                                                                    $$ = new Block();
                                                                    $$->nextBlock = new Block();

                                                                    if($5==NULL)
                                                                    {
                                                                        $2->falseBlock = $$->nextBlock;

                                                                        $4->nextBlock = $$->nextBlock;

                                                                        $$->concat($2);

                                                                        Block* _2_true = new Block();
                                                                        _2_true->labelBlock = &($2->trueBlock);

                                                                        $$->concat(_2_true);
                                                                        $$->concat(colon());
                                                                        $$->concat(newline());
                                                                        
                                                                        $$->concat($4);
                                                                    }
                                                                    else
                                                                    {
                                                                    }
                                                                }
elsePart            :   ELSE stmtList                           {   $$ = $2;    }
                    |   epsilon                                 {   $$ = NULL;  }
whileStmt           :   WHILE bExp COLON stmtList END           {
                                                                    string begin = new_label();
                                                                    $2->trueBlock->code->append(new_label());

                                                                    Block* b = new Block();
                                                                    $$ = b;
                                                                    b->nextBlock = new Block();

                                                                    $2->falseBlock = b->nextBlock;
                                                                    if($4->nextBlock==NULL)
                                                                        $4->nextBlock = new Block();
                                                                    $4->nextBlock->code->append(begin);

                                                                    Block* _begin = new Block();
                                                                    _begin->labelBlock = &($4->nextBlock);

                                                                    b->concat(_begin);
                                                                    b->concat(colon());
                                                                    b->concat(newline());

                                                                    b->concat($2);

                                                                    Block* b_true = new Block();
                                                                    b_true->labelBlock = &($2->trueBlock);

                                                                    b->concat(b_true);
                                                                    b->concat(colon());
                                                                    b->concat(newline());
                                                                    b->concat($4);

                                                                    string c = "goto ";
                                                                    Code* curr = new Code();
                                                                    curr->append(c);
                                                                    Block* nb = new Block();
                                                                    nb->code = curr;

                                                                    b->concat(nb);

                                                                    Block* _begin2 = new Block();
                                                                    _begin2->labelBlock = &($4->nextBlock);

                                                                    b->concat(_begin2);
                                                                    b->concat(newline());
                                                                }
exitLoop            :   EXITLOOP
skip                :   SKIP
id                  :   ID indxListO                {
                                                        string c = $<code_str>1;
                                                        c+=$2->code->code;
                                                        Code* curr = new Code();
                                                        curr->append(c);

                                                        $$ = new Block();
                                                        // $$->code = curr;
                                                        $$->var = c;
                                                    }   
indxListO           :   indxList
                    |   epsilon
indxList            :   indxList LEFT_SQ_BKT exp RIGHT_SQ_BKT       {
                                                                        string c = $1->code->code;
                                                                        c+=$<code_str>2;
                                                                        c+=$3->var;
                                                                        c+=$<code_str>4;
                                                                        Code* curr = new Code();
                                                                        curr->append(c);

                                                                        $$ = new Block();
                                                                        $$->code = curr;
                                                                    }   
                    |   LEFT_SQ_BKT exp RIGHT_SQ_BKT                {
                                                                        string c = $<code_str>1;
                                                                        c+=$2->var;
                                                                        c+=$<code_str>3;
                                                                        Code* curr = new Code();
                                                                        curr->append(c);

                                                                        $$ = new Block();
                                                                        $$->code = curr;
                                                                    }   
bExp                :   bExp OR bExp                        {
                                                                Block* b = new Block();
                                                                $$ = b;
                                                                b->trueBlock = new Block();
                                                                b->falseBlock = new Block();

                                                                $1->trueBlock = b->trueBlock;
                                                                $1->falseBlock->code->append(new_label());
                                                                $3->trueBlock = b->trueBlock;
                                                                
                                                                $3->falseBlock = b->falseBlock;
                                                                b->concat($1);
                                                                
                                                                Block* _1_false = new Block();
                                                                _1_false->labelBlock = &($1->falseBlock);
                                                                b->concat(_1_false);
                                                                b->concat(colon());
                                                                b->concat(newline());

                                                                b->concat($3);
                                                            }
                    |   bExp AND bExp                       {
                                                                Block* b = new Block();
                                                                $$ = b;
                                                                b->trueBlock = new Block();
                                                                b->falseBlock = new Block();

                                                                $1->trueBlock->code->append(new_label());
                                                                $1->trueBlock = b->falseBlock;
                                                                $3->trueBlock = b->trueBlock;
                                                                
                                                                $3->falseBlock = b->falseBlock;
                                                                b->concat($1);
                                                                
                                                                Block* _1_true = new Block();
                                                                _1_true->labelBlock = &($1->trueBlock);
                                                                b->concat(_1_true);
                                                                b->concat(colon());
                                                                b->concat(newline());

                                                                b->concat($3);
                                                                b->printCode();
                                                            }
                    |   NOT bExp                            {
                                                                $$ = new Block();
                                                                $$->trueBlock = new Block();
                                                                $$->falseBlock = new Block();

                                                                $2->trueBlock = $$->falseBlock;
                                                                $2->falseBlock = $$->trueBlock;
                                                            }
                    |   LEFT_PAREN bExp RIGHT_PAREN         {   $$ = $2;    }
                    |   exp relOP exp                       {
                                                                Block *b = new Block();

                                                                b->concat($1);
                                                                b->concat($3);

                                                                Block *b1 = new Block();
                                                                Code* curr1 = new Code();
                                                                string c1 = "if ";
                                                                c1+=$1->var;
                                                                c1+=" ";
                                                                c1+=$<code_str>2;
                                                                c1+=" ";
                                                                c1+=$3->var;
                                                                c1+=" goto ";

                                                                curr1->append(c1);
                                                                b1->code = curr1;

                                                                b->concat(b1);
                                                                b->trueBlock = new Block();

                                                                Block *b_true = new Block();
                                                                b_true->labelBlock = &(b->trueBlock);
                                                                b->concat(b_true);

                                                                b->concat(newline());

                                                                Block* b2 = new Block();
                                                                Code* curr2 = new Code();
                                                                string c2 = "goto ";
                                                                curr2->append(c2);
                                                                b2->code = curr2;

                                                                b->falseBlock = new Block();

                                                                Block *b_false = new Block();
                                                                b_false->labelBlock = &(b->falseBlock);
                                                                
                                                                b2->concat(b_false);

                                                                b->concat(b2);
                                                                b->concat(newline());

                                                                $$ = b;
                                                            }
relOP               :   EQ
                    |   LE
                    |   LT
                    |   GE
                    |   GT
                    |   NE
exp                 :   exp PLUS exp                        {
                                                                $1->concat($3);

                                                                string var = new_variable();
                                                                string c = var;
                                                                c+=" = ";
                                                                c+=$1->var;
                                                                c+=" + ";
                                                                c+=$3->var;
                                                                
                                                                Code* curr = new Code();
                                                                curr->append(c);

                                                                Block* b = new Block();
                                                                b->code = curr;
                                                                
                                                                b->concat(newline());
                                                                $1->concat(b);
                                                                
                                                                $$ = $1;
                                                                $$->var = var;
                                                            }
                    |   exp MINUS exp                       {
                                                                $1->concat($3);

                                                                string var = new_variable();
                                                                string c = var;
                                                                c+=" = ";
                                                                c+=$1->var;
                                                                c+=" - ";
                                                                c+=$3->var;
                                                                
                                                                Code* curr = new Code();
                                                                curr->append(c);

                                                                Block* b = new Block();
                                                                b->code = curr;
                                                                
                                                                b->concat(newline());
                                                                $1->concat(b);
                                                                
                                                                $$ = $1;
                                                                $$->var = var;
                                                            }
                    |   exp MULT exp                        {
                                                                $1->concat($3);

                                                                string var = new_variable();
                                                                string c = var;
                                                                c+=" = ";
                                                                c+=$1->var;
                                                                c+=" * ";
                                                                c+=$3->var;
                                                                
                                                                Code* curr = new Code();
                                                                curr->append(c);

                                                                Block* b = new Block();
                                                                b->code = curr;
                                                                
                                                                b->concat(newline());
                                                                $1->concat(b);
                                                                
                                                                $$ = $1;
                                                                $$->var = var;
                                                            }
                    |   exp DIV exp                         {
                                                                $1->concat($3);

                                                                string var = new_variable();
                                                                string c = var;
                                                                c+=" = ";
                                                                c+=$1->var;
                                                                c+=" / ";
                                                                c+=$3->var;
                                                                
                                                                Code* curr = new Code();
                                                                curr->append(c);

                                                                Block* b = new Block();
                                                                b->code = curr;
                                                                
                                                                b->concat(newline());
                                                                $1->concat(b);
                                                                
                                                                $$ = $1;
                                                                $$->var = var;
                                                            }
                    |   exp MOD exp                         {
                                                                $1->concat($3);

                                                                string var = new_variable();
                                                                string c = var;
                                                                c+=" = ";
                                                                c+=$1->var;
                                                                c+=" % ";
                                                                c+=$3->var;
                                                                
                                                                Code* curr = new Code();
                                                                curr->append(c);

                                                                Block* b = new Block();
                                                                b->code = curr;
                                                                
                                                                b->concat(newline());
                                                                $1->concat(b);
                                                                
                                                                $$ = $1;
                                                                $$->var = var;
                                                            }
                    |   MINUS exp
                    |   PLUS exp
                    |   exp DOT exp
                    |   LEFT_PAREN exp RIGHT_PAREN
                    |   id
                    |   INT_CONST                           {   
                                                                Code* c = new Code();
                                                                string var = new_variable();
                                                                c->append(var);
                                                                c->append(" = ");
                                                                c->append($<code_str>1);
                                                                $$ = new Block();

                                                                $$->code = c;
                                                                $$->var = var;
                                                                $$->concat(newline());
                                                            }
                    |   FLOAT_CONST                         {   
                                                                Code* c = new Code();
                                                                string var = new_variable();
                                                                c->append(var);
                                                                c->append(" = ");
                                                                c->append($<code_str>1);
                                                                $$ = new Block();

                                                                $$->code = c;
                                                                $$->var = var;
                                                                $$->concat(newline());
                                                            }
epsilon             :                                       { $$ = new Block(); }
%%  

int yyerror(char *s)    
{   
	fprintf(stderr, "%s in line no : %d - %s\n", s, yylineno, yytext);
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