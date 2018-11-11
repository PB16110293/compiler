#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include "../lex/pl0_lex.h"
#include "pl0_tax.h"
int table_index = 0;
int stark_index = 0;
/*operations for token table*/
void table_append(PL0Lex * lex, int kind) {
	strcpy(token_table[table_index].name, lex->last_id);
	token_table[table_index].kind = kind;
	switch (kind) {
		case ID_CONSTANT:
			if (lex->last_num > MAXADDRESS) {
				printf("the number is too great!\n");
				lex->last_num = 0;
			}
			token_table[table_index].value = lex->last_num;
			break;
		case ID_VARIABLE:
			token_table[table_index].level = lex->last_level;
			break;
		case ID_PROCEDURE:
			token_table[table_index].level = lex->last_level;
			break;
	} //switch
	table_index += 1;
	//return 0;
} //table_append
void stark_push(char * ch, int num) {
	int length = strlen(ch);
	strcpy(token_stark[stark_index].name, ch);
	token_stark[stark_index].name[length] = '\0';
	token_stark[stark_index].value = num;
	stark_index += 1;
}
void stark_pop() {
	stark_index -= 1;
}
void stark_print(PL0Lex * lex) {
	int i;
	for(i = 0; i <= stark_index-1; i++) {
		//if(token_stark[stark_index].value == -1) {
			printf("%s ",token_stark[i].name);
		//}
	//	else {
		//	printf("%d ",token_stark[stark_index].value);
	//	}
	}
	printf("      input token: %d $",lex->last_token_type);
	printf("\n");
}
/*functions for syntax analysis*/
//void statement(PL0Lex * lex) {
////	printf("analysis the statement\n");
//}
int isR(PL0Lex * lex) {
	if(lex->last_token_type >= 7 && lex->last_token_type <= 13) {
		return 1;
	}
	else
		return 0;
}
void condition(PL0Lex * lex) {
	printf("analysis the condition expression\n");
	if (lex->last_token_type == TOKEN_ODD) {
		stark_pop();
		stark_push("EXPR",-1);
		stark_push("ODD",-1);
		stark_print(lex);
		stark_pop();
		PL0Lex_get_token(lex);
		stark_print(lex);
		expression(lex);
	}
	else if(lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_NUMBER || lex->last_token_type == TOKEN_MINUS || lex->last_token_type == TOKEN_LPAREN) {
		stark_pop();
		stark_push("EXPR",-1);
		stark_push("R",-1);
		stark_push("EXPR",-1);
		stark_print(lex);
		expression(lex);
		if(lex->last_token_type >= TOKEN_EQU && lex->last_token_type <= TOKEN_GEQ) {
			stark_pop();
			PL0Lex_get_token(lex);
			stark_print(lex);
			expression(lex);
		}
		else {
			printf("there must be a symbol between two expreesion \n");
		}
	}
}

void expression(PL0Lex * lex) {
	printf("analysis the expression\n");
	if(lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_NUMBER || lex->last_token_type == TOKEN_MINUS || lex->last_token_type == TOKEN_LPAREN) {
		stark_pop();
		stark_push("termAdd",-1);
		stark_push("term",-1);
		stark_print(lex);
	}
		term(lex);
	do {
		if (lex->last_token_type == TOKEN_PLUS || lex->last_token_type == TOKEN_MINUS) {
			if(lex->last_token_type == TOKEN_PLUS) {
				stark_push("term",-1);
				stark_push("PLUS",-1);
				stark_print(lex);
				stark_pop();
				stark_print(lex);
			}
			else if(lex->last_token_type == TOKEN_MINUS) {
				stark_push("term",-1);
				stark_push("MINUS",-1);
				stark_print(lex);
				stark_pop();
				stark_print(lex);
			}
			PL0Lex_get_token(lex);
			term(lex);
		}
		else if(lex->last_token_type == TOKEN_SEMICOLON || lex->last_token_type == TOKEN_RPAREN || lex->last_token_type == TOKEN_PERIOD || isR(lex) == 1|| lex->last_token_type == TOKEN_END || lex->last_token_type == TOKEN_DO || lex->last_token_type == TOKEN_THEN) {
			stark_pop();
			stark_print(lex);
			break;
		}
	}while(1);
}

