#include "ast.h"

node* add_node(node* left, int type, node* right){
  node* n = malloc(sizeof(node));
  n->type = type;
  n->value.i = 0;
  n->left = left;
  n->right = right;
  return n;
}

node* add_num(int value){
  node* n = malloc(sizeof(node));
  n->type = T_VAL;
  n->value.i = value;
  n->left = NULL;
  n->right = NULL;

  return n;
}

node* add_str(char* value){
  node* n = malloc(sizeof(node));
  n->type = T_STR;
  n->value.s = value;
  n->left = NULL;
  n->right = NULL;

  return n;
}


void graphviz(node* start_node) {
	FILE *dotfile = fopen("ast.dot", "w");
	fprintf(dotfile, "digraph tree {\n");
	dump_ast(dotfile, start_node, 1);
	fprintf(dotfile, "}\n");
	fclose(dotfile);

	// OS X
  	//system("dot -Tpng ast.dot > ast.png && open ast.png");

  	// Linux
  	system("dot -Tpng ast.dot > ast.png && xdg-open ast.png");
}

void dump_ast(FILE *dotfile, node* start_node, int index){
	switch(start_node->type) {
		case T_STR :
		    fprintf(dotfile, "node%i [label=\"%s\"];\n", index,start_node->value.s);
		    break;
		case T_VAL :
		    fprintf(dotfile, "node%i [label=\"%i\"];\n", index,start_node->value.i);
		    break;
		case BLOCK :
			fprintf(dotfile, "node%i [label=\"list\"];\n", index);
		    break;
		case LIST :
			fprintf(dotfile, "node%i [label=\"LIST\"];\n", index);
		    break;
		case PGRM :
			fprintf(dotfile, "node%i [label=\"LISTS\"];\n", index);
		    break;
		 default :
	    	fprintf(dotfile, "node%i;\n", index);
	}
	if (start_node->left > 0){	
			fprintf(dotfile, "node%i->node%i;\n", index, 2*index);
		dump_ast(dotfile, start_node->left,2*index);
	}
	if (start_node->right > 0){
		fprintf(dotfile, "node%i->node%i;\n", index, 2*index+1);
		dump_ast(dotfile, start_node->right,2*index+1);
	}
}