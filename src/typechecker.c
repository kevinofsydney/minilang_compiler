#include "typechecker.h"

extern SYMBOLTABLE *g_table;

void tcPROGRAM (PROGRAM *p)
{
	if (p->stmt != NULL)
		tcSTATEMENT(p->stmt);
}

void tcSTATEMENT (STATEMENT *s)
{
	while (s != NULL)
	{
		switch (s->kind)
		{
			//The condition of an if statement must be an integer or a boolean
			case sk_If:
				tcEXPRESSION(s->val.ifS.condition);
			
				if (s->val.ifS.condition->type.type != tk_Int &&
					s->val.ifS.condition->type.type != tk_Boolean)
				{
					fprintf(stderr, "Error: Type check exception at line %d: the condition of an if statement must be of type int or boolean.\n", s->lineno);
					exit(1);
				}

				tcSTATEMENT(s->val.ifS.body);
				break;
				
			//The condition of an ifelse statement must be an integer or a boolean
			case sk_Ifelse:
				tcEXPRESSION(s->val.ifelseS.condition);

				if (s->val.ifelseS.condition->type.type != tk_Int &&
					s->val.ifelseS.condition->type.type != tk_Boolean)
				{
					fprintf(stderr, "Error: Type check exception at line %d: the condition of an if-else statement must be of type int or boolean.\n", s->lineno);
					exit(1);
				}

				tcSTATEMENT(s->val.ifelseS.thenpart);
				tcSTATEMENT(s->val.ifelseS.elsepart);
				break;
			//The condition of an if statement must be an integer or a boolean
			case sk_While:
				tcEXPRESSION(s->val.whileS.condition);
				
				if (s->val.whileS.condition->type.type != tk_Int &&
					s->val.whileS.condition->type.type != tk_Boolean)
				{
					fprintf(stderr, "Error: Type check exception at line %d: the condition of a while statement must be of type int or boolean.\n", s->lineno);
					exit(1);
				}

				tcSTATEMENT(s->val.whileS.body);
				break;
				
			//Because the type of a read symbol is decided at run time,
			//	you can't type check a read statement. You can only
			//	check that the symbol has been declared, whcih is what
			//	happens already in symbol.c .
			case sk_Read:
				break;

			case sk_Print:
				tcEXPRESSION(s->val.printS);
				break;

			case sk_Assign:
				tcASSIGNMENT(s); 
				break;
		}
		s = s->next;
	}
}

