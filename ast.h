#include <stdlib.h>
#include <stdio.h>

enum {
  T_ADD,
  T_MUL,
  T_VAL,
  T_KEY,
  BLOCK,
  FUNC,
  LIST,
  T_STR,
  T_SYM,
  PGRM
};

typedef struct astnode {
  int type;
  union value {
   int i;
   char* s;
   char c;
  } value; 
  struct astnode *left;
  struct astnode *right;
} node;

node* add_node(node* left, int type, node* right);
node* add_num(int value);
node* add_str(char* value);

void print_ast(node* start_node);

void graphviz(node* start_node);
void dump_ast(FILE *dotfile, node* start_node, int index);