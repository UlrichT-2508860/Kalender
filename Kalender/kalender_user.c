// Student: Ulrich Tuts
// Nummer: 2058860
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "Kalender.h"
#include "kalender_user.h"

//----------------------------------------------------------------
//- U S E R   R E L A T E D   C A L E N D A R  F U N C T I O N S -
//----------------------------------------------------------------


/**
* @brief	This function requests the user to input a time and update a given time struct
* @param	p_time (st_time*) The address to the time struct
* @param	p_message (char*) A pointer to a string to print if it was given
* @return	void
*/
void user_request_time(st_time* p_time, char* p_message)
{
	int scanf_result;
	//print a message (if not NULL) and get the time.
	if (p_message != NULL)
	{
		//printf(p_message);
		printf("%s", p_message);	//to avoid Linux warning: "format string is not a string literal"
	}
	scanf_result = scanf("%d:%d", &p_time->hour, &p_time->minute);
	flush_keyboard_input();	//flush remaining keyboard input (including \n).

	//check for invalid input
	while ((is_time_valid(p_time) != 0) ||
		(scanf_result != 2))
	{
		printf("INVALID TIME. USE THE GIVEN FORMAT (HH:MM): ");
		scanf_result = scanf("%d:%d", &p_time->hour, &p_time->minute);
		flush_keyboard_input();	//flush remaining keyboard input (including \n).
	}
}


/**
* @brief	This function requests two times from the user that respresent a range. It will also check if the input is correct and consistent
* @param	p_start_time (st_date*) The address of the start time struct
* @param	p_end_time (st_date*) The address of the end time struct
* @return	void
*
* @note		- errors if the entered time is in an incorrect format (HH/MM). The user needs to retry the entry.
*			- errors if the end-time is on or before the start-time. The user needs to retry the entry of both times.
*/
void user_request_time_range(st_time* p_start_time, st_time* p_end_time)
{
	int times_valid = 0;
	do
	{
		//get the start-time
		user_request_time(p_start_time, "Give the specific time when your appointment starts (HH:MM) : ");
		//get the end-time
		user_request_time(p_end_time, "Give the specific time when your appointment ends (HH:MM) : ");

		//Check if end-time is earlier than start-time.
		if (is_end_time_valid_to_start_time(p_end_time, p_start_time) != 0)
		{
			//times are NOT OK, repeat while loop until correct times filled in.
			printf("END-time OCCURS EARLIER THAN START-time!\n");
			printf("Please try again (press ENTER).");
			user_wait_for_Enter_Press();	//wait for enter and flush garbage input
		}
		else
		{
			//times are OK, break off while loop and continue
			times_valid = 1;
		}
	} while (times_valid == 0);

	return;
}


/**
* @brief	This function requests the user to input a date and update a given date struct
* @param	p_date (st_date*) The address to the date struct
* @param	p_message (char*) A pointer to a string to print if it was given
* @return	void
*/
void user_request_date(st_date* p_date, char* p_message)
{
	int scanf_result;
	if (p_message != NULL)
	{	//print a message (if not NULL)
		//printf(p_message);
		printf("%s", p_message);	//to avoid Linux warning: "format string is not a string literal"
	}
	scanf_result = scanf("%d/%d/%d", &p_date->year, &p_date->month, &p_date->day);	//get the date
	flush_keyboard_input();	//flush remaining keyboard input (including \n).

	//check for invalid input
	while ((is_date_valid(p_date) != 0) ||
		(scanf_result != 3))
	{
		printf("INVALID DATE! TRY AGAIN AND USE THE GIVEN FORMAT: (YYYY/MM/DD): ");
		scanf_result = scanf("%d/%d/%d", &p_date->year, &p_date->month, &p_date->day);
		flush_keyboard_input();	//flush remaining keyboard input (including \n).
	}
}



