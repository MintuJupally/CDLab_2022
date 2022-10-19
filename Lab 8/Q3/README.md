# Calculator using LEX + YACC

Grammar used:

```
A -> A + B | A - B | B
B -> B * C | B / C | C
C -> + C | - C | D
D -> E ^ C | E
E -> ( A ) | id
```

Supports the following:

1. Addition
2. Subtraction
3. Multiplication
4. Division
5. Exponentiation
6. Parantheses
7. Unary plus and minus
8. Any combination of the above

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

gcc y.tab.c -lm && ./a.out
```

**Note:** input.txt must have a new line character at the end of the input expression

## Examples

| Input Expression                       | Output |
| -------------------------------------- | ------ |
| -2 ^ - 2 - 5                           | -5.25  |
| 4 ^ 2 ^ - 1                            | 2      |
| 4 ^ - 2 ^ - 1                          | 10     |
| 1 + 2 \* 3 / 4 - 2 ^ ( 5 + 6 - 7 + 8 ) | -10.25 |
