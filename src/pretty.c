#include <stdio.h>
#include "pretty.h"

int indent;

void prettyPROGRAM(PROGRAM *p)
{
	indent = 0;
	if (p->decl != NULL)
		prettyDECLARATION(p->decl);
	
	if (p->stmt != NULL) 
	{	
		printf("\n");	
		prettySTATEMENT(p->stmt);
	}
}

void prettyDECLARATION(DECLARATION *d)
{
	switch (d->kind->type)
	{
		case tk_Int:
			printf("var %s : int = ", d->identifier);
			prettyEXP(d->exp);
			printf(";\n");
			break;
		case tk_Float:
			printf("var %s : float = ", d->identifier);
			prettyEXP(d->exp);
			printf(";\n");
			break;
		case tk_String:
			printf("var %s : string = ", d->identifier);
			prettyEXP(d->exp);
			printf(";\n");
			break;
		case tk_Boolean:
			printf("var %s : boolean = ", d->identifier);
			prettyEXP(d->exp);
			printf(";\n");
			break;
	}

	if (d->next != NULL)
	{
		prettyDECLARATION(d->next);
	}
}

void prettySTATEMENT(STATEMENT *s)
{
	int i;
	for (i = 0; i < indent; i++)
		printf("\t");

	switch (s->kind)
	{
		case sk_If:
			printf("if ");
			prettyEXP(s->val.ifS.condition);
			printf(" {\n");

			indent = indent + 1;
			prettySTATEMENT(s->val.ifS.body);
			indent = indent - 1;

			printf("}\n\n");
			break;	
		case sk_Ifelse:
			printf("if ");
			prettyEXP(s->val.ifelseS.condition);
			printf(" {\n");
			
			indent = indent + 1;
			prettySTATEMENT(s->val.ifelseS.thenpart);
			indent = indent - 1;

			for (i = 0; i < indent; i++)
				printf("\t");

			printf("} else {\n");

			indent = indent + 1;
			prettySTATEMENT(s->val.ifelseS.elsepart);
			indent = indent - 1;

			for (i = 0; i < indent; i++)
				printf("\t");

			printf("}\n");
			break;	
		case sk_While:
			printf("while ");
			prettyEXP(s->val.whileS.condition);
			printf(" {\n");

			indent = indent + 1;
			prettySTATEMENT(s->val.whileS.body);
			indent = indent - 1;
			
			printf("}\n\n");
			break;	
		case sk_Read:
			printf("read %s;\n", s->val.readS);
			break;	
		case sk_Print:
			printf("print ");
			prettyEXP(s->val.printS);
			printf(";\n");
			break;	
		case sk_Assign:
			printf("%s = ", s->val.assignS.identifier);
			prettyEXP(s->val.assignS.right);
			printf(";\n");
			break;	
	}

	if (s->next != NULL)
	{
		prettySTATEMENT(s->next);
	}
}

void prettyEXP(EXP *e)
{
	switch (e->kind) 
	{
		case ek_Identifier:
			printf("%s", e->val.identifier);
			break;
		case ek_IntConst:
			printf("%i", e->val.intconstK);
			break;
		case ek_BoolConst:
			if (e->val.boolconstK)
			{ printf("TRUE"); }
			else
			{ printf("FALSE"); }
			break;	
		case ek_StringConst:
			printf("%s", e->val.stringconstK);
			break;	
		case ek_FloatConst:
			printf("%f", e->val.floatconstK);
			break;	
		case ek_Plus:
			printf("(");
			prettyEXP(e->val.plusE.left);
			printf("+");
			prettyEXP(e->val.plusE.right);
			printf(")");
			break;
		case ek_Minus:
			printf("(");
			prettyEXP(e->val.minusE.left);
			printf("-");
			prettyEXP(e->val.minusE.right);
			printf(")");
			break;
		case ek_Times:
			printf("(");
			prettyEXP(e->val.timesE.left);
			printf("*");
			prettyEXP(e->val.timesE.right);
			printf(")");
			break;
		case ek_Div:
			printf("(");
			prettyEXP(e->val.divE.left);
			printf("/");
			prettyEXP(e->val.divE.right);
			printf(")");
			break;
		case ek_And:
			printf("(");
			prettyEXP(e->val.andE.left);
			printf("&&");
			prettyEXP(e->val.andE.right);
			printf(")");
			break;
		case ek_Or:
			printf("(");
			prettyEXP(e->val.orE.left);
			printf("||");
			prettyEXP(e->val.orE.right);
			printf(")");
			break;
		case ek_Eq:
			printf("(");
			prettyEXP(e->val.eqE.left);
			printf("==");
			prettyEXP(e->val.eqE.right);
			printf(")");
			break;
		case ek_Neq:
			printf("(");
			prettyEXP(e->val.neqE.left);
			printf("!=");
			prettyEXP(e->val.neqE.right);
			printf(")");
			break;
		case ek_Bang:
			//TODO
			break;	
		case ek_Uminus:
			//TODO
			break;	

	}
}

