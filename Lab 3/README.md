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
![1.1_output](https://user-images.githubusercontent.com/59505795/185085040-c464a76d-9d60-44ae-a67d-ace1bd4a1045.png)

2. `Q1.3 - Binary strings with their integer equivalent divisible by 2`
![1.3_output](https://user-images.githubusercontent.com/59505795/185085243-4740a2fe-29ba-44f0-bf5e-1cc7bced619c.png)

3. `Q2 - Basic analyser`
![2_output](https://user-images.githubusercontent.com/59505795/185085574-415675eb-2601-438a-8d8d-f76a0a2d4dcb.png)

4. `Q3 - Simple C program analyser`
![3_output_1](https://user-images.githubusercontent.com/59505795/185086140-e8241822-f1d7-4601-9962-c7fb6a9d3438.png)
![3_output_2](https://user-images.githubusercontent.com/59505795/185086217-d9b59c5c-aafa-470b-93d4-4939beb2ff05.png)

