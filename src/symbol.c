#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

extern SYMBOLTABLE *g_table;

int Hash (char *str){
	unsigned int hash = 0;
	while (*str) hash = (hash << 1) + *str++; 
	return hash % HashSize;
}

SYMBOLTABLE *initSymbolTable ()
{
	SYMBOLTABLE *t = malloc(sizeof(SYMBOLTABLE));

	for (int i = 0; i < HashSize; i++)
		t->table[i] = NULL;
	
	t->parent = NULL;
	return t;
}

SYMBOLTABLE *scopeSymbolTable (SYMBOLTABLE *s)
{
	SYMBOLTABLE *t = initSymbolTable();
	t->parent = s;
	return t;
}

SYMBOL *putSymbol (SYMBOLTABLE *t, char *identifier, TYPE *type, int lineno)
{
	int i = Hash(identifier);
	for (SYMBOL *s = t->table[i]; s; s = s->next) {
		if (strcmp(s->identifier, identifier) == 0) 
		{ 
			fprintf(stderr, "Error: Illegal redeclaration of identifier '%s' on line %d.\n", identifier, lineno);
			exit(1);
		}
	}
	
	SYMBOL *s = malloc(sizeof(SYMBOL));
	s->identifier = malloc((strlen(identifier) + 1) * sizeof(char));
	strcpy(s->identifier, identifier);
	s->type = type;
	s->next = t->table[i];
	t->table[i] = s;
	
	return s;
}

SYMBOL *getSymbol (SYMBOLTABLE *t, char *identifier){
	int i = Hash(identifier);
	
	//Check the current scope
	for (SYMBOL *s = t->table[i]; s; s = s->next) 
	{
		if (strcmp(s->identifier,identifier) == 0) 
			return s;
	}

	//Check for the existence of a parent scope
	if (t->parent  ==  NULL) 
		return NULL;

	//Check the parent scope
	return getSymbol(t->parent, identifier);
}

//Used to check that a symbol hasn't already been declared
int defSymbol(SYMBOLTABLE *t, char *identifier)
{
	int i = Hash(identifier);

	for (SYMBOL *s = t->table[i]; s; s = s->next)
		if (strcmp(s->identifier, identifier) == 0)
			return 1;

	return 0;
}

void makePROGRAMstable (PROGRAM *p, int printFlag)
{
	g_table = initSymbolTable();

	if (p->decl != NULL)
		makeDECLARATIONstable(g_table, p->decl);
	
	if (p->stmt != NULL)
		makeSTATEMENTstable(g_table, p->stmt);

	DECLARATION *decl = p->decl;

	if (printFlag)
	{
		while (decl != NULL)
		{
			switch (decl->kind->type)
			{
				case tk_Int:
					printf("%s\t|\tint\n", decl->identifier);
					break;
				case tk_Float:
					printf("%s\t|\tfloat\n", decl->identifier);
					break;
				case tk_String:
					printf("%s\t|\tstring\n", decl->identifier);
					break;
				case tk_Boolean:
					printf("%s\t|\tboolean\n", decl->identifier);
					break;
			}
			decl = decl->next;
		}
	}
}

void makeDECLARATIONstable (SYMBOLTABLE *t, DECLARATION *d)
{
	//First, check that the identifier is not already in the symbol table.
	//
	//Secondly, for the right-hand side (the expression) check that any identifiers
	//	used in it are in the symbol table. 
	DECLARATION *currentDecl = d;

	while (currentDecl != NULL)
	{
		putSymbol(t, currentDecl->identifier, currentDecl->kind, currentDecl->lineno);
		makeEXPRESSIONstable(t, currentDecl->exp);
		currentDecl = currentDecl->next;
	}
}

void makeSTATEMENTstable (SYMBOLTABLE *t, STATEMENT *s)
{
	SYMBOLTABLE *new_st;
	SYMBOLTABLE *extra_st;		//this is used for the else block in an if-else stmt

	if (s != NULL)
	{
		switch (s->kind)
		{
			case sk_If:
				makeEXPRESSIONstable(t, s->val.ifS.condition);
				new_st = scopeSymbolTable(t);
				makeSTATEMENTstable(new_st, s->val.ifS.body);
				break;

			case sk_Ifelse:
				makeEXPRESSIONstable(t, s->val.ifelseS.condition);

				new_st = scopeSymbolTable(t);
				makeSTATEMENTstable(new_st, s->val.ifelseS.thenpart);

				extra_st = scopeSymbolTable(t);
				makeSTATEMENTstable(extra_st, s->val.ifelseS.elsepart);
				break;

			case sk_While:
				makeEXPRESSIONstable(t, s->val.whileS.condition);

				new_st = scopeSymbolTable(t);
				makeSTATEMENTstable(new_st, s->val.whileS.body);
				break;

			case sk_Read:
				//Check that the identifier is already within a symbol table
				if (!getSymbol(t, s->val.readS))
				{
					fprintf(stderr, "Error: Variable %s on line %d has not been declared.\n",
							s->val.readS, s->lineno);
					exit(1);
				}
				break;

			case sk_Print:
				makeEXPRESSIONstable(t, s->val.printS);
				break;

			case sk_Assign:
				//Check that the identifier is already within a symbol table
				if (!getSymbol(t, s->val.assignS.identifier))
				{
					fprintf(stderr, "Error: Variable %s on line %d has not been declared.\n",
							s->val.assignS.identifier, s->lineno);
					exit(1);
				}

				makeEXPRESSIONstable(t, s->val.assignS.right);
				break;

		}
	}
}

void makeEXPRESSIONstable (SYMBOLTABLE *t, EXP *e)
{
	if (e != NULL)
	{
		switch (e->kind)
		{
			case ek_Identifier:
				if (getSymbol(t, e->val.identifier) == NULL)
				{
					fprintf(stderr, "Error: Variable %s on line %d has not been declared.\n",
							e->val.identifier, e->lineno);
					exit(1);
				}
				break;
			case ek_Plus:
				makeEXPRESSIONstable(t, e->val.plusE.left);
				makeEXPRESSIONstable(t, e->val.plusE.right);
				break;
			case ek_Minus:
				makeEXPRESSIONstable(t, e->val.minusE.left);
				makeEXPRESSIONstable(t, e->val.minusE.right);
				break;
			case ek_Div:
				makeEXPRESSIONstable(t, e->val.divE.left);
				makeEXPRESSIONstable(t, e->val.divE.right);
				break;
			case ek_And:
				makeEXPRESSIONstable(t, e->val.andE.left);
				makeEXPRESSIONstable(t, e->val.andE.right);
				break;
			case ek_Or:
				makeEXPRESSIONstable(t, e->val.orE.left);
				makeEXPRESSIONstable(t, e->val.orE.right);
				break;
			case ek_Eq:
				makeEXPRESSIONstable(t, e->val.eqE.left);
				makeEXPRESSIONstable(t, e->val.eqE.right);
				break;
			case ek_Neq:
				makeEXPRESSIONstable(t, e->val.neqE.left);
				makeEXPRESSIONstable(t, e->val.neqE.right);
				break;
			case ek_Uminus:
				makeEXPRESSIONstable(t, e->val.uminusE);
				break;
			case ek_Bang:
				makeEXPRESSIONstable(t, e->val.bangE);
				break;
			//Default case should catch boolconstK, stringconstK, intconstK, floatconst
			default:
				break;
		}
	}
}

