# Lexical Analysis

To use the program the following commands must be available:
```
# g++
sudo apt install g++
```

## How to compile and run the program:
```
g++ prog.cpp -o prog
./prog
```

## Sample execution output
![image](https://user-images.githubusercontent.com/59505795/184016699-1592f6fe-6c44-4ce9-9ba4-de03fda8ea20.png)

## Examples
1. `if input<10 then output1=100 else output2>=100`
```
( if )
( id, input )
( relop, < )
( number, 10 )
( then )
( id, output1 )
( relop, = )
( number, 100 )
( else )
( id, output2 )
( relop, >= )
( number, 100 )
```

2. `if num1 < 0 then num1 = 0 else num1 = 1`
```
( if )
( id, num1 )
( relop, < )
( number, 0 )
( then )
( id, num1 )
( relop, = )
( number, 0 )
( else )
( id, num1 )
( relop, = )
( number, 1 )
```

3. `if a<b then if a<c then min=a else min=c else if b<c then min=b else min=c`
```
( if )
( id, a )
( relop, < )
( id, b )
( then )
( if )
( id, a )
( relop, < )
( id, c )
( then )
( id, min )
( relop, = )
( id, a )
( else )
( id, min )
( relop, = )
( id, c )
( else )
( if )
( id, b )
( relop, < )
( id, c )
( then )
( id, min )
( relop, = )
( id, b )
( else )
( id, min )
( relop, = )
( id, c )
```