#include <stdlib.h>
#include <string.h> //use strcmp() function
//#include <bits/mathcalls.h>
#include "../common/common.h"
#include "pl0_lex.h"

const char * TOKEN_RESERVED_WORDS[NRW] = {"var", "const", "procedure", "begin", "end", "if", "then", "do", "while", "call", "odd"};
const char * TOKEN_SYMBOLS[NSYM] = { "+", "-", "*", "/", "=", "!=", "<", "<=", ">", ">=", "(", ")", ",", ";", ".", ":=" };
int lex_count = 0;
int lex_line = 0;
int stream[20000];
int line_count = 0;

PL0Lex * PL0Lex_create(PL0Compiler * parent)
{
	PL0Lex * lex = (PL0Lex *) calloc(1, sizeof(PL0Lex));
	lex->compiler = parent;
	int i = -1;
	do
	{
	        i++;
		stream[i] = fgetc(fin);
		printf("%c",stream[i]);
	}while(stream[i]>-1);
	return lex;
}

BOOL PL0Lex_destroy(PL0Lex * lex)
{
	free(lex);
	return TRUE;
}

int mypow(int x ,int y )
{
  int result = 1; 
  if(y == 0)
    return 1;
  else
  {
    while(y >= 1)
    {
      result = result * x;
      y = y - 1;
    }
    return result;
  }
}
   
BOOL isnumber(int stm)
{
    if(stm <= 57 && stm >= 48)
		return TRUE;
	else
		return FALSE;
}

BOOL isletter_(int stm)
{
	if(stm <= 90 && stm >= 65)
		return TRUE;
	else if(stm <= 122 && stm >= 97)
		return TRUE;
	else if(stm == 95)
		return TRUE;
	else
		return FALSE;
}

