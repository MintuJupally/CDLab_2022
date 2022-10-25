# Intermediate Representations

| Q   |                              |
| --- | ---------------------------- |
| Q1  | Abstract Syntax Tree (AST)   |
| Q2  | Directed Acyclic Graph (DAG) |
| Q3  | 3 address code               |

To use the program the following commands must be available:

```
# g++
sudo apt install g++

# lex
sudo apt install lex

# yacc
sudo apt install bison
```

## How to compile and run the program:

```
lex lexA.l && yacc parser.y -d

gcc y.tab.c && ./a.out
```

**Note:** input.txt must have a new line character at the end of the input expression
