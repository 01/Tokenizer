/*
 * CS211 CompArch
 * Andrew Khazanovich
 * tokenizer.c
 */
#include "tokenizer.h"


/* "is" helper methods are utilized by getType method to determine the type of a token
 * be it hexadecimal, decimal, octal, float or non token. Al 
 * "is" helper method returns a 0 if false or 1 if true */


int isDigit(char a){										//isDigit simply returns 1 if a character is the character 1-9 and 0 if its not
	if(a <'0'|| a > '9'){
		return 0;
	}
	else return 1;
}

int isFloatChar(char a){									//isFloatChar is a helper method used by isFloat method. It takes in a character and 
	if(a == 'e' || a == 'E' || a == '.') return 1;			//a returns 1 (true) if the character is an 'e', 'E' or '.' and 0 (false) otherise
	else return 0;
}

int isOctal(char * token){									//isOctal takes in a character string token and returns 1 (true) if its octal and 0 otherwise
	if(token[0]!= '0') return 0;							//initial check of first character since all octal token will begin with a 0
	
	for (int i = 0; i < strlen(token); ++i)					//After initial check the token is looped through each character. Since an octal will only have digits
	{														//the method will return 0 (false) if a non digit is found
		if(!(isDigit(token[i]))) return 0;
	}

	return 1;
}

int isHex(char * token){									//isHex takes in character string and returns 1 (true) if its a hexadecimal and 0 if its not
	if(strlen(token)<3) return 0;							//method does initial check of the strings length as a hexadecimal will have at least 3 characters
	if(token[1] == 'X' || token[1] == 'x') return 1;		//Since the input is limited to valid tokens and single character invalid tokens simple check of 2nd character
	else return 0;										 	//is all the is necessary to determine if string is a hexadecimal
}

int isFloat(char * token){									//isFloat takes in a character string token and returns 1 (true) if the token is a floating point and 0 if its not
	for (int i = 0; i < strlen(token); ++i)					//loops through each character and uses to isFloatChar method, if not float characters are found it returns 0.
	{
		if(isFloatChar(token[i])) return 1;
	}
	return 0;
}


/*
 * getType method takes in a token and utilizes "is" Type helper methods to determine the deci of the token. It returns the corresponding string literal
 * to the type, which are "hex", "decimal" , "float", "octal", and "error". For efficiency and to minimize method calls conditionals are utilized 
 * to narrow down the possible type of the token and only call helper methods with necessary */

char* getType(char * token){
	char *tok;

	if(strlen(token)==1){									//To take advantage of the limited scope of possible inputs, a token of character length one can
		if(isDigit(token[0])){								//can only be an invalid token, or a decimal, e.g if its a digit its a decimal if its not its an 
			tok = "Decimal";								//invalid token 
			return tok;
		}
		else{
			
			tok="error";
			return tok ;
		}
	}
	else if(token[0] == '0'){								//Since there is limited scope of possible inputs, any token with length greater then 1 that begins
		if(isHex(token)){									//with a '0' will either be a float, hex or octal. Since isFloat and isOctal require looping through
			tok = "Hex";									//entire string isHex is called first before attempting isOctal and isFloat
			return tok;
		}
		else if(isOctal(token)){
		 tok = "Octal";
		 return tok;
		}
		else {
			tok = "float";
			return tok;
		}
	}
	else{
		if(isFloat(token)){							 		//Due to limited scope of possible inputs, if a token of length greater then 1 does not begin with a '0'
			tok = "float";									//it must either be a floating point or a decimal number.
			return tok;
		}
		else{
			tok = "Decimal";
			return tok;
		} 
	}
	
}

/* 
 * TKGetNextToken to takes in a character pointer pointing at a string or character array and returns a pointer to a new string continaing 
 *  the first token deliminated by white space, weather it be new line, line feed or white space in the string. */

char *TKGetNextToken( char * start ) {

		char *front, *token, *ptr;
		ptr = start; 
		
		int length = 1;									// Starts at 1 to allow for null byte when allocating memory down the line

		if(*ptr=='\n') return NULL;						// If the the character pointer given to the method is pointing at a new line character there are no
														// further tokens in line so return NULL
    	
    	while(isspace(*ptr )!=0){						// Loop to remove any leading white space before a token		
    		if(strlen(ptr)==1){							// Conditional to prevent attempting to access data not in line. If at last character and its white space
    			return NULL; 							// Return NULL as there are no more tokens in the the line
    		}
    		ptr++;										
    	}
    	
    	front = ptr;									// Once leading white space is removed, first non white space is the first character in the token

    	while(isspace(*ptr) == 0){						// Loop to determine length of the token to copy, keeps going while character is not a white space character
    		if(strlen(ptr) == 1 || strlen(ptr) > 50){   // Conditional to stop at end of the line or if improper string length is given to method to break.
    			length++;
    			break;
    		}
    		ptr++;
    		length++;
    	}
    	if(length<=50){									// Once length of token is determined and starting point, if the length is within scope (50 bytes) memory
    		
    		token = (char *)malloc(length);				// is dynamically allocated for it and a character pointed is pointed to the memory, the length includes
    													// the null byte for the string
    		return strncpy(token, front, length-1);		// the length-1 (the number of characters) of the token are copied from the line and store in memory token
    	}												// is pointing at and returns token 

    	return NULL;
}


