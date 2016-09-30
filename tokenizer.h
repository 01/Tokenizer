/*
 * CS211 CompArch
 * Andrew Khazanovich
 * tokenizer.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ctype.h was included to allow for the use of isspace(int c) method which allowed checking if white space was a blank space, a tab or a line feed
 */
#include <ctype.h>

/* 
 *	Since the tokens could only be one of 5 types, hexadecimal, decimal, octal, and floating point or non token the strings corresponding to the types 
 *	to be stored and printed are predefined 
 */

#define hex "hex" 


#ifndef _TOKENIZER_H
#define _TOKENIZER_H


/*
 *  The TokenizerT struct is a linked List. It is defined as a pointer to the head node. but a pointer to the tail node is kept
 *	to allow quick add to back additions to the Linked List. A token_node structure keeps a character pointer 
 *  to a token string and a character pointer to the literal string type of the token, 
 *	along with a pointer to the next token node if there is one.
 *
 */

struct token_node {
	char *type;
	char *token;
	struct token_node *next;
};


typedef struct TokenizerT_ {
	struct token_node *head;
	struct token_node *tail;

} TokenizerT;

/*
 *	getType returns the literal string defined by the preprocessor that corresponds to the type for a given token character string. 
 *  It uses a number of helper methods to determine if a given token string is a float, hex, octal, decimal or non valid token.
 *
 *	isDigit returns the integer 1 if given character is a number 1-9 and 0 if not
 *	isOctal returns the integer 1 if a given token string is an Octal and 0 if not
 *  isHex returns the integer 1 if a given token string is a Hexadecimal number and 0 if not
 *	isFloatChar returns 1 if given token string contains the letter e or capital E or the character '.' otherwise returns 0
 *	isFloat returns 1 if a given token string is a floating point otherwise returns 0
 */

char *getType(char * token);

int isDigit(char a);

int isFloatChar(char a);

int isOctal(char * token);

int isHex(char * token);

int isFloat(char * token);





/*
 * TKCreate creates a new TokenizerT object for a given file name
 * TKCreate should open the file and read the file line by line.
 * If you are following the steps in description, 'lexer' / 'parser' should both be implemented here
 * If you are not following the steps, the output of TKCreate should be a pointer to a TokenizerT object which contains all the information needed for writer
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts );


/*
 * TKDestroy destroys a TokenizerT object.  It frees all dynamically
 * allocated memory that is part of the object being destroyed.
 * this frees 
 */

void TKDestroy( TokenizerT * tk );


/*
 * TKGetNextToken is a helper function that would be used by TKCreate when traversing through each line in the file
 * This function would take a char* as input, indicating where it should start
 * Then returns a char* pointing to a string which is a copy of the next token
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( char * start );
    

/*
 * TKPrint would take a Tokenizer object and use the information stored in that object to print out all the result to file "result" and file "error.msg"
 * If you are following the result, implement the 'writer' here
 */

void TKPrint(TokenizerT *tk);



#endif
