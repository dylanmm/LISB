### Installing
```
$ git clone https://github.com/dylanmm/LISB.git
$ cd LISB/
```
### Running
**Makefile**
```
CC=gcc

target:
	bison LISB.bison
	flex LISB.flex
	$(CC) -std=gnu99 -o LISB main.c flex.c bison.c ast.c symtab.c
clean:
	rm flex.c flex.h bison.c bison.h LISB ast.dot ast.png
```
**NOTE:** I'm using Flex 2.5.39 and Bison 3.0.2, symlinked flex and bison respectively. (You may have bison symlinked as bison3).
```
$ make
$ ./LISB path/to/file.lb
```
LISB expects one parameter, a path to a `.lb` file to evaluate. Examples are included in the `tests/` directory. LISB will evaluate, and then print the global symbol table.

# LISB
**Lots of Irritating Superfluous Brackets**, or as I like to refer to it, an overly complicated way to implement the Nth Fibonacci number.

LISB is an example compiler written in C, using the Flex/Bison tool stack. It is LISP-like in syntax, and was created for just evaluating integer math. Internally, an AST is created from Bison, and traversed to populate symbol tables with variable declarations and user-defined functions, and then evaluated.

#### Lexical Analysis
Here is the grammar straight from the flex file, LISB.flex:
```
/* Grammar */
%%
"["    {return OBRACK;}
"]"    {return CBRACK;}
[a-zA-Z]+ {yylval.str = strdup(yytext); return STRING;}
"+" {yylval.str = strdup(yytext); return STRING;}
"-" {yylval.str = strdup(yytext); return STRING;}
"*" {yylval.str = strdup(yytext); return STRING;}
"/" {yylval.str = strdup(yytext); return STRING;}
[<|>] {yylval.str = strdup(yytext); return STRING;}
"==" {yylval.str = strdup(yytext); return STRING;}
[0-9]+ {yylval.num = atoi(yytext); return NUMBER;}
[ \t\n]  { /* ignore whitespaces */ }
"#".*   { /* ignore whitespaces */ }
%%
```
Every token is either a string (STRING) or an integer (NUMBER). Other language examples implement reserved words like 'IF' or 'THEN', as a separate token types. As I was creating a language with more complicated features, I wanted to keep the lexical analysis and syntax parsing as simple as possible. 

A **STRING** can be any number of letters (no numbers included). Single characters and common operators, like '+', '-', and '<', are all string tokens. LISB doesn't support floating point type, so any **NUMBER** can only be an integer. LISB is not whitespace sensitive. All whitespace, including newlines, are ignored.

