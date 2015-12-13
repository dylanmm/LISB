/* Bison Directives */
%output  "bison.c" // Code containing yyparse()
%define parse.error verbose
%defines "bison.h" // Header file including token declarations needed by Flex
/* This section is copied verbatim to the .C file generated */
%code top {
#include "flex.h"
#include "ast.h"
node* rootnode;
/* yyerror() needs the parse-param's defined aswell */
void yyerror (char const *s);
}

%union{
  int num;
  struct astnode* node;
  char *str;
}


/* Terminal Tokens and Type Declaration */
%token <num> NUMBER
%token <str> str reserved STRING
%token <ch> ADD MUL DIV SUB OBRACK CBRACK

%type <node> lists list atoms atom


%start pgrm

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

void yyerror (char const *s){ 
	fprintf (stderr, "%s\n", s);
	exit(0);
}