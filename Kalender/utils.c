// Student: Ulrich Tuts
// Nummer: 2508860
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"

/**
* @brief	This function should clear the terminal.
* @param	void
* @return	void
*/
void clearscreen(void)
{	//with the help of ANSI escape codes.
	//source: https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
	printf("\033[2J\033[H"); //NOTE: this doesn't really "clear" the terminal put prints enough enters so the terminal is empty
	//you can still scroll up to see the history of what went down.
//fflush(stdout);

//with the help of system commands, these vary from OS to OS though
//source: https://www.designgurus.io/answers/detail/how-to-clear-in-c-programming
#ifdef _WIN32
	system("cls");
#else 
	system("clear"); //for linux
#endif
}


/**
* @brief	A safe version of malloc. ATTENTION: It will exit the program when an allocation error has occurred.
* @param	size (size_t) The size of space you want to preserve for the allocation process.
* @return	p_malloc_location (void*) The address of where the preserved memory is located.
*/
void* malloc_s(size_t size)
{
	void* p_malloc_location = malloc(size);
	if (p_malloc_location == NULL)
	{
		printf("Malloc allocation error has occurred!\nAborting program...");
		exit(EXIT_FAILURE);	//source: https://www.geeksforgeeks.org/c/c-exit-abort-and-assert-functions/
	}

	return p_malloc_location;
}


/**
* @brief	This function will flush the keyboard input stream. 
*			If the keyboard stream is already empty: It will wait for a user entry (character + enter) (not so nice?)
*			If the keyboard stream not empty: It will get the characters until it has found an enter (\n) character. 
*			We presume the \n character as last character that could be present?
* @param	void
* @return	void
*/
void flush_keyboard_input(void)
{
	char c;
	//todo: perhaps change this function so that it wipes out everything in the input-buffer and does not wait until something is typed.
	do
	{
		c = (char) getchar();
	} while ((c != '\n') && (c != EOF));

}


/**
* @brief	This function will wait until the user has pressed the ENTER button.
*			If the keyboard stream is already empty: It will wait for a user entry (character + enter)
*			If the keyboard stream not empty: It will get the characters until it has found an enter (\n) character.
* @param	void
* @return	void
*/
void user_wait_for_Enter_Press(void)
{
	//same as flush_keyboard_input()??
	flush_keyboard_input();
}


/**
* @brief	A safe version of getchar. This function will clear the keyboard input stream after the character has been entered. 
*			I noted that when a '\n' has been given, there will be nothing in the input stream. So there is no reason to flush in that case.
*			This function exits with the input stream cleared.
* @param	void
* @return	c (char) The character entered.
*/
char getchar_s(void)
{
	char c = (char) getchar();	//getchar always returns a character or a \n and leaves a \n in the stdin-buffer.
	if (c != '\n')
	{	//clean up all residues in stdin.
		flush_keyboard_input();	//flush remaining keyboard input (including \n).
	}
	return c;
}



/**
* @brief	This function will split a string with a given delimiter-character, it also works
*			if the token is empty.
*			It is based on the same principles of the original sttrtok:
*			 - ptr str must contain a non-NULL value to start the search over str for finding the first token.
*			 - during all next calls, ptr str must be set to NULL so that the function knows that it must use its previously saved internal static variable end to start the search for the next token.
* @param	str (char*) the address of a given string when starting the search, if NULL it will continue with the previous saved end value.
* @param	delimiter (const char*) a pointer to a constant delimiter-character
* @return	char* a pointer to the found token (NULL if no more tokens found.
*/
char* my_strtok(char* str, const char* delimiter)
{
	char* start;
	static char* end = NULL;	//GLOBAL HIDDEN VARIABLE 

	if (str == NULL)
	{	//If strtok needs to continue on used string, check global variable 'end'.
		if (end == NULL)
		{
			return NULL;	//if end-ptr was never started, return NULL as safety.
		}
		start = end;	
	}
	else
	{	//If strtok is (re)started, set start-ptr to incoming string.
		start = str;
	}

	//Search for the delimiter-character starting from address in start.
	end = strchr(start, *delimiter);
	if (end == NULL)
	{
		//no more delimters found, check to get rid of last '\n'
		end = strchr(start, '\n');
	}
	if (end != NULL)
	{
		*end = '\0';	//terminate found token, by overwriting it's delimiter with a \0.
		end++;			//increase ptr just behind delimiter to allow future splits.
	}

	//increment for next member
	return start;

}