void term(PL0Lex * lex) {
	printf("analysis the term\n");
	if(lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_NUMBER || lex->last_token_type == TOKEN_MINUS || lex->last_token_type == TOKEN_LPAREN) {
		stark_pop();
		stark_push("factorMul",-1);
		stark_push("factor",-1);
		stark_print(lex);
	}
	factor(lex);
	do {
		if (lex->last_token_type == TOKEN_SLASH || lex->last_token_type == TOKEN_TIMES) {
			if(lex->last_token_type == TOKEN_SLASH) {
				stark_push("factor",-1);
				stark_push("SLASH",-1);
				stark_print(lex);
				stark_pop();
				stark_print(lex);
			}
			else if(lex->last_token_type == TOKEN_TIMES) {
				stark_push("factor",-1);
				stark_push("TIMES",-1);
				stark_print(lex);
				stark_pop();
				stark_print(lex);
			}
			PL0Lex_get_token(lex);
			factor(lex);
		}
		else if(lex->last_token_type == TOKEN_SEMICOLON || lex->last_token_type == TOKEN_RPAREN || lex->last_token_type == TOKEN_PERIOD || lex->last_token_type == TOKEN_PLUS || lex->last_token_type == TOKEN_MINUS || isR(lex) == 1 || lex->last_token_type == TOKEN_END || lex->last_token_type == TOKEN_DO || lex->last_token_type == TOKEN_THEN) {
			stark_pop();
			stark_print(lex);
			break;
		}
	}while(1);
}

void factor(PL0Lex * lex) {
	printf("analysis the factor\n");
	if (lex->last_token_type == TOKEN_IDENTIFIER) {
		stark_pop();
		stark_push("ident",-1);
		stark_print(lex);
		stark_pop();
		stark_print(lex);
		PL0Lex_get_token(lex);
	}
	else if (lex->last_token_type == TOKEN_NUMBER) {
		stark_pop();
		stark_push("number",-1);
		stark_print(lex);
		stark_pop();
		stark_print(lex);
		PL0Lex_get_token(lex);
	}
	else if (lex->last_token_type == TOKEN_MINUS) {
		stark_pop();
		stark_push("EXPR",-1);
		stark_push("MINUS",-1);
		stark_print(lex);
		stark_pop();
		stark_print(lex);
		PL0Lex_get_token(lex);
		expression(lex);
	}
	else if (lex->last_token_type == TOKEN_LPAREN) {
		stark_pop();
		stark_push("RPAREN",-1);
		stark_push("EXPR",-1);
		stark_push("LPAREN",-1);
		stark_print(lex);
		stark_pop();
		stark_print(lex);
		PL0Lex_get_token(lex);
		expression(lex);
		if (lex->last_token_type == TOKEN_RPAREN) {
			stark_pop();
			stark_print(lex);
			PL0Lex_get_token(lex);
		}
		else {
			printf("there must be a ')' to follow expr\n");
		}
	}
	else {
		printf("factor must start with 'ident', 'number', '-', '+'\n");
	}
}


