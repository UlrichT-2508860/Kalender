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
		exit(EXIT_FAILURE);
	}

	return p_malloc_location;
}

/**
* @brief	This function will flush the input stream.
* @param	void
* @return	void
*/
void flush_keyboard_input(void)
{
	char c;
	do
	{
		c = getchar();
		//} while (c != '\n');
	} while ((c != '\n') && (c != EOF));

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




