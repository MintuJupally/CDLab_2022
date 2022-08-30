# Recursive Descent Parser

To use the program the following commands must be available:

```
# g++
sudo apt install g++

# dot
sudo apt install graphviz

# xdg-open
sudo apt install xdg-utils
```

## Steps to run the program:

1. Provide start non-terminal (S) in first line of _input.txt_ file, followed by grammar production rules in the subsequent lines.

2. Compile and run the program as follows:

```
g++ Q3.cpp && ./a.out
```

## Sample execution output

1. Output for regular expression `a+b*c` for:
```
# input.txt
E
E->TE'
E'->+TE'|∈
T->FT'
T'->*FT'|∈
F->(E)|id
```
![image](https://user-images.githubusercontent.com/59505795/187394225-410134c3-acb7-4454-81d4-81e9d3057011.png)

2. Output for regular expression `a(+b)*c` for:
```
# input.txt
E
E->TE'
E'->+TE'|∈
T->FT'
T'->*FT'|∈
F->(E)|id
```
![image](https://user-images.githubusercontent.com/59505795/187394377-b1727a14-852b-45c4-b1ff-cdf7ba629d20.png)