void const_declaration(PL0Lex * lex) {
	if (lex->last_token_type == TOKEN_IDENTIFIER) {
		stark_pop();
		stark_push(",Con_declaration",-1);
		stark_push("number",-1);
		stark_push("EQU",-1);
		stark_push("ident",-1);
		stark_print(lex);
		stark_pop();
		stark_print(lex);
		PL0Lex_get_token(lex);
		if (lex->last_token_type == TOKEN_EQU || lex->last_token_type == TOKEN_BECOMES) {
			if (lex->last_token_type == TOKEN_BECOMES)
				printf("found ':=' when expecting '='\n");
			if(lex->last_token_type == TOKEN_EQU) {
				stark_pop();
				PL0Lex_get_token(lex);
				stark_print(lex);
			}
			if (lex->last_token_type == TOKEN_NUMBER) {
				//table_append(lex, ID_CONSTANT);
				stark_pop();
				stark_print(lex);
				PL0Lex_get_token(lex);
			} else {
				printf("there must be a number to follow '='\n");
			}
		} else {
			printf("there must be an '=' to follow the identifier\n");
		}
	} else {
		printf("There must be an identifier to follow 'const'\n");
	}
} //const_declaration

void var_declaration(PL0Lex * lex) {
	if (lex->last_token_type == TOKEN_IDENTIFIER) {
		//PL0Lex_get_token(lex);
		//table_append(lex, ID_VARIABLE);
		stark_pop();
		stark_push(",Var_declaration",-1);
		stark_push("ident",-1);
		stark_print(lex);
		stark_pop();
		stark_print(lex);
		PL0Lex_get_token(lex);
	}
	else {
		printf("there must be an identifier to follow 'var'");
	}
} //var_declaration

