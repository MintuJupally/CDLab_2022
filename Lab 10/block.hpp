#include <iostream>
#include <iomanip>

using namespace std;

class Code
{
public:
    string code;

    Code()
    {
        code = "";
    }

    void append(string line)
    {
        this->code += line;
    }

    void print()
    {
        cout << code;
        // if (code.length() > 0)
        //     cout << endl;
    }
};

class Block
{
public:
    Code *code;
    Block *trueBlock;
    Block *falseBlock;
    Block *nextBlock;
    Block *next;
    Block **labelBlock;
    string var;

    Block()
    {
        code = new Code();
        trueBlock = NULL;
        falseBlock = NULL;
        nextBlock = NULL;
        next = NULL;
        labelBlock = NULL;
        var = "";
    }

    void concat(Block *newblock)
    {
        Block *ptr = this;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }

        ptr->next = newblock;
    }

    void printCode()
    {
        Block *ptr = this;

        while (ptr != NULL)
        {
            if (ptr->labelBlock == NULL)
                ptr->code->print();
            else
                (*(ptr->labelBlock))->printCode();
            ptr = ptr->next;
        }
    }
};