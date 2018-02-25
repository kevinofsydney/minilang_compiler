#include "codegen.h"

int indent;
void cgPROGRAM (PROGRAM *p, char *outputFileName){
        indent = 1;

        if (p == NULL)
                return;

        FILE *outputFile = fopen(outputFileName, "w+");

        /* Write include statements to the output file*/
	fprintf(outputFile, "#include <stdio.h>\n");
	fprintf(outputFile, "#include <stdlib.h>\n");
	fprintf(outputFile, "#include <string.h>\n");
	fprintf(outputFile, "#include <stdbool.h>\n");

        /* Write main function */
        fprintf(outputFile, "int main(){\n");

        if (p->decl != NULL){
                cgDECLARATION(p->decl, outputFile);
        }

        if (p->stmt != NULL){
                cgSTATEMENT(p->stmt, outputFile);
        }

        /* Return 0 and close main() */
	fprintf(outputFile,"\treturn 0;\n");
	fprintf(outputFile,"}\n");

        fclose(outputFile);
}

void cgDECLARATION(DECLARATION *d, FILE *outputFile)
{
        for(int i=0; i<indent; i++) fprintf(outputFile, "\t");

	switch (d->kind->type)
	{
        case tk_Int:
                fprintf(outputFile, "int %s = ", d->identifier);
                cgEXP(d->exp, outputFile);
                fprintf(outputFile, ";\n");
                break;
        case tk_Float:
                fprintf(outputFile, "float %s = ", d->identifier);
                cgEXP(d->exp, outputFile);
                fprintf(outputFile, ";\n");
                break;
        case tk_String:
                fprintf(outputFile, "char* %s = ", d->identifier);
                cgEXP(d->exp, outputFile);
                fprintf(outputFile, ";\n");
                break;
        case tk_Boolean:
                fprintf(outputFile, "bool %s = ", d->identifier);
                cgEXP(d->exp, outputFile);
                fprintf(outputFile, ";\n");
                break;
	}

	if (d->next != NULL)
		cgDECLARATION(d->next, outputFile);
}

void cgSTATEMENT(STATEMENT *s, FILE *outputFile){
        for(int i=0; i<indent; i++) fprintf(outputFile, "\t");

	switch (s->kind)
	{
        case sk_If:
                fprintf(outputFile, "if(");
                cgEXP(s->val.ifS.condition, outputFile);
                fprintf(outputFile, ") {\n");

                indent++;
                cgSTATEMENT(s->val.ifS.body, outputFile);
                indent--;

                for(int i=0; i<indent; i++) 
			fprintf(outputFile, "\t");
                
		fprintf(outputFile, "}\n");
                break;
        case sk_Ifelse:
                fprintf(outputFile, "if(");
                cgEXP(s->val.ifelseS.condition, outputFile);
                fprintf(outputFile, ") {\n");

                indent++;
                cgSTATEMENT(s->val.ifelseS.thenpart, outputFile);
                indent--;

                fprintf(outputFile, "\n");
                for(int i=0; i<indent; i++) 
			fprintf(outputFile, "\t");
                fprintf(outputFile, "}\n");
                for(int i=0; i<indent; i++) 
			fprintf(outputFile, "\t");
                fprintf(outputFile, "else {\n");

                indent++;
                cgSTATEMENT(s->val.ifelseS.elsepart, outputFile);
                indent--;

                fprintf(outputFile, "\n");
                for(int i=0; i<indent; i++) 
			fprintf(outputFile, "\t");
                fprintf(outputFile, "}\n");
                break;
        case sk_While:
                fprintf(outputFile, "while(");
                cgEXP(s->val.whileS.condition, outputFile);
                fprintf(outputFile, ") {\n");

                indent++;
                cgSTATEMENT(s->val.whileS.body, outputFile);
                indent--;

                fprintf(outputFile, "\n");
                for(int i=0; i<indent; i++) 
			fprintf(outputFile, "\t");
                fprintf(outputFile, "}\n");
                break; 
        case sk_Read:
                switch(getSymbol(g_table, s->val.readS)->type->type)
		{
                	case tk_Int:
                        	fprintf(outputFile, "scanf(\"%%d\", &%s);\n", s->val.readS);
                        	break;
	                case tk_Float:
	                        fprintf(outputFile, "scanf(\"%%f\", &%s);\n", s->val.readS);
	                        break;
        	        case tk_String:
	                        fprintf(outputFile, "scanf(\"%%s\", &%s);\n", s->val.readS);
       		                 break;
	                case tk_Boolean:
	                        fprintf(outputFile, "scanf(\"%%d\", &%s);\n", s->val.readS);
	                        break;
                }
                /* fprintf(outputFile, "read %s;\n", s->val.readId); */
                break;

        case sk_Print:
                switch(s->val.printS->type.type)
		{
                	case tk_Int:
                       		fprintf(outputFile, "printf(\"%%d\", ");
                       		cgEXP(s->val.printS, outputFile);
	                        fprintf(outputFile, ");\n");
        	                break;
	                case tk_Float:
       	                	fprintf(outputFile, "printf(\"%%f\", ");
                        	cgEXP(s->val.printS, outputFile);
                        	fprintf(outputFile, ");\n");
                        	break;
	                case tk_String:
	                        fprintf(outputFile, "printf(\"%%s\", ");
	                        cgEXP(s->val.printS, outputFile);
                 	       fprintf(outputFile, ");\n");
	                        break;
	                case tk_Boolean:
	                        fprintf(outputFile, "printf(\"%%d\", ");
	                        cgEXP(s->val.printS, outputFile);
	                        fprintf(outputFile, ");\n");
	                        break;
                }

                break;

        case sk_Assign:
                fprintf(outputFile, "%s = ", s->val.assignS.identifier);
                cgEXP(s->val.assignS.right, outputFile);
                fprintf(outputFile, ";\n");
                break;
	}

	if (s->next!= NULL)
	{
		cgSTATEMENT(s->next, outputFile);
	}
}


