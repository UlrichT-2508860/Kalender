#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"



/**
* @brief	A safe version of malloc. It will exit the program when an allocation error has occurred.
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
* @brief	A safe version of fgets. It will check your input constantly and give you retries when your string input was invalid.
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
		printf(p_message);
	}

	do
	{	//get user input
		fgets(p_destination, max_size, stream);
		while (p_destination[strlen(p_destination) - 1] != '\n')	//get rid of potential overflow
		{
			printf("Input was longer than %d characters! Please try again: ", max_size - 2);
			flush_keyboard_input();
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
* @brief	This function will flush the input stream.
* @param	void
* @return	void
*/
void flush_keyboard_input(void)
{
	char c;
	//todo: change this function so that it wipes out everything in the input-buffer and does not wait until something is typed.
	do
	{
		c = getchar();
		//} while (c != '\n');
	} while ((c != '\n') && (c != EOF));

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
	char c = getchar();	//getchar always returns a character or a \n and leaves a \n in the stdin-buffer.
	if (c != '\n')
	{	//clean up all residues in stdin.
		flush_keyboard_input();
	}
	return c;
}



/**
* @brief	This function will split a string with a given delimiter, it also works
*			if the token is empty
* @param	str (char*) the address of a given string
* @param	delimiter (const char*) a pointer to a constant delimiter
* @return	char* a pointer to the token
*/
char* my_strtok(char* str, const char* delimiter)
{
	char* start;
	static char* end;

	if (str == NULL)
	{
		start = end;
	}
	else
	{
		start = str;
	}

	end = strchr(start, *delimiter);
	if (end == NULL)
	{
		//no more delimters found, check to get rid of last '\n'
		end = strchr(start, '\n');
	}
	*end = '\0';
	end++;


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
	for (int i = 0; i < strlen(string); i++)
	{
		string[i] = tolower(string[i]);
	}
}



/**
* @brief	This function asks the user for confirmation. The user either inputs 'y' to confirm, or 'n' to cancel.
* @param	p_message (char*) An optional message to print at the start
* @return	0 if 'n' is entered, 1 if 'y' is entered
*/
int get_user_confirmation(char* p_message)
{

	if (p_message != NULL)
	{
		printf(p_message);
	}
	else
	{
		printf("Type y/n: ");
	}

	while(1)
	{	//Do always the following until a 'y' or 'n' character has been typed.
		char c = tolower(getchar_s());
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
