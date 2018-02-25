%{

#include <stdio.h>
#include "latte.tab.h"

void tprinter(char *token);

extern int g_tokens;
%} 

D [0-9]

%option yylineno
%option noinput
%option nounput

%%

";"			{ tprinter("tSEMICOLON"); return ';'; }

"\/\/".*		;
[ \t\n;]		;
TRUE			{ tprinter("tTRUE"); return tTRUE; }
FALSE			{ tprinter("tFALSE"); return tFALSE; }

boolean			{ tprinter("tBOOLEAN"); return tBOOLEAN; }
int 			{ tprinter("tINT"); return tINT; }
float 			{ tprinter("tFLOAT"); return tFLOAT; }
string 			{ tprinter("tSTRING"); return tSTRING; }

var			{ tprinter("tVAR"); return tVAR; }
if 			{ tprinter("tIF"); return tIF; }
while			{ tprinter("tWHILE"); return tWHILE; }
else			{ tprinter("tELSE"); return tELSE; }
print			{ tprinter("tPRINT"); return tPRINT; }
read			{ tprinter("tREAD"); return tREAD; }

[a-zA-Z_][a-zA-Z0-9_]*	{ 
				yylval.string_val = strdup(yytext); 
				if (g_tokens)	
				{
					printf("tIDENTIFIER(%s)\n", yytext);
				}
				return tIDENTIFIER; 
			}

0+{D}+			{	
				fprintf(stderr, "Error: (line %d) illegal integer %s\n", yylineno, yytext);
				exit(1);
			}
"."{D}+|{D}+"."|0+{D}+"."{D}+	{
				fprintf(stderr, "Error: (line %d) illegal float %s\n", yylineno, yytext);
				exit(1);
				}
0|([1-9]{D}*)		{ 
				yylval.int_val = atoi(yytext); 
				if (g_tokens)	
				{
					printf("tINTVAL(%s)\n", yytext);
				}
				return tINTVAL; 
			}


{D}+"."{D}*		{ 
				yylval.float_val = atof(yytext); 
				if (g_tokens)	
				{
					printf("tFLOATVAL(%s)\n", yytext);
				}
				return tFLOATVAL; 
			}
\"[^\\n]*\\[^abfntrv\\\"].*\" {
				fprintf(stderr, "Error: (line %d) illegal escape sequence %s\n", yylineno, yytext);
				exit(1);
			}
\"(\\.|[^\\"])*\"	{ 
				yylval.string_val = strdup(yytext); 
				if (g_tokens)	
				{
					printf("tSTRINGVAL(%s)\n", yytext);
				}	
				return tSTRINGVAL; 
			}

":"			{ tprinter("tCOLON"); return ':'; }
"="			{ tprinter("tASSIGN"); return tASSIGN; }
"=="			{ tprinter("tEQ"); return tEQ; }
"!=" 			{ tprinter("tNEQ"); return tNEQ; }
"&&"			{ tprinter("tAND"); return tAND; }
"||"			{ tprinter("tOR"); return tOR; }
"+"			{ tprinter("tPLUS"); return '+'; }
"-"			{ tprinter("tMINUS"); return '-'; }
"*"			{ tprinter("tMULT"); return '*'; }
"/"			{ tprinter("tDIV"); return '/'; }
"("			{ tprinter("tLPAREN"); return '('; }
")"			{ tprinter("tRPAREN"); return ')'; }
"{"			{ tprinter("tLBRACE"); return '{'; }
"}"			{ tprinter("tRBRACE"); return '}'; }
"!"			{ tprinter("tBANG"); return '!'; }

.			{ fprintf(stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); }


%%

int yywrap(void) { return 1; }

void tprinter(char *token)
{
	if (g_tokens) { printf("%s\n", token); }
}

