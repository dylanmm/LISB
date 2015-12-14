#include "symtab.h"
#include "ast.h"
#include <string.h>


void add_var(symbolTable* symtab, char* symbol, int value){
	symbolTable* find = (symbolTable*)malloc(sizeof(symbolTable));
	find = find_var(symtab, symbol);
	if (find) {
		find->value = value;
	} else {
		symbolTable * current = symtab;
	    while (current->next != NULL) {
	        current = current->next;
	    }
	    /* now we can add a new variable */
	    current->next = malloc(sizeof(symbolTable));
	    current->next->type = VAR;
	    current->next->symbol = symbol;
	    current->next->value = value;
	    current->next->next = NULL;	
	}

}

symbolTable* find_var(symbolTable* symtab, char* symbol){

symbolTable *cur = symtab;
	while(cur->next != NULL)
	{
		cur = cur->next;
		if (strcmp(cur->symbol, symbol) == 0) {
			return(cur);
		}
	}
	return NULL;
}

int count_list(symbolTable* list){
	int counter = 0;
	symbolTable * current = list;
    while (current->next != NULL) {
    	counter = counter + 1;
        current = current->next;
    }
	return counter;
}

void add_fnc(symbolTable* symtab, char* symbol, struct astnode* params, struct astnode* fncptr){
	symbolTable* fn = (symbolTable*)malloc(sizeof(symbolTable));
	fn->type = FNC;
  	fn->symbol = symbol;
  	fn->fnptr = fncptr;
  	fn->params = (symbolTable*)malloc(sizeof(symbolTable));
	fn->next = NULL;
  	set_params(fn->params, params);

  	while ( symtab->next != 0){
        symtab = symtab->next;
    }

    symtab->next = fn;
}

void set_params(symbolTable* fnp, struct astnode* astp) {
    if (astp->left != 0) {
    	set_params(fnp, astp->left);
    }
    if (astp->right != 0) {
    	set_params(fnp->next, astp->right);
    }
    if (astp->type == T_STR) {
    	add_var(fnp, astp->value.s, 0);
    } 
    if (astp->type == T_VAL) {
    	puts("Error, parameters can only be variables.");
    	exit(0);
    }
}

void set_args(symbolTable* fnp, struct astnode* astp, symbolTable* global) {

    if (astp->left != 0) {
    	if(astp->left->type == T_STR){
			// FIND THE FUNCTION
			symbolTable* fnfind = (symbolTable *)malloc(sizeof(symbolTable));
			fnfind = find_var(global, astp->left->value.s);

			if (strcmp(astp->left->value.s, "-") == 0 || 
				strcmp(astp->left->value.s, "+") == 0 || fnfind)
			{
				symbolTable * current = (symbolTable*)malloc(sizeof(symbolTable));
		    	current = fnp;
			    while (current->next != NULL) {
			        current = current->next;
			    }
			    current->next = (symbolTable*)malloc(sizeof(symbolTable));
			    current->next->type = VAR;
			    current->next->symbol = NULL;
			    current->next->value = eval(global, astp);
			} else {
				printf("ERROR: %s\n is undefined.", astp->left->value.s);
				exit(0);
			}
    	} else {
    		set_args(fnp, astp->left, global);
    		set_args(fnp, astp->right, global);
    	} 
    }
    if (astp->type == T_VAL) {
    	symbolTable * current = (symbolTable*)malloc(sizeof(symbolTable));
    	current = fnp;
	    while (current->next != NULL) {
	        current = current->next;
	    }
	    current->next = (symbolTable*)malloc(sizeof(symbolTable));
	    current->next->type = VAR;
	    current->next->symbol = NULL;
	    current->next->value = astp->value.i;
    }
    if (astp->type == T_STR) {
		symbolTable * current = (symbolTable*)malloc(sizeof(symbolTable));
    	current = fnp;
	    while (current->next != NULL) {
	        current = current->next;
	    }
	    current->next = (symbolTable*)malloc(sizeof(symbolTable));
	    current->next->type = VAR;
	    current->next->symbol = NULL;
	    current->next->value = eval(global, astp);
    }	

}