/**
* @brief	This function transforms a given string into its lowercase version. This will be used for the matching string function
* @param	string (char*) The address of a string
* @return	void
*/
void string_to_lower(char* string)
{
	for (size_t i = 0; i < strlen(string); i++)
	{
		string[i] = (char) tolower(string[i]);
	}
}


/**
* @brief	A safer version of fgets. It will check your input constantly for string size overflow and give you retries when your string input was invalid.
* @param	p_destination (char*) The address of the destination.
* @param	max_size (int) The max size your input can hold.
* @param	stream (FILE*) The stream you want to use (like stdin).
* @param	mandatory_input (int) If 0, this function can accept empty input. If NOT 0, it prompts you to try again.
* @param	p_message (char*) An optional message to print at the start.
*/
void fgets_s(char* p_destination, int max_size, FILE* stream, int mandatory_input, char* p_message)
{
	int str_input_is_ok = 0;	//variable to check if input is valid

	if (p_message != NULL)
	{	//print a message (if not NULL)
		//printf(p_message);
		printf("%s", p_message);	//to avoid Linux warning: "format string is not a string literal"
	}

	do
	{	//get user input
		fgets(p_destination, max_size, stream);
		while (p_destination[strlen(p_destination) - 1] != '\n')	//get rid of potential overflow
		{
			printf("Input was longer than %d characters! Please try again: ", max_size - 2);
			flush_keyboard_input();	//flush remaining keyboard input (including \n).
			fgets(p_destination, max_size, stream);
		}
		p_destination[strlen(p_destination) - 1] = '\0';	//get rid of '\n'

		if ((mandatory_input != 0) &&	//in case of empty input while it was mandatory
			strlen(p_destination) == 0)
		{
			printf("Input was empty while it was mandatory! Please try again: ");
		}
		else
		{
			str_input_is_ok = 1;
		}
	} while (str_input_is_ok == 0);
}

/**
* @brief	Requests the user to enter a string via the keyboard. It will check your input constantly for string size overflow and give you retries when your string input was invalid.
* @param	p_destination (char*) The address of the destination.
* @param	max_size (int) The max size your input can hold.
* @param	mandatory_input (int) If 0, this function can accept empty input. If NOT 0, it prompts you to try again.
* @param	p_message (char*) An optional message to print at the start.
*/
void user_request_string(char* p_destination, int max_size, int mandatory_input, char* p_message)
{
	fgets_s(p_destination, max_size, stdin, mandatory_input, p_message);
}



/**
* @brief	This function asks the user for confirmation. The user either inputs 'y' to confirm, or 'n' to cancel.
* @param	p_message (char*) An optional message to print at the start
* @return	0 if 'n' is entered, 1 if 'y' is entered
*/
int user_request_confirmation(char* p_message)
{
	if (p_message != NULL)
	{
		//printf(p_message);
		printf("%s", p_message);	//to avoid Linux warning: "format string is not a string literal"
	}
	else
	{
		printf("Type y/n: ");
	}

	while(1)
	{	//Do always the following until a 'y' or 'n' character has been typed.
		char c = (char) tolower(getchar_s());		//ask for a character + ENTER and then lower the character.
		if (c == 'y')
		{
			printf("You decided to continue.\n");
			return 1;
		}
		else if (c == 'n')
		{
			printf("You decided to cancel...\n");
			return 0;
		}
		else
		{
			printf("Invalid input! Please try again (y/n) :");
		}
	}
}
