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

