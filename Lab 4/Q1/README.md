# Recursive Descent Parser

To use the program the following commands must be available:

```
# g++
sudo apt install g++
```

## Steps to run the program:

1. Provide grammar production rules in the _input.txt_ file.

2. Compile and run the program as follows:

```
g++ Q1.cpp && ./a.out
```

## Sample execution output

1. Output for:

```
# input.txt
E->E+T|T
T->T*F|F
F->(E)|id
```
