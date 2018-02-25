#include <stdlib.h>
#include <string.h>
#include "tree.h"

extern int yylineno;

PROGRAM *makePROGRAM(DECLARATION *decl, STATEMENT *stmt)
{
	PROGRAM *p = malloc(sizeof(PROGRAM));
	p->decl = decl;
	p->stmt = stmt;
	return p;
}

DECLARATION *makeDECLARATION(char* identifier, TYPE *kind, EXP *exp)
{
	DECLARATION *d = malloc(sizeof(DECLARATION));
	d->lineno = yylineno;
	d->identifier = malloc((strlen(identifier)+1) * sizeof(char));
	strcpy(d->identifier, identifier);
	d->kind=kind;
	d->exp = exp;
	d->next = NULL;
	return d;
}

//STATEMENT STRUCTS
STATEMENT *makeSTATEMENTprint(EXP *exp)
{ 
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = sk_Print;
	s->val.printS = exp;
	s->next = NULL;
	return s;
}

STATEMENT *makeSTATEMENTread(char* identifier)
{ 
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = sk_Read;
	s->val.readS = malloc((strlen(identifier) + 1) * sizeof(char));
	strcpy(s->val.readS, identifier);
	s->next = NULL;
	return s;
}

STATEMENT *makeSTATEMENTif(EXP *condition, STATEMENT *body)
{ 
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = sk_If;
	s->val.ifS.condition = condition;
	s->val.ifS.body = body;
	s->next = NULL;
	return s;
}

STATEMENT *makeSTATEMENTifelse(EXP *condition, STATEMENT *thenpart, STATEMENT *elsepart)
{ 
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = sk_Ifelse;
	s->val.ifelseS.condition = condition;
	s->val.ifelseS.thenpart = thenpart;
	s->val.ifelseS.elsepart = elsepart;
	s->next = NULL;
	return s;
}

STATEMENT *makeSTATEMENTwhile(EXP *condition, STATEMENT *body)
{ 
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = sk_While;
	s->val.whileS.condition = condition;
	s->val.whileS.body = body;
	s->next = NULL;
	return s;
}

STATEMENT *makeSTATEMENTassign(char *left, EXP *right)
{ 
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = sk_Assign;
	s->val.assignS.identifier = malloc((strlen(left) + 1) * sizeof(char));
	strcpy(s->val.assignS.identifier, left);
	s->val.assignS.right = right;
	s->next = NULL;
	return s; 
}

//EXPRESSION STRUCTS - TYPES
EXP *makeEXPidentifier(char *identifier)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Identifier;
	e->val.identifier = malloc((strlen(identifier) + 1) * sizeof(char));
	strcpy(e->val.identifier, identifier);
	return e;
}

EXP *makeEXPstringconst(char* stringconstK)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_StringConst,
	e->val.stringconstK = malloc((strlen(stringconstK) + 1) * sizeof(char));
	strcpy(e->val.stringconstK, stringconstK);
	return e;
}

EXP *makeEXPintconst(int intconstK)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_IntConst;
	e->val.intconstK = intconstK;
	return e;
}

EXP *makeEXPboolconst(int boolconstK)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_BoolConst;
	e->val.boolconstK = boolconstK;
	return e;
}

EXP * makeEXPfloatconst(float floatconstK)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_FloatConst;
	e->val.floatconstK = floatconstK;
	return e;
}

//EXPRESSION STRUCTS - BINARY OPERATIONS
EXP *makeEXPplus(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Plus;
	e->val.plusE.left = left;
	e->val.plusE.right = right;
	return e;
}

EXP *makeEXPminus(EXP *left, EXP *right)
{ 
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Minus;
	e->val.minusE.left = left;
	e->val.minusE.right = right;
	return e;
}

EXP *makeEXPtimes(EXP *left, EXP *right)
{ 
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Times;
	e->val.timesE.left = left;
	e->val.timesE.right = right;
	return e;
}

EXP *makeEXPdiv(EXP *left, EXP *right)
{ 
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Div;
	e->val.divE.left = left;
	e->val.divE.right = right;
	return e;
}

EXP *makeEXPbang(EXP *bang)
{ 
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Bang;
	e->val.bangE = bang;
	return e;
}

EXP *makeEXPuminus(EXP *uminus)
{ 
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Uminus;
	e->val.uminusE = uminus;
	return e;
}

EXP *makeEXPand(EXP *left, EXP *right)
{ 
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_And;
	e->val.andE.left = left;
	e->val.andE.right = right;
	return e;
}

EXP *makeEXPor(EXP *left, EXP *right)
{ 
	EXP *e = malloc(sizeof(EXP));
  	e->lineno = yylineno;
	e->kind = ek_Or;
	e->val.orE.left = left;
	e->val.orE.right = right;
	return e;
}

EXP *makeEXPeq(EXP *left, EXP *right)
{ 
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Eq;
	e->val.eqE.left = left;
	e->val.eqE.right = right;
	return e;
}

EXP *makeEXPneq(EXP *left, EXP *right)
{ 
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = ek_Eq;
	e->val.eqE.left = left;
	e->val.eqE.right = right;
	return e;
}

TYPE *makeTYPEint()
{
	TYPE *t = malloc(sizeof(TYPE));
	t->lineno = yylineno;
	t->type = tk_Int;
	return t;
}

TYPE *makeTYPEfloat()
{
	TYPE *t = malloc(sizeof(TYPE));
	t->lineno = yylineno;
	t->type = tk_Float;
	return t;
}

TYPE *makeTYPEstring()
{
	TYPE *t = malloc(sizeof(TYPE));
	t->lineno = yylineno;
	t->type = tk_String;
	return t;
}

TYPE *makeTYPEbool()
{
	TYPE *t = malloc(sizeof(TYPE));
	t->lineno = yylineno;
	t->type = tk_Boolean;
	return t;
}

