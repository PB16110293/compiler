#ifndef __PL0_TAX_H__
#define __PL0_TAX_H__

#define TXMAX				500 		// max length of token table
#define MAXADDRESS 	32767		// max address
#define MAX_ID_LEN	10
/*variables for token table*/
extern int table_index;
extern int stark_index;
/*struct for token table*/
typedef struct {
	char name[MAX_ID_LEN + 1];
	int kind;
	int value;
	short level;
	short address;
} Table;

/*definition for token table*/
typedef struct {
	char name[50];
	int value;
	int flag;
} Stark;
Table token_table[TXMAX];
Stark token_stark[TXMAX];
/*identifier type*/
enum idtype {
	ID_CONSTANT,
	ID_VARIABLE,
	ID_PROCEDURE
};


/*operations for token table*/
void table_append(PL0Lex * lex, int kind);
void stark_push(char * ch, int num);
int first_of_program_block(PL0Lex * lex);
int isR(PL0Lex * lex);
/*functions for syntax analysis*/
void statement(PL0Lex * lex); //analysis the statement
void condition(PL0Lex * lex); //analysis the condition statement
void expression(PL0Lex * lex); //analysis the expression
void term(PL0Lex * lex); //analysis the term
void factor(PL0Lex * lex); //analysis the factor
void program_block(PL0Lex * lex); //analysis the program block
void program(PL0Lex * lex);
#endif //pl0_tax.h
