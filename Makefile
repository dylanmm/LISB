CC=gcc

target:
	bison3 LISB.bison
	flex LISB.flex
	$(CC) -std=gnu99 -o LISB main.c flex.c bison.c ast.c symtab.c
clean:
	rm flex.c flex.h bison.c bison.h LISB ast.dot ast.png