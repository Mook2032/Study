/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"
#include "scanner.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank() {
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_SPACE))
    readChar();
}

void skipComment() {
  int state = 0;
  while ((currentChar != EOF) && (state < 2)) {
    switch (charCodes[currentChar]) {
    case CHAR_TIMES:
      state = 1;
      break;
    case CHAR_RPAR:
      if (state == 1) state = 2;
      else state = 0;
      break;
    default:
      state = 0;
    }
    readChar();
  }
  if (state != 2) 
    error(ERR_END_OF_COMMENT, lineNo, colNo);
}

Token* readIdentKeyword(void) {
  Token *token = makeToken(TK_NONE, lineNo, colNo);
  int count = 1;

  token->string[0] = toupper((char)currentChar);
  readChar();

  while ((currentChar != EOF) && 
	 ((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT))) {
    if (count <= MAX_IDENT_LEN) token->string[count++] = toupper((char)currentChar);
    readChar();
  }

  if (count > MAX_IDENT_LEN) {
    error(ERR_IDENT_TOO_LONG, token->lineNo, token->colNo);
    return token;
  }

  token->string[count] = '\0';
  token->tokenType = checkKeyword(token->string);

  if (token->tokenType == TK_NONE)
    token->tokenType = TK_IDENT;

  return token;
}

//Dear me, we need a special function to work with float number first.
//We will have to call it from inside readNumber(), so it will be the only odd ball in "read series"
//	that take a Token as argument.
Token* readFloat(Token* token,int count) {
  if (count==0) token->string[count++]='0';
	token->string[count++]='.';
	int oldCount=count;
	int ln,cn;
	
	ln=lineNo;
	cn=colNo;
	
	readChar();
	if (charCodes[currentChar]==CHAR_RPAR)
	{
		savedToken= makeToken(SB_RSEL, ln, cn); 	//Just because...
		readChar();
		token->string[count]='\0';
		token->value=atoi(token->string);
		
		return token;
	}
	//copy paste again, from readNumber below
	else
	{
		token->tokenType=TK_FLOAT;
	
	    while (charCodes[currentChar]==CHAR_DIGIT)
	      {
		if (count<MAX_IDENT_LEN)
		{
			token->string[count++]=(char)currentChar;
			readChar();											//Dear me, the finisher is here.
			if (currentChar==EOF) break;
		}
		else
		{
			error(ERR_IDENT_TOO_LONG,token->lineNo,token->colNo);
			token->string[MAX_IDENT_LEN]='\0';
		}
	      }
	    if (oldCount==count) token->string[count++]='0';
	    token->string[count]='\0';
	    token->valueFloat=atof(token->string);
	    return token;
	}
}

Token* readNumber(void) {
	Token* token=makeToken(TK_INT,lineNo,colNo);
	int count=0;
	while (charCodes[currentChar]==CHAR_DIGIT)
	{
		if (count<MAX_IDENT_LEN)
		{
			token->string[count++]=(char)currentChar;
			readChar();											//Dear me, the finisher is here.
			if (currentChar==EOF) break;
		}
		else
		{
			error(ERR_IDENT_TOO_LONG,token->lineNo,token->colNo);
			token->string[MAX_IDENT_LEN]='\0';
		}
	}
	if (charCodes[currentChar]==CHAR_PERIOD)
	  {
	    return readFloat(token,count);	//It's over yet!!
	  }
	
	token->string[count]='\0';
	token->value=atoi(token->string);
		
	return token;
}

Token* readString(void) {
	Token* token=makeToken(TK_STRING, lineNo, colNo);
	int count=0;
	readChar();
	if (currentChar == EOF) {
		token->tokenType = TK_NONE;
		error(ERR_INVALID_STRING, token->lineNo, token->colNo);	//The program should quit here, but...
		return token;		//...just to be safe.
	}	//Copy from above. And let's add a new error message, as well.
	while ((charCodes[currentChar]!=CHAR_QUOTE)&&(currentChar!='\n'))
	{
		if (count<MAX_IDENT_LEN)
		{
		  if (currentChar==92)
		  {
			   readChar();
			   switch (currentChar)
			     {
			     case '"': break;
			     case 't': currentChar='\t'; break;
			     case 'n': currentChar='\n'; break;
			     case '\'': currentChar='\'';break;
			     case '?': currentChar= '\?'; break;
			     case '\\': currentChar='\\'; break;
			     case '0': currentChar='\0';break;
			     case 'a': currentChar= '\a'; break;
			     case 'b': currentChar= '\b'; break;
			     case 'f': currentChar= '\f'; break;
			     case 'r': currentChar= '\r'; break;
			     case 'v': currentChar= '\v'; break;
			     case '\n': readChar(); break;
			     default:
			       token->string[count++]=(char)currentChar;
			       if (count>=MAX_IDENT_LEN)
				 {
				     error(ERR_IDENT_TOO_LONG,token->lineNo,token->colNo);
				     return token;
		 		 }
			     }

		  }
		  
			token->string[count++]=(char)currentChar;
			readChar();
			if (currentChar==EOF)
			{
				token->tokenType = TK_NONE;
				error(ERR_INVALID_STRING, token->lineNo, token->colNo);
				return token;
			}
		}
		else
		{
			error(ERR_IDENT_TOO_LONG,token->lineNo,token->colNo);
			return token;
		}
	}
	if (currentChar=='\n') error(ERR_INVALID_STRING, token->lineNo, token->colNo);
	token->string[count]='\0';
	readChar();				//Uh, unlike other function, this one stop when it get to see that CHAR_QUOTE dude,
							//so we have to append one to the end.
	return token;
}

