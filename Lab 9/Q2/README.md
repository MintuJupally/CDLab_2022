# Simple Calculator using LEX + YACC

Grammar from Q1 used:

```
A -> A + B | A - B | B
B -> B * C | B / C | C
C -> ( A ) | + C | - C | id
```

Supports the following:

1. Addition
2. Subtraction
3. Multiplication
4. Division
5. Parantheses
6. Unary plus and minus

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

## Examples

| Input Expression                 | Output |
| -------------------------------- | ------ |
| 2 + 3 \* 4                       | 14     |
| - 2 + 3 \* 4                     | 10     |
| - 2 + 3 \* 4                     | 10     |
| 2.5 \* - 2.5 + 2 \* 5 / -2.5     | -10.25 |
| 2.5 \* ( - 2.5 + 2 ) \* 5 / -2.5 | 2.5    |
