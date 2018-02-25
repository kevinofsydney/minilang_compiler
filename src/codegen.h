#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdlib.h>
#include <stdio.h>
#include "symbol.h"
#include "tree.h"

extern SYMBOLTABLE *g_table;

void cgPROGRAM (PROGRAM *p, char *outputFileName);
void cgDECLARATION (DECLARATION *d, FILE *outputFile);
void cgSTATEMENT (STATEMENT *s, FILE *outputFile);
void cgEXP (EXP *e, FILE *outputFile);

#endif
