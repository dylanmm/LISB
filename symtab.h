#include <stdlib.h>
#include <stdio.h>

enum stnodetype {VAR, FNC};

typedef struct symtab {
	enum stnodetype type;
	char* symbol;
	int value;
	struct astnode* fnptr;
	struct symtab* params;
	struct symtab* next;
} symbolTable;

void add_var(symbolTable* symtab, char* symbol, int value);
void add_fnc(symbolTable* symtab, char* symbol, struct astnode* args, struct astnode* fncptr);
int count_list(symbolTable* list);
void set_params(symbolTable* fnp, struct astnode* astp);
void set_args(symbolTable* fnp, struct astnode* astp, symbolTable* global);
symbolTable* find_var(symbolTable* symtab, char* symbol);
int fn_call(symbolTable* fnc, struct astnode* args, symbolTable* global);
int eval(symbolTable* ST, struct astnode* n);
void print_st(symbolTable* stnode);