//The goal of tcEXPRESSION is to correctly assign a type to each expression.
void tcEXPRESSION (EXP *e)
{
	SYMBOL *y;
	if (e != NULL)
	{
		switch (e->kind)
		{
			case ek_Identifier:
				y = getSymbol(g_table, e->val.identifier);
				
				if (y == NULL)
				{
					fprintf(stderr, "Error: The symbol '%s' is uncdeclared.\n", e->val.identifier);
					exit(1);
				}

				switch (y->type->type)
				{
					case tk_Int:
						e->type.type = tk_Int;
						break;
					case tk_Float:
						e->type.type = tk_Float;
						break;
					case tk_String:
						e->type.type = tk_String;
						break;
					case tk_Boolean:
						e->type.type = tk_Boolean;
						break;
				}
				break;

			case ek_BoolConst:
				e->type.type = tk_Boolean;
				break;

			case ek_StringConst:
				e->type.type = tk_String;
				break;

			case ek_IntConst:
				e->type.type = tk_Int;
				break;

			case ek_FloatConst:
				e->type.type = tk_Float;
				break;

			//Where <op> is + - * /
			//int <op> int = int
			//int <op> float = float
			
			//float <op> float = float
			//float <op> int = float
			
			//string + string = string
			//string * int = string (plus checks to ensure int gt 0)
			case ek_Plus:
				tcEXPRESSION(e->val.plusE.left);
				tcEXPRESSION(e->val.plusE.right);

				//Left side is an int
				if (e->val.plusE.left->type.type == tk_Int)
				{
					if (e->val.plusE.right->type.type == tk_Int)
						e->type.type = tk_Int;
					else if (e->val.plusE.right->type.type == tk_Float)
						e->type.type = tk_Float;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to add an int to something other than an int or a float.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a float
				else if (e->val.plusE.left->type.type == tk_Float)
				{
					if (e->val.plusE.right->type.type == tk_Int)
						e->type.type = tk_Float;
					else if (e->val.plusE.right->type.type == tk_Float)
						e->type.type = tk_Float;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to add to float something other than an int or a float.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a string
				else if (e->val.plusE.left->type.type == tk_String)
				{
					if (e->val.plusE.right->type.type == tk_String)
						e->type.type = tk_String;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to add to a string something other than another string.\n", e->lineno);
						exit(1);
					}
				}
				else
					fprintf(stderr, "Error: (line %d) PLUS (+) operator can only be used with int, float or string types.\n", e->lineno);
				break;

			case ek_Minus:
				tcEXPRESSION(e->val.minusE.left);
				tcEXPRESSION(e->val.minusE.right);

				//Left side is an int
				if (e->val.minusE.left->type.type == tk_Int)
				{
					if (e->val.minusE.right->type.type == tk_Int)
						e->type.type = tk_Int;
					else if (e->val.minusE.right->type.type == tk_Float)
						e->type.type = tk_Float;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to subtract from an int something other than an int or a float.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a float
				else if (e->val.minusE.left->type.type == tk_Float)
				{
					if (e->val.minusE.right->type.type == tk_Int)
						e->type.type = tk_Float;
					else if (e->val.minusE.right->type.type == tk_Float)
						e->type.type = tk_Float;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to subtract from a float something other than an int or a float.\n", e->lineno);
						exit(1);
					}
				}
				else
					fprintf(stderr, "Error: (line %d) MINUS (-) operator can only be used with int or float types.\n", e->lineno);
				break;

			case ek_Times:
				tcEXPRESSION(e->val.timesE.left);
				tcEXPRESSION(e->val.timesE.right);

				//Left side is an int
				if (e->val.timesE.left->type.type == tk_Int)
				{
					if (e->val.timesE.right->type.type == tk_Int)
						e->type.type = tk_Int;
					else if (e->val.timesE.right->type.type == tk_Float)
						e->type.type = tk_Float;
					else if (e->val.timesE.right->type.type == tk_String)
					{
						e->type.type = tk_String;
						//BEWARE!! Commented out this for now because its untested.
						/*if (e->val.timesE.left->val.intconstK < 0)
						{
							fprintf(stderr, "Cannot multiply a string by an integer less than 0.\n");
							exit(1);
						}*/
					}
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to multiply an int to something other than an int, float or string.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a float
				else if (e->val.timesE.left->type.type == tk_Float)
				{
					if (e->val.timesE.right->type.type == tk_Int)
						e->type.type = tk_Float;
					else if (e->val.timesE.right->type.type == tk_Float)
						e->type.type = tk_Float;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to multiply a float with something other than an int or a float.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a string
				else if (e->val.timesE.left->type.type == tk_String)
				{
					if (e->val.timesE.right->type.type == tk_Int)
					{
						e->type.type = tk_String;
						//BEWARE!! Commented out this for now because its untested.
						/*if (e->val.timesE.right->val.intconstK < 0)
						{
							fprintf(stderr, "Cannot multiply a string by an integer less than 0.\n");
							exit(1);
						}*/
					
					}
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to multiply a string with something other than an integer.\n", e->lineno);
						exit(1);
					}
				}
				else
					fprintf(stderr, "Error: (line %d) MULT (*) operator can only be used with int, float or string types.\n", e->lineno);
				break;
			
			case ek_Div:
				tcEXPRESSION(e->val.divE.left);
				tcEXPRESSION(e->val.divE.right);

				//Left side is an int
				if (e->val.divE.left->type.type == tk_Int)
				{
					if (e->val.divE.right->type.type == tk_Int)
						e->type.type = tk_Int;
					else if (e->val.divE.right->type.type == tk_Float)
						e->type.type = tk_Float;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to divide an int by something other than an int or a float.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a float
				else if (e->val.divE.left->type.type == tk_Float)
				{
					if (e->val.divE.right->type.type == tk_Int)
						e->type.type = tk_Float;
					else if (e->val.divE.right->type.type == tk_Float)
						e->type.type = tk_Float;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to divide a float by something other than an int or a float.\n", e->lineno);
						exit(1);
					}
				}
				else
					fprintf(stderr, "Error: (line %d) DIV (/) operator can only be used with int or float types.\n", e->lineno);

				break;

			case ek_And:
				tcEXPRESSION(e->val.andE.left);
				tcEXPRESSION(e->val.andE.right);

				//Left side is an int
				if (e->val.andE.left->type.type == tk_Int)
				{
					if (e->val.andE.right->type.type == tk_Int)
						e->type.type = tk_Boolean;
					else if (e->val.andE.right->type.type == tk_Boolean)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to AND (&&) an int with something other than an int or a boolean.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a boolean
				else if (e->val.andE.left->type.type == tk_Boolean)
				{
					if (e->val.andE.right->type.type == tk_Int)
						e->type.type = tk_Boolean;
					else if (e->val.andE.right->type.type == tk_Boolean)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to AND (&&) a boolean with something other than an int or a boolean.\n", e->lineno);
						exit(1);
					}
				}
				else	
					fprintf(stderr, "Error: (line %d) AND (&&) operator can only be used with int or boolean types.\n", e->lineno);
			
				break;

			case ek_Or:
				tcEXPRESSION(e->val.orE.left);
				tcEXPRESSION(e->val.orE.right);

				//Left side is an int
				if (e->val.orE.left->type.type == tk_Int)
				{
					if (e->val.orE.right->type.type == tk_Int)
						e->type.type = tk_Boolean;
					else if (e->val.orE.right->type.type == tk_Boolean)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to OR (||) an int with something other than an int or a boolean.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a boolean
				else if (e->val.orE.left->type.type == tk_Boolean)
				{
					if (e->val.orE.right->type.type == tk_Int)
						e->type.type = tk_Boolean;
					else if (e->val.orE.right->type.type == tk_Boolean)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to OR (||) a boolean with something other than an int or a boolean.\n", e->lineno);
						exit(1);
					}
				}
				else
					fprintf(stderr, "Error: (line %d) OR (||) operator can only be used with int or boolean types.\n", e->lineno);
				break;

			case ek_Eq:
				tcEXPRESSION(e->val.eqE.left);
				tcEXPRESSION(e->val.eqE.right);

				//Left side is an int
				if (e->val.eqE.left->type.type == tk_Int)
				{
					if (e->val.eqE.right->type.type == tk_Int)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to EQUALS (==) an int with something other than another int.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a float
				else if (e->val.eqE.left->type.type == tk_Float)
				{
					if (e->val.eqE.right->type.type == tk_Float)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to EQUALS (==) a float with something other than another float.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a string
				else if (e->val.eqE.left->type.type == tk_String)
				{
					if (e->val.eqE.right->type.type == tk_String)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to EQUALS (==) a string with something other than another string.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a boolean
				else if (e->val.eqE.left->type.type == tk_Boolean)
				{
					if (e->val.eqE.right->type.type == tk_Boolean)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to EQUALS (==) a boolean with something other than another boolean.\n", e->lineno);
						exit(1);
					}
				}
			
				break;

			case ek_Neq:
				tcEXPRESSION(e->val.neqE.left);
				tcEXPRESSION(e->val.neqE.right);

				//Left side is an int
				if (e->val.neqE.left->type.type == tk_Int)
				{
					if (e->val.neqE.right->type.type == tk_Int)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to NOTEQUALS (!=) an int with something other than another int.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a float
				else if (e->val.neqE.left->type.type == tk_Float)
				{
					if (e->val.neqE.right->type.type == tk_Float)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to NOTEQUALS (!=) a float with something other than another float.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a string
				else if (e->val.neqE.left->type.type == tk_String)
				{
					if (e->val.neqE.right->type.type == tk_String)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to NOTEQUALS (!=) a string with something other than another string.\n", e->lineno);
						exit(1);
					}
				}
				//Left side is a boolean
				else if (e->val.neqE.left->type.type == tk_Boolean)
				{
					if (e->val.neqE.right->type.type == tk_Boolean)
						e->type.type = tk_Boolean;
					else
					{
						fprintf(stderr, "Error: (line %d) Attempting to NOTEQUALS (!=) a boolean with something other than another boolean.\n", e->lineno);
						exit(1);
					}
				}
			
				break;

			case ek_Bang:
				if (e->val.bangE->type.type == tk_Boolean)
					e->type.type = tk_Boolean;
				else
				{
						fprintf(stderr, "Error: (line %d) UNARY NOT operator (!) can only be used with boolean types.\n", e->lineno);
						exit(1);
				}
				break;

			case ek_Uminus:
				//Left side is an int or a float
				if (e->val.uminusE->type.type == tk_Int || 
						e->val.uminusE->type.type == tk_Float)
				{
					tcEXPRESSION(e->val.uminusE);
					e->type = e->val.uminusE->type;
				}
				else
				{
					fprintf(stderr, "Error: (line %d) UNARY MINUS (-) can only be used with an int or a float.\n", e->lineno);
					exit(1);
				}

				break;
		}
	}
}

void tcASSIGNMENT (STATEMENT *s)
{
	SYMBOL *y;

	if (s != NULL)
	{
		y = getSymbol(g_table, s->val.assignS.identifier);
		
		if (y == NULL)
		{
			fprintf(stderr, "Error: The symbol '%s' is uncdeclared.\n", s->val.assignS.identifier);
			exit(1);
		}

		tcEXPRESSION(s->val.assignS.right);
		TYPE *kev_type = getSymbol(g_table, s->val.assignS.identifier)->type;

		switch(kev_type->type)
		{
			case tk_Int:
				if (s->val.assignS.right->type.type != tk_Int)
				{
					fprintf(stderr, "Error: (line %d) Incorrect assignment to an int type.\n", s->lineno);
					exit(1);
				}
				break;

			case tk_Float:
				if (s->val.assignS.right->type.type != tk_Float)
				{
					fprintf(stderr, "Error: (line %d) Incorrect assignment to an float type.\n", s->lineno);
					exit(1);
				}
				break;

			case tk_String:
				if (s->val.assignS.right->type.type != tk_String)
				{
					fprintf(stderr, "Error: (line %d) Incorrect assignment to an string type.\n", s->lineno);
					exit(1);
				}
				break;

			case tk_Boolean:
				if (s->val.assignS.right->type.type != tk_Boolean)
				{
					fprintf(stderr, "Error: (line %d) Incorrect assignment to an boolean type.\n", s->lineno);
					exit(1);
				}
				break;
		}
	}
}