void statement(PL0Lex * lex) {
	printf("analysis the statement\n");
	if (lex->last_token_type == TOKEN_IDENTIFIER) {
		//printf("2\n");
		stark_pop();
		stark_push("EXPR",-1);
		stark_push("TOKEN_BECOMES",-1);
		stark_push("TOKEN_IDENTIFIER",-1);
		stark_print(lex);
		stark_pop();
		stark_print(lex);
		PL0Lex_get_token(lex);
		if(lex->last_token_type == TOKEN_BECOMES) {
			stark_pop();
			stark_print(lex);
			PL0Lex_get_token(lex);
			expression(lex);
		}
		//expression
		else {
			printf("there must be a ':=' to follow the identifier\n");
		}
	}
	else if (lex->last_token_type == TOKEN_CALL) {
		stark_pop();
		stark_push("ident",-1);
		stark_push("TOKEN_CALL",-1);
		stark_print(lex);
		stark_pop();
		PL0Lex_get_token(lex);
		if (lex->last_token_type == TOKEN_IDENTIFIER) {
			stark_pop();
			stark_print(lex);
			PL0Lex_get_token(lex);
		}
		else {
			printf("there must be an identifier to follow call\n");
		}
	}
	else if (lex->last_token_type == TOKEN_BEGIN) {
		stark_pop();
		stark_push("end",-1);
		stark_push("statements",-1);
		stark_push("SEMICOLON",-1);
		stark_push("statement",-1);
		stark_push("begin",-1);
		stark_print(lex);
		stark_pop();
		stark_print(lex);
		PL0Lex_get_token(lex);
		/*statement sequence*/
		do {
			//printf("1\n");
			if(lex->last_token_type == TOKEN_BEGIN || lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_CALL || lex->last_token_type == TOKEN_IF || lex->last_token_type == TOKEN_WHILE) {
			statement(lex);
			//printf("%d\n",lex->last_token_type);
			if (lex->last_token_type == TOKEN_SEMICOLON) {
				stark_pop();
				stark_print(lex);
				PL0Lex_get_token(lex);
				if(lex->last_token_type == TOKEN_BEGIN || lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_CALL || lex->last_token_type == TOKEN_IF || lex->last_token_type == TOKEN_WHILE) {
					stark_push("SEMICOLON",-1);
					stark_push("statement",-1);
					stark_print(lex);
				}
				else if(lex->last_token_type == TOKEN_END) {
					stark_pop();
					stark_print(lex);
					break;
				}
			}
			}
		}while(1);
		if (lex->last_token_type == TOKEN_END) {
			stark_pop();
			PL0Lex_get_token(lex);
			stark_print(lex);
		}
		else {
			 printf("there must be an 'end' to follow statements\n");
		}
	}
	else if (lex->last_token_type == TOKEN_IF) {
		stark_pop();
		stark_push("statement",-1);
		stark_push("THEN",-1);
		stark_push("condition",-1);
		stark_push("IF",-1);
		stark_print(lex);
		stark_pop();
		PL0Lex_get_token(lex);
		stark_print(lex);
		condition(lex);
		if(lex->last_token_type == TOKEN_THEN) {
			stark_pop();
			PL0Lex_get_token(lex);
			stark_print(lex);
			statement(lex);
		}
		else {
			printf("there must be a 'then' follow to condition\n");
		}
	}
	else if (lex->last_token_type == TOKEN_WHILE) {
		stark_pop();
		stark_push("statement",-1);
		stark_push("DO",-1);
		stark_push("condition",-1);
		stark_push("WHILE",-1);
		stark_print(lex);
		stark_pop();
		PL0Lex_get_token(lex);
		stark_print(lex);
		condition(lex);
		if (lex->last_token_type == TOKEN_DO) {
			stark_pop();
			PL0Lex_get_token(lex);
			stark_print(lex);
			statement(lex);
		}
		else {
			printf("there must be a 'do' follow to condition\n");
		}
	}
	else {
		printf("statement must begin with 'identifier', 'call', 'begin', 'if', 'while'\n");
	}
}
int first_of_program_block(PL0Lex * lex) {
	if(lex->last_token_type == TOKEN_CONST) return 1;
	else if(lex->last_token_type == TOKEN_VAR) return 1;
	else if(lex->last_token_type == TOKEN_PROCEDURE) return 1;
	else if(lex->last_token_type == TOKEN_IF) return 1;
	else if(lex->last_token_type == TOKEN_BEGIN) return 1;
	else if(lex->last_token_type == TOKEN_CALL) return 1;
	else if(lex->last_token_type == TOKEN_IDENTIFIER) return 1;
	else if(lex->last_token_type == TOKEN_WHILE) return 1;
	else return 0;
}
void program(PL0Lex * lex) {
	if(first_of_program_block(lex) == 1) {
		if(stark_index != 0) {
			stark_index = 0;
		}
		stark_push("PERIOD",-1);
		stark_push("program_block",-1);
		stark_print(lex);
		program_block(lex);
		if(lex->last_token_type == TOKEN_PERIOD) {
			stark_pop();
			stark_push("$",-1);
			stark_print(lex);
		}
	}
}
void program_block(PL0Lex * lex) {
	printf("analysis the program block\n");
	// PL0Lex * lex = (PL0Lex *) calloc(1, sizeof(PL0Lex));
	// PL0Lex_get_token(lex);
	while(lex->last_token_type == TOKEN_CONST || lex->last_token_type == TOKEN_VAR || lex->last_token_type == TOKEN_PROCEDURE)
	 {
		stark_push("declaration",-1);
		stark_print(lex);
		if (lex->last_token_type == TOKEN_CONST) {
			stark_pop();
			stark_push("CONST",-1);
			stark_print(lex);
			stark_pop();
			stark_push("SEMICOLON",-1);
			stark_push("Con_declaration",-1);
			stark_push("const",-1);
			stark_print(lex);
			stark_pop();
			PL0Lex_get_token(lex);
			stark_print(lex);
			const_declaration(lex);
				while (lex->last_token_type == TOKEN_COMMA) {
					stark_pop();
					stark_push("Con_declaration",-1);
					stark_push("COMMA",-1);
					stark_print(lex);
					stark_pop();
					stark_print(lex);
					PL0Lex_get_token(lex);
					const_declaration(lex);
				}
				stark_pop();
				stark_print(lex);
				if (lex->last_token_type == TOKEN_SEMICOLON) {
					stark_pop();
					stark_print(lex);
					PL0Lex_get_token(lex);
				}
				else {
					printf("missing ',' or ';'\n");
				}
		}
		else if (lex->last_token_type == TOKEN_VAR) {
			stark_pop();
			stark_push("VAR",-1);
			stark_print(lex);
			stark_pop();
			stark_push("SEMICOLON",-1);
			stark_push("Var_declaration",-1);
			stark_push("var",-1);
			stark_print(lex);
			stark_pop();
			stark_print(lex);
			PL0Lex_get_token(lex);
				var_declaration(lex);
				while (lex->last_token_type == TOKEN_COMMA) {
					stark_pop();
					stark_push("Var_declaration",-1);
					stark_push("COMMA",-1);
					stark_print(lex);
					stark_pop();
					stark_print(lex);
					PL0Lex_get_token(lex);
					var_declaration(lex);
				}
				if(lex->last_token_type == TOKEN_SEMICOLON) {
					stark_pop();
					stark_print(lex);
				}
				if (lex->last_token_type == TOKEN_SEMICOLON) {
					stark_pop();
					stark_print(lex);
					PL0Lex_get_token(lex);
				}
				else {
					printf("missing ';' or ','\n");
				}
		}
		else if (lex->last_token_type == TOKEN_PROCEDURE) {
			stark_pop();
			stark_push("PROC",-1);
			stark_print(lex);
			stark_pop();
			stark_push("SEMICOLON",-1);
			stark_push("program_block",-1);
			stark_push("SEMICOLON",-1);
			stark_push("ident",-1);
			stark_push("procedure",-1);
			stark_print(lex);
			stark_pop();
			PL0Lex_get_token(lex);
			stark_print(lex);
			if (lex->last_token_type == TOKEN_IDENTIFIER) {
				stark_pop();
				PL0Lex_get_token(lex);
				stark_print(lex);
				if(lex->last_token_type == TOKEN_SEMICOLON) {
					stark_pop();
					PL0Lex_get_token(lex);
					stark_print(lex);
					program_block(lex);
					//table_append(lex, ID_VARIABLE);
					if(lex->last_token_type == TOKEN_SEMICOLON) {
						stark_pop();
						PL0Lex_get_token(lex);
						stark_print(lex);
			 		}
					else {
						printf("missing ';'");
					}
				}
				else {
					printf("missing ';'");
				}
			}
			else {
				printf("There must be an identifier to follow 'procedure'\n");
			}
		}
	}
	if(lex->last_token_type == TOKEN_IDENTIFIER || lex->last_token_type == TOKEN_IF || lex->last_token_type == TOKEN_BEGIN || lex->last_token_type == TOKEN_CALL || lex->last_token_type == TOKEN_WHILE) {
		stark_pop();
		stark_push("statement",-1);
		stark_print(lex);
		/*if(lex->last_token_type == TOKEN_IDENTIFIER) {
			stark_pop();
			stark_push("EXPR",-1);
			stark_push("TOKEN_BECOMES",-1);
			stark_push("TOKEN_IDENTIFIER",-1);
			stark_print(lex);
			stark_pop();
			stark_print(lex);
			PL0Lex_get_token(lex);
			if(lex->last_token_type == TOKEN_BECOMES) {
				stark_pop();
				stark_print(lex);
				PL0Lex_get_token(lex);
				expression(lex);
			}
		}*/
		statement(lex);
	}
	//} while(lex->last_token_type == TOKEN_CONST || lex->last_token_type == TOKEN_VAR || lex->last_token_type == TOKEN_PROCEDURE);
	//statement
	//statement(lex);
	//PL0Lex_get_token(lex);
	/*if (lex->last_token_type == TOKEN_PERIOD) {
		printf("it is the end of program_block\n");
		PL0Lex_get_token(lex);
	}
	else {
		printf("there should be a 'PERIOD' follow to statement and at the end of program\n");
	}*/
} //program_block