/**
* @brief	This function requests two dates from the user that respresent a range. It will also check if the input is correct
* @param	p_start_date (st_date*) The address of the start date struct
* @param	p_end_date (st_date*) The address of the end date struct
* @param	p_start_date_in_days (int*) The address of the start date converted to days number
* @param	p_start_year_month_in_days (int*) The address of the start date (year and month only) converted to days number
* @param	p_end_date_in_days (int*) The address of the end date converted to days number
* @param	p_end_year_month_in_days (int*) The address of the end date (year and month only) converted to days number
* @return	void
*/
void user_request_date_range(st_date* p_start_date,
	st_date* p_end_date,
	int* p_start_date_in_days,
	int* p_start_year_month_in_days,
	int* p_end_date_in_days,
	int* p_end_year_month_in_days)
{
	int dates_valid = 0;
	do
	{
		//get the start-date
		user_request_date(p_start_date, "Please give the start-date (YYYY/MM/DD): ");
		//get the end-date
		user_request_date(p_end_date, "Please give the end-date (YYYY/MM/DD): ");

		*p_start_date_in_days = date_to_int(p_start_date->year, p_start_date->month, p_start_date->day);
		*p_end_date_in_days = date_to_int(p_end_date->year, p_end_date->month, p_end_date->day);
		//Check if end-date is earlier than start-date.
		if (*p_end_date_in_days < *p_start_date_in_days)
		{
			//dates are NOT OK, repeat while loop until correct dates filled in.
			printf("END-DATE OCCURS EARLIER THAN START-DATE!\n");
			printf("Please try again (press ENTER).");
			user_wait_for_Enter_Press();	//wait for enter and flush garbage input
		}
		else
		{
			//dates are OK, break off while loop and continue
			*p_start_year_month_in_days = date_to_int(p_start_date->year, p_start_date->month, 0);
			*p_end_year_month_in_days = date_to_int(p_end_date->year, p_end_date->month, 0);
			dates_valid = 1;
		}
	} while (dates_valid == 0);

	return;
}

/**
* @brief	Adds an appointment to the calendar (by user input)
* @param	p_root (st_root*) The address of the root struct
* @return	void
*/
void add_appointment_manually(st_root* p_root)
{

	st_appointment new_appointment;

	char title[MAX_TITLE_LENGTH + 1]; //one extra byte for receiving the \n of the fgets
	char description[MAX_DESCRIPTION_LENGTH + 1];
	char location_description[MAX_LOCATION_LENGTH + 1];

	//get title of appointment (MANDATORY)
	user_request_string(title, sizeof(title), 1, "Give the title of your appointment: ");
	new_appointment.p_title = malloc_s(strlen(title) + 1); // malloc + 1 for nullbyte
	strcpy(new_appointment.p_title, title);

	//get optional description
	user_request_string(description, sizeof(description), 0, "Give an optional description of your appointment: ");
	new_appointment.p_description = malloc_s(strlen(description) + 1); // malloc + 1 for nullbyte
	strcpy(new_appointment.p_description, description);

	//get optional location description
	user_request_string(location_description, sizeof(location_description), 0, "Give an optional location descrption of your appointment: ");
	new_appointment.p_location_description = malloc_s(strlen(location_description) + 1); // malloc + 1 for nullbyte
	strcpy(new_appointment.p_location_description, location_description);

	//get date
	user_request_date(&new_appointment.date, "Give the date of your appointment. (Format: YYYY/MM/DD) : ");

	//get specific time when appointment starts
	user_request_time_range(&new_appointment.time_start, &new_appointment.time_end);


	//generate unique id for this appointment
	new_appointment.id = generate_id();

	add_appointment_to_tree(p_root, &new_appointment);
	printf("Appointment has been added to Calendar!\n");

}