#### Syntax Parsing
The Bison BNF from LISB.bison:
```
%%
pgrm : lists { rootnode = $1; }

lists : list { $$ = $1; }
      | list lists { $$ = add_node($1, PGRM, $2); } 

list : OBRACK atoms CBRACK { $$ = $2; }
         
atoms : atom atoms { $$ = add_node($1, LIST, $2); }
      | atom { $$ = $1; }

atom : STRING { $$ = add_str($1); }
     | NUMBER { $$ = add_num($1); }
     | list { $$ = $1; }

%%
```
If you ignored my AST building functions, changed the bracket tokens to parentheses, and added a few declarations to the flex file before; this grammar would parse common LISP as well. As you can see, this grammar is worlds different from my [BASICD](https://github.com/dylanmm/basicd) calculator language's. 

The heart of the language is lists. A list begins with an opening bracket, `[`, and ends with a closing bracket `]`. Inside the brackets, or 'list', can be any number of strings, numbers, or other lists, in any order. Unlike LISP, I require a list to contain at least *something*, for reasons discussed in semantic analysis.

The `pgrm` rule is my way of handling more than one list not contained within each other. `[1 2] [3 4]` then translates to an AST that looks like:

![alt text](https://cdn.pbrd.co/images/2PZz6O5P.png)

This list of lists is recursive, so there can end up being lists of lists pointing to lists etc.

Browsing through the code, you'll notice a lot of Bison specific updates as well from BASICD. The main thing you might pick up on is nothing being passed to `yyparse()`. That's right, the rootnode of the AST is now a global variable that I extern to. In my research for passing in a symbol table struct, I reread the Bison docs for yyparse() and realized adding parameters to it was for the benefit of reentrant parsers. LISB is non-reentrant, so I can stick with globals. The benefit of reentrant parsers is that they can start a subroutine of themselves to parse languages within languages, like HTML mixed with Javascript. For my purposes, reentrant parsers don't necessarily provide any computational benefits. Furthermore, creating a pure reentrant parser with the Flex/Bison tool chain adds an extra layer of complication that forces you into a a very undocumented corner. 

#### Semantic Analysis
Semantic analysis, also context sensitive analysis, is a process in compiler construction, usually after syntax parsing, to gather necessary semantic information from the source code. It usually includes type checking, or makes sure a variable is declared before use which is impossible to detect in parsing. Most simple calculator languages, like my BASICD repo, implement semantic analysis while syntax parsing happens in Bison. With the addition of variables and function declaration, I've moved semantic parsing unto its own step (as it is usually described in theory).

*Everything is a function.* The syntax for LISB is very simple, everything is a "list". I evaluate with the theory that every list is a function. 

The semantics I'm checking for are:
``` [functionName argone argtwo argthree returnfunction] ```
The meat of my analysis is in the `eval` function in `symtab.c`. In the eval function I'm checking if the node to the left is a string, If it is, is it a variable or function? If it is defined, return it's integer value. If the string is not a previously defined variable or function an error is thrown. 

The I'm using the words "argument" and "returnFunction" in the example above, however remember everything is a function, and so those arguments can be functions themselves. Lists within lists within lists. `functionName` is also not required. Every function only requires its return value to be defined. To explain, let's look at the simplest list: `[0]`. This is an unnamed function that returns zero. Numbers are just functions that return themselves.

A return value is the right-most item in a list. `[0 1 3 4]`, returns 4.

However if there is a function name defined, the remaining list's are then arguments to it.
```[+ 1 3]``` returns 4 as well. 
```[+ 1 [+ 1 3]]``` returns 5.

The syntax before means any of the following is also valid:
```
[[1] [2] [3]]
[[[0]]]
[+ 1 [3]]
```
`+` is a reserved function name in LISB. There are others, and I will provide a table of them at the bottom. If you run the code above, you may notice there is no output. LISB functions return values, but that doesn't mean they output to the terminal. The function `PRINT` is the only way to output a function's return.

```
[PRINT [+ 1 [+ 1 [3]]]]
[PRINT [+ 1 [3]]]
[PRINT [3]]
[PRINT 7]
```
In this integer language what does PRINT return? It returns 1 on successful completion. Other predefined functions that do something and just return 1 are: `SET`, `IF`, and `DEF`.

### Predefined functions
| **Function** |  **Example** |  **return** | extra |
| ------------ |:-----------------|:-----------:| :-------------------------------: |
| +  | [+ 1 2] | 3 | |
| -  | [- 2 1] | 1 | |
| *  | [* 2 1] | 2 | |
| == | [== 3 5] | 0 | Compares two arguments, returns 0 if false, 1 if true. |
| > | [> 3 5] | 0 | returns 0 if false, 1 if true. |
| < | [< 3 5] | 1 | returns 0 if false, 1 if true. |
| SET  | [SET x 7] | 1  | adds x to symbol table with value 7. The function call itself returns 1 on completion|
| DEF  | [DEF foo [x y][+ x y]] | 1  | Adds a new function 'foo' to the scope's symbol table, with parameters x and y. It returns the sum of x and y when called there after. |
| PRINT  | [PRINT x 3] | 1  | Outputs 3 to the terminal, the function completion returns 1. |
| IF  | [IF [2] [4]] | 1  | If the first argument evaluates to > 0, then the second argument is then evaluates |
|   | [IF [< 3 5] [PRINT 1]] | 1  | Outputs 1 to the terminal because 3 < 5. |
| LOOP  | [LOOP 2 [PRINT 1]] | 1 1 | Will loop the second argument, equal to the number of times of the second argument|
|   | [LOOP [+ 1 1] [PRINT 1]] | 1 1 | |

These predefined functions are checked for in the eval method as well.

### Scope
Creating this language has been a lot of work. The main hurdle was implementing variables and functions (in a general sense). I know I said everything is a function before, but I'm using the words variable and functions in the pragmatic term. To keep track of variables/functions and their values I use a pretty generic link list as a symbol table. The global scope, and every function's scope is an initialized symbol table.

Defined in `symtab.h`:
```
enum stnodetype {VAR, FNC};

typedef struct symtab {
	enum stnodetype type;
	char* symbol;
	int value;
	struct astnode* fnptr;
	struct symtab* params;
	struct symtab* next;
} symbolTable;
```
This struct stores both variables and functions, acting as a scope. When adding a variable or function, I donate an entry in the list as VAR or FNC respectively. A variable is stored as the symbol (a string) and its int value. `[SET thisisavar 1]` adds symbol 'thisisavar' to the symbol table with a value of 1.

A function in the symbol table has a value of 0. `fnptr` is a pointer to the AST node after its parameter declaration. 

So if you define a function:`[DEF foo [n] [+ n 2] [+ n 5] [+ n 2] [n]]`, fnptr points to `[+ n 2] [+ n 5] [+ n 2] [n]` so it can be executed every function call. `params` is another symbol table keeping track of parameters required by a function. In the function above it would just list `n`. 

When a function is call, a lot happens.
```
int fn_call(symbolTable* fnc, struct astnode* args, symbolTable* global) {
	// GET ARGUMENTS
	symbolTable* arglist = (symbolTable *)malloc(sizeof(symbolTable));
	set_args(arglist,args,global);
	int paramcount = count_list(fnc->params);
	int argcount = count_list(arglist);

	// CHECK PARAMS VS. ARGS
	if(argcount < paramcount || argcount > paramcount) {
		printf("ERROR: %s exepecting %i arguments, but %i given.\n", fnc->symbol,paramcount,argcount);
		exit(0);
	} else {
		// SET PARAMS == ARGS
		symbolTable* fnscope = (symbolTable *)malloc(sizeof(symbolTable));
		symbolTable* currparam = fnc->params->next;
		symbolTable* currarg = arglist->next;
	    while (currparam != NULL) {
	    	add_var(fnscope,currparam->symbol,currarg->value);
	        currparam->value = currarg->value;
	        currparam = currparam->next;
	        currarg = currarg->next;
	    }
	    // ADD SELF TO SCOPE
	    symbolTable * curr = fnscope;
	    while (curr->next != NULL) {
	        curr = curr->next;
	    }
    	curr->next = malloc(sizeof(symbolTable));
	    curr->next->type = FNC;
	  	curr->next->symbol = fnc->symbol;
	  	curr->next->fnptr = fnc->fnptr;
	  	curr->next->params = fnc->params;
		curr->next->next = NULL;

		// EVAL FUNCTION
		return eval(fnscope,fnc->fnptr);
	} 

}
```

Every function call has the preceding scope (symbol table) passed into it. First the arguments provided are transferred from AST form to a link list (symbol table). During that process each argument is eval-ed so variables and other functions passed as arguments are evaluated to integers. The parameters and arguments are compared to be the same size and then mapped together into a list, the function scope. Lastly the function itself is added to the function scope as to support recursion. Then the scope and AST the fncptr points to are evaluated, and the resulting value returned. Every function call, the variables are initialized and that branch of the overall AST evaluated.

Functions don't have access to declarations outside their own scope without passing them in as arguments. A function's parameter [n] is a different [n] then one declared outside. There are more examples of this in the `test/` directory.