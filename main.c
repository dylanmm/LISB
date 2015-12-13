#include <stdio.h>
#include "bison.h"
#include "flex.h"
#include "ast.h"
#include "symtab.h"



int main(int argc, char **argv) {	
	// SYMBOL TABLE	
	symbolTable * gscope = malloc(sizeof(symbolTable));
	gscope->next = NULL;

	// yyin Defined in flex.h: extern FILE *yyin, *yyout;
	yyin = fopen(argv[1], "r");

	if (yyin == NULL) {
		puts("No source file given or found");
		exit(0);
	} 

	// AST
	extern node* rootnode;

	// GET AST AND SYMBOL TABLE
  	yyparse();

	eval(gscope,rootnode); 

	puts("\nGlobal Symbol Table:");
	print_st(gscope);

  	//graphviz(rootnode);
}