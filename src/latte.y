%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int yylex();
extern int yylineno;
extern char* yytext;
extern FILE *yyin;
extern PROGRAM *g_program;
void yyerror(const char *s);
%}

%locations
%error-verbose

%union {
	int int_val;
	int bool_val;
	char *string_val;
	float float_val;
	struct EXP *exp;
	struct STATEMENT *stmt;
	struct DECLARATION *decl;
	struct TYPE *type;
}

%token ';'
%token tBOOLEAN tINT tFLOAT tSTRING
%token tVAR tIF tELSE tWHILE tPRINT tREAD

%token <int_val> tINTVAL
%token <string_val> tSTRINGVAL
%token <float_val> tFLOATVAL
%token <string_val> tIDENTIFIER
%token <bool_val> tTRUE tFALSE;

%type <exp> exp
%type <stmt> statement statement_list
%type <decl> declaration declaration_list
%type <type> type

%token ':' tASSIGN 
%token '(' ')' '{' '}'

%left tOR
%left tAND
%left tEQ tNEQ
%left '+' '-'
%left '*' '/'
%left tUMINUS '!'

%%

program		: declaration_list statement_list { g_program = makePROGRAM($1, $2); }
		;

declaration_list: /* empty */			{ $$ = NULL;	}
		| declaration_list declaration { $$ = $2; $$->next = $1; }
		;

declaration 	: tVAR tIDENTIFIER ':' type tASSIGN exp ';' { $$ = makeDECLARATION($2, $4, $6); }
		;	

statement_list	: /*empty*/			  { $$ = NULL; }
		| statement_list statement { $$ = $2; $$->next = $1; }
		;

statement	: tREAD tIDENTIFIER ';' 	{ $$ = makeSTATEMENTread($2); }
		| tPRINT exp ';'		{ $$ = makeSTATEMENTprint($2); }
		| tIDENTIFIER tASSIGN exp ';'	{ $$ = makeSTATEMENTassign($1, $3); }
		| tIF exp '{' statement_list '}' { $$ = makeSTATEMENTif($2, $4); }
		| tIF exp '{' statement_list '}' tELSE '{' statement_list '}' { $$ = makeSTATEMENTifelse($2, $4, $8); }
		| tWHILE exp '{' statement_list '}' { $$ = makeSTATEMENTwhile($2, $4); }
		;

/*Higher levels have lower precedence*/
/*Higher levels are lower precedence operations - they can contain higher 
	precedence operations.*/

exp		: exp '+' exp		{ $$ = makeEXPplus($1, $3); }
		| exp '-' exp 		{ $$ = makeEXPminus($1, $3); }
		| exp tAND exp		{ $$ = makeEXPand($1, $3); }
		| exp tOR exp		{ $$ = makeEXPor($1, $3); }
		| exp tEQ exp		{ $$ = makeEXPeq($1, $3); }
		| exp tNEQ exp		{ $$ = makeEXPneq($1, $3); }
		| '-' exp %prec tUMINUS	{ $$ = makeEXPuminus($2); }
		| exp '*' exp		{ $$ = makeEXPtimes($1, $3); }
		| exp '/' exp		{ $$ = makeEXPdiv($1, $3); }
		| tINTVAL		{ $$ = makeEXPintconst($1); }
		| tFLOATVAL		{ $$ = makeEXPfloatconst($1); }
		| tSTRINGVAL		{ $$ = makeEXPstringconst($1); }
		| tIDENTIFIER		{ $$ = makeEXPidentifier($1); }
		| tTRUE			{ $$ = makeEXPboolconst(1); }
		| tFALSE		{ $$ = makeEXPboolconst(0); }
		| '!' exp		{ $$ = makeEXPbang($2); }
		| '(' exp ')' 		{ $$ = $2; }
		;

type		: tINT			{ $$ = makeTYPEint(); }
		| tFLOAT		{ $$ = makeTYPEfloat(); }
		| tSTRING		{ $$ = makeTYPEstring(); }
		| tBOOLEAN		{ $$ = makeTYPEbool(); }
		;
%%

void yyerror(const char *s) 
{ 
	fprintf(stderr, "Error: (line %d) %s\n", yylineno, s); 
	exit(1); 
}
