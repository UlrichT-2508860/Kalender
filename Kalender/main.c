#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Kalender.h"

/**
* @brief	This function should clear the terminal
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
 * @brief   This function prints the main menu. The user also inputs the option.
 * @param   void
 * @return  option (int) The returned option the user gave as input
 */
static int write_menu_and_get_option(void)
{

	printf("What would you like to do?\n"
		"[0] Exit Program\n"
		"[1] Add singular appointment\n"
		"[2] Remove appointments in a range\n"
		"[3] Empty Calendar\n"
		"[4] Display complete Calendar\n"
		"[5] Write out appointments in a range\n"
		"[6] Write out appointments based on textual matching\n"
		"[7] Import Calendar\n"
		"[8] Export Calendar\n");

	int choice;
	if (scanf("%d", &choice) != 1)
	{	//scanf went wrong, return illegal value
		choice = -1;
	}
	//flush all remaining characters for next input
	char c;
	do
	{
		c = getchar();
	} while (c != '\n');

	//while (c != '\n' && c != EOF)

	//c = getchar();
	//while (c != '\n' && c != EOF) 
	//{
	//	c = getchar();
	//}


	//while ((c = getchar()) != '\n' && c != EOF);
	//getchar();
	
	return choice;
}

/**
* @brief	This function processes the given menu option.
* @param	p_root (st_root*) The address of the root struct
* @param	Choice (int) The choice the user gave as input
* @return	Processed choice (int) The choice input that has been processed  
*/
int process_menu_option(st_root* p_root, int choice)
{
	switch (choice)
	{
	case EXIT_PROGRAM:

		printf("Exiting...\n");

		return 0;

	case CREATE_APPOINTMENT:

		add_appointment_manually(p_root);
		break;

	case REMOVE_APPOINTMENT_RANGE:

		remove_appointments_in_range(p_root);

		break;

	case EMPTY_CALENDAR:

		remove_tree(p_root, 1);
		
		break;

	case DISPLAY_CALENDAR:

		print_appointments_from_tree(p_root);

		break;

	case DISPLAY_CALENDAR_RANGE:

		print_appointments_in_range(p_root);
		break;

	case WRITE_CALENDAR_MATCH:

		print_appointments_with_match(p_root);

		break;

	case IMPORT_CALENDAR:

		//import_calendar_file(p_root, "C:\\Users\\ulric\\OneDrive\\Dokumente\\Ulrich Tuts\\Universiteit\\2025 - 2026\\Problem Solving\\Kwartiel 2\\Individueel project\\Kalender\\Kalender_data.txt");
		import_calendar_file(p_root, "C:\\Program Files Ulrich\\Kalender_data.txt");
		//import_calendar_file(p_root, NULL);
		break;

	case EXPORT_CALENDAR:
		export_calendar_file(p_root, "C:\\Program Files Ulrich\\new.txt");
		//export_calendar_file(p_root, "new.txt");
		break;

	default:
		printf("Invalid input! Expecting 0-8!\n");

	}

	return choice;

}



int main(void)
{
	printf("Welcome back user!\n");
	int processed_choice;

	//char* my_string = malloc(1000);
	//strcpy(my_string, "kiss my ass!");

	st_root root;
	init_root(&root);


	
	do
	{


		int choice = write_menu_and_get_option();

		processed_choice = process_menu_option(&root, choice);


		//wait for user input to return to menu
		if (processed_choice != EXIT_PROGRAM)
		{
			printf("Press ENTER to return to menu.\n");
			//getchar();
			//flush all remaining characters for next input
			char c;
			do
			{
				c = getchar();
			} while (c != '\n');

			clearscreen();
		}


	} while (processed_choice != 0);
		


	return 0;

}

