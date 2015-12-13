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
Every token is either a string (STRING) or an integer (NUMBER). Other language examples implement reserved words like 'IF' or 'THEN', as a seperate token types, but as I was creating a language with more complicated features I wanted to keep the lexical analysis and syntax parasing as simple as possible. A STRING can be any number of letters (no numbers included). Single characters and common operators, like '+', '-', and '<', are all string tokens. LISB doesn't support floats, so any NUMBER is an integer.

#### Syntax Parsing
Most calculator languages, like my BASICD repo, implement semmantic analysis while parsing in Bison. With the addition of variables and function declaration, I've moved semantic parsing unto its own step (as it is usually described in theory).

Lexically, LISB ignores whitespace, and recongizes strings and integers. Single characters and common operators, like '+', '-', and '<', are all string tokens. The syntax for LISB is very simple, everything is a "list", or function. Every function must be encompased by and opening and closing bracket, giving it the lisp appearance. Every function only requires it's return value to be defined. To exaplain, let's look at the simpilest list: `[0]`. This is an unnamed function that returns zero.

A return value is the right-most item in a list. `[0 1 3 4]`, returns 4.*Everything is a function.. technically.* 