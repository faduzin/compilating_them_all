CC=gcc
LEX=flex
YACC=bison
CFLAGS=-Wall -Wextra -O2

all: parser

gramatica.tab.c gramatica.tab.h: gramatica.y ast.h
	$(YACC) -d -v gramatica.y

analexico.lex.c: analexico.l gramatica.tab.h
	$(LEX) -o analexico.lex.c analexico.l

parser: gramatica.tab.c analexico.lex.c ast.o
	$(CC) $(CFLAGS) -o parser gramatica.tab.c analexico.lex.c ast.o -lfl

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

clean:
	rm -f parser gramatica.tab.c gramatica.tab.h gramatica.output analexico.lex.c ast.o
