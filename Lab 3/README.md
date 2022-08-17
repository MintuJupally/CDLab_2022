# Lexical Analysis (LEX)

To use the program the following commands must be available:
```
# g++
sudo apt install g++

# lex
sudo apt install lex
```

## How to compile and run the program:
```
# Q1.1
lex 1.1.l && gcc lex.yy.c && ./a.out

# Q1.3
lex 1.3.l && gcc lex.yy.c && ./a.out

# Q2
lex 2.l && gcc lex.yy.c && ./a.out

# Q3
lex 3.l && gcc lex.yy.c && ./a.out
```

## Sample execution output

1. `Q1.1 - Binary string with even number of 1s`

2. `Q1.3 - Binary strings with their integer equivalent divisible by 2`

3. `Q2 - Basic analyser`

4. `Q3 - Simple C program analyser`