Token* readConstChar(void) {
  Token *token = makeToken(TK_CHAR, lineNo, colNo);

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
    return token;
  }
    
  token->string[0] = currentChar;
  token->string[1] = '\0';

  readChar();
  if (currentChar == EOF) {
    token->tokenType = TK_NONE;
    error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
    return token;
  }

  if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {
    readChar();
    return token;
  } else {
    token->tokenType = TK_NONE;
    error(ERR_INVALID_CONSTANT_CHAR, token->lineNo, token->colNo);
    return token;
  }
}

Token* getToken(void) {
  Token *token;
  int ln, cn;

  if (savedToken !=NULL)
  {
	token=savedToken;
	savedToken=NULL;
	return token;
  }
  
  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);	
	
  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();
  case CHAR_LETTER: return readIdentKeyword();
  case CHAR_DIGIT: return readNumber();
  case CHAR_SINGLEQUOTE: return readConstChar();
  case CHAR_QUOTE: return readString();
  case CHAR_PLUS: 
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar(); 
    return token;
  case CHAR_MINUS:
	token = makeToken(SB_MINUS, lineNo,colNo);
	readChar();
	return token;
  case CHAR_TIMES:
	token = makeToken(SB_TIMES, lineNo,colNo);
	readChar();
	return token;
  case CHAR_SLASH:
	token = makeToken(SB_SLASH, lineNo, colNo);
	readChar();
	return token;
  case CHAR_COLON:
	ln = lineNo;
	cn = colNo;
	readChar();
	if (currentChar!=EOF)
	{
		if (charCodes[currentChar]==CHAR_EQ)
		{
			token = makeToken(SB_ASSIGN, ln, cn);
			readChar();
			return token;
		}
	}
	token = makeToken(SB_COLON, ln, cn);
	return token;
  case CHAR_SEMICOLON:
	token = makeToken(SB_SEMICOLON, lineNo, colNo);
	readChar();
	return token;
  case CHAR_PERIOD:
	ln=lineNo;
	cn=colNo;
	readChar();
	if (currentChar!=EOF)
	{
		if (charCodes[currentChar]==CHAR_RPAR)
		{
			token = makeToken(SB_RSEL, ln, cn);
			readChar();
			return token;
		}
		else if (charCodes[currentChar]==CHAR_DIGIT)
		  {
		    token=makeToken(TK_FLOAT,ln,cn);
		    return readFloat(token,0);
		  }
	}
	token = makeToken(SB_PERIOD, ln, cn);
	return token;
  case CHAR_COMMA:
	token = makeToken(SB_COMMA, lineNo, colNo);
	readChar();
	return token;
  case CHAR_EQ:
	token = makeToken(SB_EQ, lineNo, colNo);
	readChar();
	return token;
  case CHAR_LT:
	ln = lineNo;
	cn = colNo;
	readChar();
	if (currentChar!=EOF)
	{
		if (charCodes[currentChar]==CHAR_EQ)
		{
			token = makeToken(SB_LE, ln, cn);
			readChar();
			return token;
		}
	}
	token = makeToken(SB_LT, lineNo, colNo);
	return token;
  case CHAR_GT:
  	ln = lineNo;
	cn = colNo;
	readChar();
	if (currentChar!=EOF)
	{
		if (charCodes[currentChar]==CHAR_EQ)
		{
			token = makeToken(SB_GE, ln, cn);
			readChar();
			return token;
		}
	}
	token = makeToken(SB_GT, ln, cn);
	return token;
  case CHAR_EXCLAIMATION:
    ln = lineNo;
	cn = colNo;
	readChar();
	if (currentChar!=EOF)
	{
		if (charCodes[currentChar]==CHAR_EQ)
		{
			token = makeToken(SB_NEQ, ln, cn);
			readChar();
			return token;
		}
	}
	token = makeToken(TK_NONE, ln, cn);
	error(ERR_INVALID_SYMBOL, ln, cn);
	return token;
  case CHAR_LPAR:
	ln = lineNo;
	cn = colNo;
	readChar();
	if (currentChar!=EOF)
	{
		if (charCodes[currentChar]==CHAR_PERIOD)
		{
			token=makeToken(SB_LSEL, ln, cn);
			readChar();
			return token;
		}
		if (charCodes[currentChar]==CHAR_TIMES)
		{
			skipComment();
			return getToken();						//Copy sensei \(^O^)/
		}
	}
	token = makeToken(SB_LPAR, ln, cn);
	return token;
  case CHAR_RPAR:
	token = makeToken(SB_RPAR, lineNo, colNo);
	readChar();
	return token;
  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALID_SYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}

Token* getValidToken(void) {
  Token *token = getToken();
  while (token->tokenType == TK_NONE) {
    free(token);
    token = getToken();
  }


  //printToken(token);


  return token;
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_INT: printf("TK_INT(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;
  case TK_FLOAT: printf("TK_FLOAT(%s)\n", token->string); break;
  case TK_STRING: printf("TK_STRING(%s)\n", token->string); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_STRING: printf("KW_STRING\n"); break;
  case KW_FLOAT: printf("KW_FLOAT\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_MOD: printf("SB_MOD\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}