/**
* @brief	This function will import a calendar by using a txt file, reading it, and getting every member
*			of an appointment struct. This function will stop and print an error message when something invalid was read and it will delete
*			the root afterwards.
*
* 
* @param	p_root (st_root*) The address of the root struct
* @param	default_filename (char*) The default filename, could be NULL
* @return	void
*/
void import_calendar_file(st_root* p_root, char* default_filename)
{
	FILE* h_calendar_file;
	int import_result;
	char tmp_filename[512 + 2];	//to store eventually manually entered filepath + '\n' and nullbyte
	int line_counter;	

	//If present, first ask for user confirmation to delete current calendar-tree.
	if (p_root->pl_year != NULL)
	{
		int choice = user_request_confirmation("WARNING: You currently have an active calendar! Continuing will delete your current calendar.\nDo you wish to continue? (y/n) : ");
		if (choice == 0)
		{
			return;	//User cancelled the deletion, so exit import.
		}
	}

	//TODO: MAKE THIS A FUNCTION
	if (default_filename == NULL)
	{	//If no filename is given, request the filepath+filename from user:
		user_request_string(tmp_filename, sizeof(tmp_filename), 1, "Please give the path + filename to your .txt file: ");
		default_filename = tmp_filename;
	}
	else
	{	//We still offer the choice to override the default filename
		printf("Please give the path + filename or press ENTER to use the default calendar-file (%s) : ", default_filename);
		user_request_string(tmp_filename, sizeof(tmp_filename), 0, NULL);
		if (strlen(tmp_filename) != 0)
		{
			//user has specified a new filename, so use it.
			default_filename = tmp_filename;
		}
	}

	h_calendar_file = fopen(default_filename, "r");		//try to open file
	if (h_calendar_file == NULL)
	{	//in case file has not been found
		printf("Error with opening file: %s\n", default_filename);
		return;
	}

	import_result = read_calendar_from_file(p_root, h_calendar_file, &line_counter);

	if (import_result!= 0)
	{
		printf("Error has occurred at line %d during import!\nCancelling import...\n", line_counter);
		switch (import_result)
		{
		case IMPORT_ERROR_TITLE:
			printf("Title of entry was empty or was longer than %d characters!\n", MAX_TITLE_LENGTH - 1);
			break;
		case IMPORT_ERROR_DESCRIPTION:
			printf("Description was longer than %d characters!\n", MAX_DESCRIPTION_LENGTH - 1);
			break;
		case IMPORT_ERROR_LOCATION:
			printf("Location was longer than %d characters!\n", MAX_LOCATION_LENGTH - 1);
			break;
		case IMPORT_ERROR_DATE:
			printf("Date of entry was invalid!\n");
			break;
		case IMPORT_ERROR_START_TIME:
			printf("Start time of entry was invalid!\n");
			break;
		case IMPORT_ERROR_END_TIME:
			printf("End time of entry was invalid!\n");
			break;
		case IMPORT_ERROR_ID:
			printf("ID of entry was invalid!\n");
			break;
		default:
			printf("Something unexpected occurred!\n");
		}
		remove_appointments_in_range_or_all(p_root, 1, 1);	//remove partly imported tree 
	}
	else
	{
		printf("Import done!\n");
	}

	fclose(h_calendar_file);
}


/**
* @brief	This function will take the current calendar tree and export it to a file
* @param	p_root (st_root*) The address of the root struct
* @param	filename (char*) A pointer to the filename
* @return	void
*/
void export_calendar_file(st_root* p_root, char* default_filename)
{

	char tmp_filename[512 + 2];	//to store eventually manually entered filepath + '\n' and nullbyte

	st_year* p_year = p_root->pl_year;
	if (p_year == NULL)
	{
		printf("Tree is empty! Nothing to export!\n");
		return;
	}

	if (default_filename == NULL)
	{	//If no filename is given, request the MANDATORY filepath+filename from user:
		user_request_string(tmp_filename, sizeof(tmp_filename), 1, "Please give the path + filename: ");
		default_filename = tmp_filename;
	}
	else
	{	//We still offer the choice to override the default filename
		printf("Please give the path + filename or press ENTER to use the default calendar-file (%s) : ", default_filename);
		user_request_string(tmp_filename, sizeof(tmp_filename), 0, NULL);	//we print no message since we have printed it before :-)
		if (strlen(tmp_filename) != 0)
		{
			//user has specified a new filename, so use it.
			default_filename = tmp_filename;
		}
	}

	//Create file in writing mode.
	FILE* h_calendar_file = fopen(default_filename, "w");
	if (h_calendar_file == NULL)
	{	//in case file creation has failed.
		printf("Error with creating and opening file: %s\n", default_filename);
		return;
	}

	write_calendar_to_file(p_root, h_calendar_file);

	fclose(h_calendar_file);
	printf("Export completed!\n");

}