BOOL issymbol(int stm)
{
	//+ - * / = != < <= > >= ( ) , ; . :=
	if(stm == 43) //+
	return TRUE;
	if(stm == 45) //-
	return TRUE;
	if(stm == 42) //*
	return TRUE;
	if(stm == 47) ///
	return TRUE;
	if(stm == 61) //=
	return TRUE;
	if(stm == 60) //<
	return TRUE;
	if(stm == 62) //>
	return TRUE;
	if(stm == 40) //(
	return TRUE;
	if(stm == 41) //)
	return TRUE;
	if(stm == 44) //,
	return TRUE;
	if(stm == 59) //;
	return TRUE;
	if(stm == 46) //.
	return TRUE;
	else 
	return FALSE;
}
BOOL PL0Lex_get_token(PL0Lex * lex)
{
	int mark;
	while(1)
	{
		mark = 0;
		/*COME ACROSS END OF FILE*/
		if(stream[lex_count] == -1)
		{
			mark = 1;
			return FALSE;
		}
		if((char)stream[lex_count]=='/'&&(char)stream[lex_count+1]=='/')
		{
			mark = 1;
			while((char)stream[lex_count] != '\n' && (char)stream[lex_count] != EOF)
			{
			    lex_count = lex_count + 1;
				line_count = line_count + 1;
			}
			if((char)stream[lex_count] == EOF);
			else
			{
			    lex_count = lex_count + 1;
			    lex_line = lex_line + 1;
				line_count = 0;
			}
			continue;
		}

		if((char)stream[lex_count]=='/'&&(char)stream[lex_count+1]=='*')
		{
			mark = 1;
			lex_count = lex_count + 2;
			line_count = line_count + 2;
			int ori_lex_count = lex_count;
			int ori_line_count = line_count;
			int ori_lex_line = lex_line;
			/* when we meet single /*, report an error and go on*/
			if((char)stream[lex_count] == EOF)
			{ 	
				printf("error whit a single '/*' at line:%d, locate:%d-%d\n",lex_line,line_count-2,line_count-1);
				continue; 
			}
			else
			{
				while(1)
				{
                     			 lex_count = lex_count + 1;
					 line_count = line_count + 1;
					 if((char)stream[lex_count] == '\n')
					    {
							lex_line = lex_line + 1;
							line_count = 0;
					    } 
					 if(stream[lex_count] == -1)
					 {
					   printf("error whit a single '/*' at line:%d, locate:%d-%d\n",ori_lex_line,ori_line_count-2,ori_line_count-1);
					   lex_count = ori_lex_count-2;
					   line_count = ori_line_count-2;
					   lex_line = ori_lex_line;
					   break;
					 }
					 if((char)stream[lex_count]=='*'&&(char)stream[lex_count+1]=='/')
					   break;
				}
				lex_count = lex_count + 2;
				line_count = line_count + 2;
				continue;
			}
		}

		if( stream[lex_count] >= 97 && stream[lex_count] <= 122) // if true stream[lex_count] is the small letter
		{
			mark = 1;
			lex->first_ch = lex_count;
			lex->first_line_ch = line_count;
			while(stream[lex_count]>= 97 && stream[lex_count] <= 122)
			{
				lex_count = lex_count + 1;
				line_count = line_count + 1;
			}
			//if(isletter_(stream[lex_count]) == FALSE)
			//{
				char reword[100];
				int i;
				for(i = 0 ; i <= lex_count - lex->first_ch - 1 ; i++)
				{
					reword[i] = (char)stream[lex->first_ch+i];
				}
				reword[i] = '\0';
				for(i = 0 ; i <= NRW - 1 ; i++)
				{
					if(strcmp(reword,TOKEN_RESERVED_WORDS[i]) == 0) //token is a reserved word
					{
						 lex->last_token_type = i + 21;
						 lex->line = lex_line;
						 lex->last_ch = lex_count - 1;
						 lex->last_line_ch = line_count -1;
						 return TRUE;
					}
				}
				if(i == NRW)  //token is not a reserved word
				{
					lex_count = lex->first_ch; // pointer reback
					line_count = lex->first_line_ch;
				}
			//}
		}
		// check number
	/*else*///if(isnumber(stream[lex_count]) == TRUE || (char)stream[lex_count] == '-')
		if(isnumber(stream[lex_count]) == TRUE)
		{
			mark = 1;
			lex->first_ch = lex_count;
			lex->first_line_ch = line_count;
				while(isnumber(stream[lex_count]) == TRUE)
				{
					lex_count = lex_count + 1;
					line_count = line_count + 1;
				}
				if(isletter_(stream[lex_count]) == TRUE)
				{
					lex->last_token_type = 0;
					while(isletter_(stream[lex_count]) == TRUE)
					{
					  lex_count = lex_count + 1;
					  line_count = line_count + 1;
					}
					return TRUE;
				}
				else
				{
					lex->last_token_type = TOKEN_NUMBER;
					lex->line = lex_line;
					lex->last_ch = lex_count - 1;
					lex->last_line_ch = line_count - 1;
					lex->last_num = 0;
					int i;
					int j;
					if(lex->last_ch-lex->first_ch>8)
					{
					  printf("invaild number\n");
					  lex->last_token_type = TOKEN_NULL;
					  return TRUE;
					}
					for(i = lex->last_ch ; i >= lex->first_ch ; i--)
					{
						j = lex->last_ch - i;
						lex->last_num = lex->last_num + (stream[i] - 48)*mypow(10,j);
					}
					//else
					return TRUE;
				}
			
		}
		// check identifier
		/*else*/ //if(isletter_(stream[lex_count]) == TRUE && isnumber(stream[lex_count]) == FALSE)
		if(isletter_(stream[lex_count]) == TRUE)
		{
			mark = 1;
			lex->first_ch = lex_count;
			lex->first_line_ch = line_count;
			while(isletter_(stream[lex_count]) == TRUE || isnumber(stream[lex_count]) == TRUE)
			{
				lex_count = lex_count + 1;
				line_count = line_count + 1;
			}
			lex->last_token_type = TOKEN_IDENTIFIER;
			lex->last_ch = lex_count - 1;
			lex->last_line_ch = line_count - 1;
			lex->line = lex_line;
			int i;
			int length;
			length = lex->last_ch - lex->first_ch;
			if(length >= 11)
			{
			  printf("invaild ch\n");
			  lex->last_token_type = TOKEN_NULL;
		          return TRUE;
			}
			for(i = 0; i <= length; i++)
			{
				lex->last_id[i] = (char)stream[lex->first_ch + i];
			}
			lex->last_id[i] = '\0';
			return TRUE;
		}
		// check symbol
	/*else*/if(issymbol(stream[lex_count]) == TRUE || (stream[lex_count] == 33 && stream[lex_count + 1] == 61) || (stream[lex_count] == 58 && stream[lex_count + 1] == 61))
		{
			mark = 1;
			if(stream[lex_count] == 33 && stream[lex_count + 1] == 61) //!=
			{
				lex->last_token_type = TOKEN_NEQ;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count + 1;
				lex->last_line_ch = line_count + 1;
				lex_count = lex_count + 2;
				line_count = line_count + 2;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 58 && stream[lex_count + 1] == 61) //:=
			{
				lex->last_token_type = TOKEN_BECOMES;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count + 1;
				lex->last_line_ch = line_count + 1;
				lex_count = lex_count + 2;
				line_count = line_count + 2;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 43) //+
			{
				lex->last_token_type = TOKEN_PLUS;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 45) //-
			{
				lex->last_token_type = TOKEN_MINUS;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 42) //*
			{
				lex->last_token_type = TOKEN_TIMES;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 47) ///
			{
				lex->last_token_type = TOKEN_SLASH;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 61) //=
			{
				lex->last_token_type = TOKEN_EQU;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 60 && stream[lex_count + 1] == 61) //<=
			{
				lex->last_token_type = TOKEN_LEQ;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count + 1;
				lex->last_line_ch = line_count + 1;
				lex_count = lex_count + 2;
				line_count = line_count + 2;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 62 && stream[lex_count + 1] == 61) //>=
			{
				lex->last_token_type = TOKEN_GEQ;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count + 1;
				lex->last_line_ch = line_count - 1;
				lex_count = lex_count + 2;
				line_count = line_count + 2;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 60) //<
			{
				lex->last_token_type = TOKEN_LES;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 62) //>
			{
				lex->last_token_type = TOKEN_GTR;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 40) //(
			{
				lex->last_token_type = TOKEN_LPAREN;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 41) //)
			{
				lex->last_token_type = TOKEN_RPAREN;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 44) //,
			{
				lex->last_token_type = TOKEN_COMMA;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 59) //;
			{
				lex->last_token_type = TOKEN_SEMICOLON;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}
			/*else*/ if(stream[lex_count] == 46) //.
			{
				lex->last_token_type = TOKEN_PERIOD;
				lex->first_ch = lex_count;
				lex->first_line_ch = line_count;
				lex->line = lex_line;
				lex->last_ch = lex_count;
				lex->last_line_ch = line_count;
				lex_count = lex_count + 1;
				line_count = line_count + 1;
				return TRUE;
			}		
		}
		// check space
		/*else*/ if((char)stream[lex_count] == ' ')
		{
			mark = 1;
			lex_count = lex_count + 1;
			line_count = line_count + 1;
			continue;
		}
		/*else*/ if((char)stream[lex_count] == '\t')
		{
			mark = 1;
			lex_count = lex_count + 1;
			line_count = line_count + 1;
			continue;
		}
		/*else*/ if((char)stream[lex_count] == '\n')
		{
			mark = 1;
			lex_count = lex_count + 1;
			lex_line = lex_line + 1;
			line_count = 0;
			continue;
		}
		/*else*/ if((char)stream[lex_count] == '\r')
		{
			mark = 1;
			lex_count = lex_count + 1;
			line_count = line_count + 1;
			line_count = 0;
			continue;
		}
		//if(isletter_(stream[lex_count])==FALSE&&isnumber(stream[lex_count])==FALSE&&issymbol(stream[lex_count])==FALSE&&((char)stream[lex_count]!=' ')&&((char)stream[lex_count]!='\t')&&((char)stream[lex_count]!='\n')&&((char)stream[lex_count]!='\r'))
		if(mark == 0)
		{
		  lex->last_token_type = TOKEN_NULL;
		  lex_count = lex_count + 1;
		  line_count = line_count + 1;
		  return TRUE;
		}
		
	}
       return FALSE;
}