/*
 * TKCreate method creates a TokenizerT object and returns a pointer to it. The TokenizerT struct is a Linked List that contains all the valid and 
 * invalid tokens along with their types each in a node. The method reads a file, takes in and stores each line and calls getNextToken 
 * storing each generated token in node along with its type. The TokenizerT struct is defined by a pointer to the head node and the tail node. The pointer 
 * to the tail node allows O(1) additions to the linked list at the back. And does not require dynamically reallocating memory to increase the size 
 * of the linked list. */

TokenizerT *TKCreate(char * ts){

	TokenizerT *TK = malloc(sizeof(TokenizerT));

	char *current_token, *ptr_line, *current_line;
	struct token_node *temp;

	current_line = (char *)malloc(50);						//Due to the size of a line in the file is limited to 50 bytes. Allocating 50 bytes and pointing  a
															//character pointer to it alows the use of a buffer to store the line to be operated on and then overwritten
	FILE *infile;
	infile = fopen(ts, "r");
	
	if(infile){

		while(fgets(current_line, 50, infile)!= NULL)		//Loop to continuously read the next line in the file and store it in the current_line "buffer" while
   		{													//There is still a line to take
   			ptr_line = current_line;
   			
			while((strlen(ptr_line))>0){					// Loop to continue find and storing tokens while the line still have parts left to be checked. 
															// Ptr_line character pointer keeps track of where in the line the program is currently at
    			while(isspace(*ptr_line )!=0){			
    				if(strlen(ptr_line)==1) break; 			// Loop to ignore whitespace, while making sure to end the loop if at the last non nullbyte character in the line
    				ptr_line++;
    			}
	        
	        	current_token = TKGetNextToken(ptr_line);	// Calls next get token and points curren_token at the generated token.
				
				if(!current_token) break;					// Conditional to check if there is a token generated by TKGetNextToken if there is not, break and dont store
				if(strlen(current_token)>0){				// Conditional to make sure the token is not nullByte character 
					
					temp = (struct token_node *)malloc(sizeof(struct token_node));	// Allocates memory for the node to store the token and its type in
					
					temp->token = current_token;								
					
					temp->type = getType(current_token);	// Calls getType and points the node's type character and the string it returns
			
					temp->next = NULL;						// Since program is adding tokens in order, the node will not have a next until another another token is found
															
					if(TK->head==NULL) TK->head = temp;		// if, else if, and else conditionals to determine how to place the node, if there is no head tokenizer should
															// point head at the generated node, if there is a head and no tail head's .next pointer should point at the node
					else if(TK->tail==NULL){				// and tail pointer should point at the node, if there is already a head and tail simply point tails next to generate
						TK->head->next = temp;				// node and make make it the new tail
						TK->tail = temp;
					}
				
					else{
						TK->tail->next = temp;
						TK->tail = temp;
					}

				}

				ptr_line += strlen(current_token);			//Update position of pointer of the current line to past the current token generated
			}
		}

		fclose(infile);	
		return TK;
	}
	
	else return NULL;										// If unable to open file TokenizerT returns NULL

}

/*
 * TKDestory simply frees any dynamically allocated memory allocated in TKCreate. Each Node contents are free (tokens), along with 
 * the pointers in the node, once all nodes and its contents are free, the TokenizerT struct is freed as well */

void TKDestroy( TokenizerT * tk ) {

		struct token_node *ptr, *temp;
		ptr = tk->head;
		
		while(ptr!=NULL){									// Loops through Linked List start at head and frees all dynamically stored memory
			
			temp = ptr->next;								// Since the type are literal strings and not dynamically allocated they are not freed
			free(ptr->token);								// For each node the token in it is freed
			free(ptr);										// and then the node itself is freed
			ptr=temp;
		} 

		free(tk);											// Once all the nodes and there contents are freed the Tokenizer struct itself is freed
} 

/* 
 * TKPrint takes in a pointer to a Tokenizer object. Using the data in the tokenizer object, It thenloops through the Linked List store in the 
 * Tokenizer object and depending on the token  writes its type and the actual token to the a results file, and if the token is of type "error" which is an 
 * invalid token it prints it to a file called error.msg in its hexdecimal form.  */

void TKPrint(TokenizerT *tk){
		FILE *result, *error;

		result = fopen("result", "w");						// Creates a file called results to write to, if it already exists it disgards its contents
		error = fopen("error.msg", "w");					// Creates a file called error.msg to write to, if it already exists it disgards its contents

		struct token_node * ptr;
		ptr = tk->head;
		while(ptr!=NULL){
			if(*(ptr->type) == 'e'){
				fprintf(error, "[OX%x]\n", *(ptr->token));	// Castes the token string as its hexadecimal form and prints it in form [Ox_]
			}
			else{
				fprintf(result, "%s %s\n", ptr->type, ptr->token); // If token type is not error prints the type followed by a space the token string followed by new line
			}
			ptr=ptr->next;
		}
		fclose(result);
		fclose(error);


	}



