# 3 address code generation

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

g++ -w y.tab.c && ./a.out
```
