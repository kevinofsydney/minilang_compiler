#ifndef TREE_H
#define TREE_H

typedef struct PROGRAM {
	struct DECLARATION *decl;
	struct STATEMENT *stmt;
} PROGRAM; 

typedef struct TYPE {
	int lineno;
	enum { tk_Int, tk_Float, tk_String, tk_Boolean } type;
} TYPE;

typedef struct DECLARATION {
	int lineno;
	char* identifier;
	struct TYPE *kind;
	struct EXP *exp;
	struct DECLARATION *next;
} DECLARATION;

typedef enum {
	ek_Identifier,
	ek_BoolConst,
	ek_StringConst,
        ek_IntConst,
        ek_FloatConst,
	ek_Plus,
        ek_Minus,
        ek_Times,
        ek_Div,
        ek_And,
        ek_Or,
        ek_Eq,
        ek_Neq,
	ek_Bang,
	ek_Uminus
} ExpressionKind;

typedef struct EXP {
        int lineno;
        ExpressionKind kind;
	TYPE type;
	union {
		int boolconstK;
                char *identifier;
		char *stringconstK;
                int intconstK;
                float floatconstK;
		struct EXP *uminusE;
		struct EXP *bangE;
		struct { struct EXP *left; struct EXP *right;} plusE;
		struct { struct EXP *left; struct EXP *right;} minusE;
		struct { struct EXP *left; struct EXP *right;} timesE;
		struct { struct EXP *left; struct EXP *right;} divE;
		struct { struct EXP *left; struct EXP *right;} andE;
		struct { struct EXP *left; struct EXP *right;} orE;
		struct { struct EXP *left; struct EXP *right;} eqE;
		struct { struct EXP *left; struct EXP *right;} neqE;
	 } val;
} EXP;

typedef enum StatementKind {
	sk_If,
	sk_Ifelse,
	sk_While,
	sk_Read, 
	sk_Print, 
	sk_Assign,
} StatementKind;

typedef struct STATEMENT {
	int lineno;
	StatementKind kind;
	union{
		struct {struct EXP *condition; 
			struct STATEMENT *body; 
			} ifS;
		struct {struct EXP *condition; 
			struct STATEMENT *thenpart;
			struct STATEMENT *elsepart; 
			} ifelseS;
		struct {struct EXP *condition; 
			struct STATEMENT *body;
			} whileS;
		char* readS; 
		struct EXP *printS; //use this when printing
		struct {char *identifier; 
			struct EXP *right;
			} assignS;
	} val;
	struct STATEMENT *next;
} STATEMENT;

PROGRAM *makePROGRAM(DECLARATION *decl, STATEMENT *stmt);

DECLARATION *makeDECLARATION(char* identifier, TYPE *kind, EXP *exp);

STATEMENT *makeSTATEMENTprint(EXP *exp);
STATEMENT *makeSTATEMENTread(char* identifier);
STATEMENT *makeSTATEMENTif(EXP *condition, STATEMENT *body);
STATEMENT *makeSTATEMENTifelse(EXP *condition, STATEMENT *thenpart, STATEMENT *elsepart);
STATEMENT *makeSTATEMENTwhile(EXP *condition, STATEMENT *body);
STATEMENT *makeSTATEMENTassign(char *left, EXP *right);

EXP *makeEXPidentifier(char *idK);
EXP *makeEXPstringconst(char* stringconstK);
EXP *makeEXPintconst(int intconstK);
EXP *makeEXPboolconst(int intconstK);
EXP *makeEXPfloatconst(float floatconstK);

EXP *makeEXPplus(EXP *left, EXP *right);
EXP *makeEXPminus(EXP *left, EXP *right);
EXP *makeEXPtimes(EXP *left, EXP *right);
EXP *makeEXPdiv(EXP *left, EXP *right);

EXP *makeEXPbang(EXP *bang);
EXP *makeEXPuminus(EXP *uminus);

EXP *makeEXPand(EXP *left, EXP *right);
EXP *makeEXPor(EXP *left, EXP *right);
EXP *makeEXPeq(EXP *left, EXP *right);
EXP *makeEXPneq(EXP *left, EXP *right);

TYPE *makeTYPEint();
TYPE *makeTYPEfloat();
TYPE *makeTYPEstring();
TYPE *makeTYPEbool();
#endif /* !TREE_H */
