/*
 * CS211 CompArch
 * Liu Liu
 * main.c
 */

#include "tokenizer.h"
/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the file name.
 */

int main(int argc, char ** argv){

	if(argc!=2) printf("Too many arguments, will attempt to use first input to run\n");				//Checks if extra input if there is warns user and still attempts to run

	TokenizerT *tk = (TokenizerT *)TKCreate(argv[1]);					//Creates a tokenizerT pointer and points it to the struct pointer returned by TKCreate

	if(tk->head==NULL){													// If TKCreate returns a Null Pointer notifies user and ends program
	printf("Unable to tokenize");
	return 0;
	}							

	TKPrint(tk);														// Prints the tokens and their types to corresponding files
	TKDestroy(tk);														// Once data is used and operations over frees dynamically allocated memory

	return 0;
}	

