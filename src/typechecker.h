#include <stdlib.h>
#include <stdio.h>
#include "symbol.h"
#include "tree.h"

extern SYMBOLTABLE *t;

void tcPROGRAM (PROGRAM *P);
void tcSTATEMENT (STATEMENT *s);
void tcEXPRESSION (EXP *e);
void tcASSIGNMENT (STATEMENT *s);
