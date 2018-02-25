CC = gcc

CFLAGS = -Wall -g

latte: latte.tab.o lex.yy.o main.o tree.h tree.o pretty.h pretty.o symbol.h symbol.o typechecker.h typechecker.o codegen.h codegen.o
	$(CC) $(CFLAGS) lex.yy.o latte.tab.o tree.o pretty.o symbol.o typechecker.o codegen.o main.o -o latte -lfl

latte.tab.c latte.tab.h: latte.y
	bison -d latte.y --report=all

lex.yy.c: latte.l latte.tab.h
	flex latte.l

clean:
	rm -f latte; rm -f latte.tab.* lex.yy.*; rm -f *.o;