void print_st(symbolTable* symboltable){
	    symbolTable* current = symboltable->next;
	    while (current != NULL) {

	    	if (current->type == FNC){
	       		printf("FNC: %s \n",current->symbol);
	    	} else {
	        	printf("VAR: %s == %d\n",current->symbol, current->value);
	    	}

	        current = current->next;
	    }
}

int fn_call(symbolTable* fnc, struct astnode* args, symbolTable* global) {
	// GET ARGUMENTS
	symbolTable* arglist = (symbolTable *)malloc(sizeof(symbolTable));
	set_args(arglist,args,global);
	//print_st(arglist);
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

		// EVAL FUNCTION
		return eval(fnscope,fnc->fnptr);
	}
	return 1;
}

int eval(symbolTable* ST, node* n) {

	if (n->type == PGRM) {
		eval(ST,n->left);
		eval(ST,n->right);
	}
	if (n->type == LIST){
		if (n->left->type == T_STR){	
			if (strcmp(n->left->value.s, "+") == 0) {
				return eval(ST,n->right->left)+eval(ST,n->right->right);
			} else if (strcmp(n->left->value.s, "-") == 0) {
				return eval(ST,n->right->left)-eval(ST,n->right->right);
			} else if (strcmp(n->left->value.s, "*") == 0) {
				return eval(ST,n->right->left)*eval(ST,n->right->right);
			} else if (strcmp(n->left->value.s, "==") == 0) {
				return eval(ST,n->right->left) == eval(ST,n->right->right);
			} else if (strcmp(n->left->value.s, ">") == 0) {
				return eval(ST,n->right->left) > eval(ST,n->right->right);
			} else if (strcmp(n->left->value.s, "<") == 0) {
				return eval(ST,n->right->left) < eval(ST,n->right->right);
			} else if (strcmp(n->left->value.s, "IF") == 0) {
				if (eval(ST,n->right->left) > 0) {
					return eval(ST,n->right->right);
				} else {
					return 0;
				}
			} else if (strcmp(n->left->value.s, "PRINT") == 0) {
				printf("%i\n", eval(ST,n->right));
				return 1;
			} else if (strcmp(n->left->value.s, "SET") == 0) {
				// ADD VARIABLES
				add_var(ST, n->right->left->value.s,eval(ST, n->right->right));
				return 1;
			} else if (strcmp(n->left->value.s, "DEF") == 0) {
				// DEFINE FUNCTIONS
				add_fnc(ST, n->right->left->value.s, n->right->right->left, n->right->right->right);
				return 1;
			} else if (strcmp(n->left->value.s, "LOOP") == 0) {
				int count = eval(ST,n->right->left);
				for (int i = 0; i < count; ++i)
				{
					eval(ST,n->right->right);
				}
				return 1;
			} else {
				// FIND THE FUNCTION
				symbolTable* fnfind = (symbolTable *)malloc(sizeof(symbolTable));
				fnfind = find_var(ST, n->left->value.s);
				if (fnfind) {
					if (fnfind->type == FNC) {
						return fn_call(fnfind, n->right, ST);
					} else {
						printf("ERROR: '%s' is not a function.\n", n->left->value.s);
						exit(0);
					}
				} else {
					printf("ERROR:  '%s' is an undefined function.\n", n->left->value.s);
					exit(0);
				} 
			}
		} else {
		  eval(ST,n->left);
		}
		return eval(ST,n->right);
	}
	if (n->type == T_VAL) {
		return n->value.i;
	}
	if (n->type == T_STR) {
		// FIND THE VARIABLES
		symbolTable* fnfind = (symbolTable *)malloc(sizeof(symbolTable));
		fnfind = find_var(ST, n->value.s);
		if (fnfind) {
			return fnfind->value;
		} else {
			printf("ERROR: '%s' is an undefined variable.\n", n->value.s);
			exit(0);
		}
		return 1;
	}
	return 0;
}