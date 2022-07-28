# NFA Generator from Regular Expression

To use the program the following commands must be available:
```
# g++
sudo apt install g++

# dot
sudo apt install graphviz

# xdg-open
sudo apt install xdg-utils
```

## How to compile and run the program:
```
g++ prog.cpp -o prog
./prog
```

## Sample execution output
Output for regular expression `a + bc*`
![terminal_output](https://user-images.githubusercontent.com/59505795/181474132-c2c0ea30-8733-4748-8c15-99ef5021da26.png)
![nfa_graph](https://user-images.githubusercontent.com/59505795/181474543-78ff1ace-f322-44da-b715-49eb7a7110a7.png)

Invalidating wrong regular expressions
![terminal_output_invalid](https://user-images.githubusercontent.com/59505795/181474863-03088232-66f6-4ab2-bf5e-505c5a90b24d.png)
