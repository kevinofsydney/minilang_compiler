#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "pretty.h"
#include "symbol.h"
#include "typechecker.h"
#include "codegen.h"

extern int yylex();
extern int yylineno;
extern int yyparse();
extern char* yytext;
extern FILE *yyin;

int g_tokens;
PROGRAM *g_program = NULL;
SYMBOLTABLE *g_table;

int main(int argc, char* argv[])
{
	//If no mode is given
	if (argc < 2) 
	{
		printf("Not enough arguments given. Exiting.\n");
		exit(1);
	}

	if (strcmp(argv[1], "scan") == 0)
	{
		g_tokens = 0;
		while(yylex()) {}
		printf("OK\n");
	}
	else if (strcmp(argv[1], "tokens") == 0)
	{
		g_tokens = 1;
		while(yylex()){}
	}
	else if (strcmp(argv[1], "parse") == 0)
	{
		g_tokens = 0; 
		yyparse();
		printf("OK\n");
	}
	else if (strcmp(argv[1], "pretty") == 0)
	{
		g_tokens = 0;
		yyparse();
		prettyPROGRAM(g_program);
	}
	else if (strcmp(argv[1], "symbol") == 0)
	{
		g_tokens = 0;
		yyparse();
		makePROGRAMstable(g_program, 1); 
	}
	else if (strcmp(argv[1], "typecheck") == 0)
	{
		g_tokens = 0;
		yyparse();
		makePROGRAMstable(g_program, 0);
		tcPROGRAM(g_program);
		printf("OK\n");
	}
	else if (strcmp(argv[1], "codegen") == 0)
	{
		g_tokens = 0;
		yyparse();
		makePROGRAMstable(g_program, 0);
		tcPROGRAM(g_program);

		char* outputFileName = malloc((strlen(argv[2]) + 1) * sizeof(char));

		for (int i = 0; (argv[2][i] != '.'); i++)
				outputFileName[i] = argv[2][i];

		strcat(outputFileName, ".c");
		cgPROGRAM(g_program, outputFileName);
		printf("OK\n");
	}
	return 0;
}
