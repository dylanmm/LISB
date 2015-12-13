### Installing
```
$ git clone https://github.com/schacon/ticgit
$ cd LISB
```
### Running
```
$ make
$ ./LISB path/to/file.lb
```

# LISB
**Lots of Irritating Superfluous Brackets** a.k.a an overly complicated way to implement the Nth Fibonacci numnber.

LISB is an example calculator compiler using the Flex/Bison tool stack. It is LISP-like in syntax, and was created for integer math. An AST is created from Bison, and traversed to populate symbol tables with variable declarations and user-defined functions.

*Everything is a function.. technically.* Lexically, LISB ignores whitespace, and recongizes strings and integers. Single characters and common operators, like '+', '-', and '<', are all string tokens. The syntax for LISB is very simple, everything is a "list", or function. Every function must be encompased by and opening and closing bracket, giving it the lisp appearance. Every function only requires it's return value to be defined. To exaplain, let's look at the simpilest list: `[0]`. This is an unnamed function that returns zero.

A return value is the right-most item in a list. `[0 1 3 4]`, returns 4.