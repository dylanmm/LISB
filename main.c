#include <stdio.h>
#include "bison.h"
#include "flex.h"
#include "ast.h"
#include "symtab.h"



int main(int argc, char **argv) {		
	// SYMBOL TABLE	
	symbolTable * gscope = (symbolTable * )malloc(sizeof(symbolTable));

	yyin = fopen(argv[1], "r");

	if (yyin == NULL) {
		puts("No source file (.bd) given");
		exit(0);
	} 

	// AST
	extern node* rootnode;

	// GET AST AND SYMBOL TABLE
  	yyparse();

	eval(gscope,rootnode); 

	puts("\nGlobal Symbol Table:");
	print_st(gscope);

  	//raphviz(rootnode);
}