void cgEXP(EXP *e, FILE *outputFile){
	switch (e->kind) {
	        case ek_Identifier:
       		        fprintf(outputFile, "%s", e->val.identifier);
               		break;

	        case ek_StringConst:
	                fprintf(outputFile, "%s", e->val.stringconstK);
	                break;
	
	        case ek_IntConst:
	                fprintf(outputFile, "%i", e->val.intconstK);
               		break;

		case ek_FloatConst:
			fprintf(outputFile, "%f", e->val.floatconstK);
			break;

		case ek_BoolConst:
			e->val.boolconstK ? fprintf(outputFile, "true") : fprintf(outputFile, "false");
			break;

		case ek_Plus:
			fprintf(outputFile, "(");
			cgEXP(e->val.plusE.left, outputFile);
			fprintf(outputFile, " + ");
			cgEXP(e->val.plusE.right, outputFile);
			fprintf(outputFile, ")");
			break;

		case ek_Minus:
			fprintf(outputFile, "(");
			cgEXP(e->val.minusE.left, outputFile);
			fprintf(outputFile, " - ");
			cgEXP(e->val.minusE.right, outputFile);
			fprintf(outputFile, ")");
			break;

		case ek_Times:
			fprintf(outputFile, "(");
			cgEXP(e->val.timesE.left, outputFile);
			fprintf(outputFile, " * ");
			cgEXP(e->val.timesE.right, outputFile);
			fprintf(outputFile, ")");
			break;

		case ek_Div:
			fprintf(outputFile, "(");
			cgEXP(e->val.divE.left, outputFile);
			fprintf(outputFile, " / ");
			cgEXP(e->val.divE.right, outputFile);
			fprintf(outputFile, ")");
			break;

		case ek_Eq:
			fprintf(outputFile, "(");
			cgEXP(e->val.eqE.left, outputFile);
			fprintf(outputFile, " == ");
			cgEXP(e->val.eqE.right, outputFile);
			fprintf(outputFile, ")");
			break;

		case ek_Neq:
			fprintf(outputFile, "(");
			cgEXP(e->val.neqE.left, outputFile);
			fprintf(outputFile, " != ");
			cgEXP(e->val.neqE.right, outputFile);
			fprintf(outputFile, ")");
			break;

		case ek_And:
			fprintf(outputFile, "(");
			cgEXP(e->val.andE.left, outputFile);
			fprintf(outputFile, " && ");
			cgEXP(e->val.andE.right, outputFile);
			fprintf(outputFile, ")");
			break;

		case ek_Or:
			fprintf(outputFile, "(");
			cgEXP(e->val.orE.left, outputFile);
			fprintf(outputFile, " || ");
			cgEXP(e->val.orE.right, outputFile);
			fprintf(outputFile, ")");
			break;

		case ek_Uminus:
			fprintf(outputFile, "-");
			cgEXP(e->val.uminusE, outputFile);
			break;

		case ek_Bang:
			fprintf(outputFile, "!");
			cgEXP(e->val.bangE, outputFile);
			break;
	}
}
