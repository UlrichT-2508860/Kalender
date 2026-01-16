#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "Kalender.h"
#include "kalender_user.h"




/**
 * @brief   This function prints the main menu. The user also inputs the option.
 * @param   void
 * @return  option (int) The returned option the user gave as input
 */
static int write_menu_and_get_option(void)
{

	printf("What would you like to do?\n"
		"----------------------------------------------------\n"
		"[0] Exit Program\n"
		"[1] Add appointment\n"
		"[2] Remove appointments in a range\n"
		"[3] Empty Calendar\n"
		"[4] Display complete Calendar\n"
		"[5] Display appointments in a range\n"
		"[6] Display appointments based on textual matching\n"
		"[7] Import Calendar\n"
		"[8] Export Calendar\n"
		"----------------------------------------------------\n"
		"Enter option and press ENTER to confirm: ");

	int choice;
	if (scanf("%d", &choice) != 1)
	{	//scanf went wrong, return illegal value
		choice = -1;
	}
	flush_keyboard_input();	//flush remaining keyboard input (including \n).
	
	return choice;
}

/**
* @brief	This function processes the given menu option.
* @param	p_root (st_root*) The address of the root struct
* @param	Choice (int) The choice the user gave as input
* @return	Processed choice (int) The choice input that has been processed  
*/
static int process_menu_option(st_root* p_root, int choice)
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
		remove_appointments_in_range_or_all(p_root, 0, 1);
		break;

	case EMPTY_CALENDAR:
		//If present, first ask for user confirmation to delete current calendar-tree.
		if (p_root->pl_year != NULL)
		{
			int del_choice = user_request_confirmation("WARNING: You currently have an active calendar! Continuing will delete your current calendar.\nDo you wish to continue? (y/n) : ");
			if (del_choice == 0)
			{
				break;
			}
		}
		remove_appointments_in_range_or_all(p_root, 1, 1);
		break;

	case DISPLAY_CALENDAR:
		print_appointments_in_range_or_all(p_root, 1);
		break;

	case DISPLAY_CALENDAR_RANGE:
		print_appointments_in_range_or_all(p_root, 0);
		break;

	case WRITE_CALENDAR_MATCH:

		print_appointments_with_match(p_root);

		break;

	case IMPORT_CALENDAR:

		//import_calendar_file(p_root, "C:\\Users\\ulric\\OneDrive\\Dokumente\\Ulrich Tuts\\Universiteit\\2025 - 2026\\Problem Solving\\Kwartiel 2\\Individueel project\\Kalender\\Kalender\\Kalender_data.txt");
		//TODO: to include the data file in the project folder
		import_calendar_file(p_root, "C:\\Program Files Ulrich\\Kalender_data.txt");
		//import_calendar_file(p_root, NULL);
		break;

	case EXPORT_CALENDAR:
		//export_calendar_file(p_root, "C:\\Users\\ulric\\OneDrive\\Dokumente\\Ulrich Tuts\\Universiteit\\2025 - 2026\\Problem Solving\\Kwartiel 2\\Individueel project\\Kalender\\Kalender\\lil_cuh.txt");
		//export_calendar_file(p_root, "C:\\Program Files Ulrich\\new.txt");
		export_calendar_file(p_root, NULL);
		break;

	default:
		printf("Invalid input! Expecting 0-8!\n");

	}

	return choice;

}



int main(void)
{
	st_root root;
	int choice;
	int processed_choice;

//#ifdef __linux__
	setvbuf(stdout, NULL, _IONBF, 0);	//This way Linux will not buffer the stdout, which will print my messages.
										//Source: https://stackoverflow.com/questions/24095701/how-does-work-setvbuf-in-c
//#endif

	clearscreen();		//Start with a clear screen

	printf("Welcome back user!\n");

	init_root(&root);	//Always init the calendar-root when the program starts

	do
	{
		//Print the menu and wait for a user's choice..
		choice = write_menu_and_get_option();
		
		//Execute the user's choice..
		processed_choice = process_menu_option(&root, choice);

		//wait for user input to return to menu
		if (processed_choice != EXIT_PROGRAM)
		{
			printf("Press ENTER to return to menu.\n");

			//flush all remaining characters for next input
			user_wait_for_Enter_Press();	

			// repeat menu but first clear screen.
			clearscreen();
		}
	} while (processed_choice != EXIT_PROGRAM);
		
	return 0;

}

