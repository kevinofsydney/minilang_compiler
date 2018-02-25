#ifndef SYMBOL_H
#define SYMBOL_H

#include "tree.h"

#define HashSize 317

typedef struct SYMBOL {
	char *identifier;
	struct TYPE *type;
	struct EXP *exp;
	struct SYMBOL *next;
} SYMBOL; 

typedef struct SYMBOLTABLE {
	SYMBOL *table[HashSize];
	struct SYMBOLTABLE *parent;
} SYMBOLTABLE;

SYMBOLTABLE *initSYMBOLTABLE();
SYMBOLTABLE *scopeSYMBOLTABLE(SYMBOLTABLE *t);
SYMBOL *putSymbol(SYMBOLTABLE *t, char *identifier, TYPE *type, int lineno);
SYMBOL *getSymbol(SYMBOLTABLE *t, char *identifier);
int defSymbol(SYMBOLTABLE *t, char *identifier);
void makePROGRAMstable (PROGRAM *prog, int printFlag);
void makeDECLARATIONstable (SYMBOLTABLE *t, DECLARATION *d);
void makeSTATEMENTstable (SYMBOLTABLE *t, STATEMENT *s);
void makeEXPRESSIONstable (SYMBOLTABLE *t, EXP *e);

#endif /* !SYMBOL_H */
