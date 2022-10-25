# Intermediate Representations

| Q   |                              |
| --- | ---------------------------- |
| Q1  | Abstract Syntax Tree (AST)   |
| Q2  | Directed Acyclic Graph (DAG) |
| Q3  | 3-address code               |

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

## Sample Execution
Considered input: `1*1 + 1*1 + 2*2 + 2*2`

![image](https://user-images.githubusercontent.com/59505795/197732902-1b144fff-70e8-4a51-9b8b-98dc7d0cd068.png)

The first line of the above image shows the value of the input expression evaluated.  
The rest lines show the 3-address code for the input given.

#### Abstract Syntax Tree
![AST](https://user-images.githubusercontent.com/59505795/197733376-fdfb99d4-9ef5-4db6-a3f4-0b065e05ebca.png)

#### Directed Acyclic Graph
![DAG](https://user-images.githubusercontent.com/59505795/197733603-560bd41d-c373-491e-afd2-790871ce0b1f.png)



