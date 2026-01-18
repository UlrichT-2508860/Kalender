// Student: Ulrich Tuts
// Nummer: 2508860
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"
#include "utils_date_time.h"
#include "Kalender.h"
#include "kalender_user.h"



/**
* @brief	This function will generate an ID based on the current time
* @param	void
* @return	the id (int) 
*/
int generate_id(void)
{
	//source: https://www.geeksforgeeks.org/c/time-h-header-file-in-c-with-examples/

	time_t current_time = time(NULL); //get current time
	//return (int)(current_time % 1000000000);
	return (unsigned int) current_time;

}






//----------------------------------------------------------------
//--          L I N K E D   L I S T   F U N C T I O N S         --
//----------------------------------------------------------------

/**
* @brief	This function initializes the root struct
* @param	p_root (st_root*) The address of the root struct
* @return	void
*/
void init_root(st_root* p_root)
{
	p_root->pl_year = NULL;
}



/**
* @brief	This function will search the tree for a given year and returns the address of that struct.
*			If the year is not present, it will be created.
* @param	p_this_root (st_root*) The address of the root
* @param	year (int) The year to be searched
* @return	Address to new year struct (st_year*)
*/
st_year* get_or_create_year(st_root* p_this_root, int year)
{
	st_year* tmp_year = p_this_root->pl_year;
	st_year* previous_tmp_year = NULL;
	st_year* tmp_new_year = NULL;

	//if nothing is present in the root, create the first year struct and return directly
	if (tmp_year == NULL)
	{
		tmp_year = malloc_s(sizeof(st_year));
		tmp_year->year = year;
		tmp_year->pl_next_year = NULL;
		tmp_year->pl_month = NULL;

		//update and return tree
		p_this_root->pl_year = tmp_year;
		return tmp_year;
	}

	//Search for a match in year
	while ((tmp_year->year < year) &&
			(tmp_year->pl_next_year != NULL))
	{
		previous_tmp_year = tmp_year;
		tmp_year = tmp_year->pl_next_year;
	}



	//If we come here, either the year is found or we are at the end of the linked list
	if (tmp_year->year != year)
	{
		//year is not found create new year struct in memory:
		tmp_new_year = malloc_s(sizeof(st_year));
		tmp_new_year->year = year;
		tmp_new_year->pl_month = NULL;
		
		if ((previous_tmp_year == NULL) && (tmp_year->year > year)) 
		{	// The wanted year needs to be inserted in the beginning
			tmp_new_year->pl_next_year = tmp_year;
			p_this_root->pl_year = tmp_new_year;
		}
		else
		{
			if (tmp_year->year < year)
			{	// The wanted year is higher than the current year -> add new entry
				tmp_year->pl_next_year = tmp_new_year;
				tmp_new_year->pl_next_year = NULL;
			}
			else
			{	// The wanted year is lower than the current year. -> insert new entry (rewiring).
				tmp_new_year->pl_next_year = tmp_year;
				previous_tmp_year->pl_next_year = tmp_new_year;
			}
		}
		//return new created year
		return tmp_new_year;
	}
	//year already present, return pointer 
	return tmp_year;
}


/**
* @brief	This function will search in a year struct for a given month and returns the address of that struct.
*			If the month is not present, it will be created.
* @param	p_this_year (st_year*) The address of the current year struct
* @param	month (int) The month to be searched
* @return	Address to new month struct (st_month*)
*/
st_month* get_or_create_month(st_year* p_this_year, int month)
{
	st_month* tmp_month = p_this_year->pl_month;
	st_month* previous_tmp_month = NULL;
	st_month* tmp_new_month = NULL;

	//if nothing is present for this year, create the first month struct and return directly
	if (tmp_month == NULL)
	{
		tmp_month = malloc_s(sizeof(st_month));
		tmp_month->month = month;
		tmp_month->pl_next_month = NULL;
		tmp_month->pl_day= NULL;
		
		//update and return tree
		p_this_year->pl_month = tmp_month;
		return tmp_month;
	}


	// search for match in month
	while ((tmp_month->month < month) &&
		(tmp_month->pl_next_month != NULL))
	{
		previous_tmp_month = tmp_month;
		tmp_month = tmp_month->pl_next_month;
	}

	//check if match has been found
	if (tmp_month->month != month)
	{
		//month is not found, create a new month struct in memory
		tmp_new_month = malloc_s(sizeof(st_month));
		tmp_new_month->month = month;
		tmp_new_month->pl_day = NULL;
		
		if ((previous_tmp_month == NULL) && (tmp_month->month > month))
		{	//the wanted month needs to be inserted in the beginning
			tmp_new_month->pl_next_month = tmp_month;
			p_this_year->pl_month = tmp_new_month;
		}
		else
		{
			if (tmp_month->month < month)
			{	//the wanted month needs to be added
				tmp_new_month->pl_next_month = NULL;
				tmp_month->pl_next_month = tmp_new_month;
			}
			else
			{	//the wanted month needs to be inserted
				tmp_new_month->pl_next_month = tmp_month;
				previous_tmp_month->pl_next_month = tmp_new_month;
			}
		}
		//return new created month
		return tmp_new_month;
	}
	//month already present, return pointer 
	return tmp_month;
	
}

/**
* @brief	This function will search in a month struct for a given day and returns the address of that struct.
*			If the day is not present, it will be created.
* @param	p_this_month (st_month*) The address of the current month struct
* @param	day (int) The day to be searched
* @return	Address to new day struct (st_day*)
*/
st_day* get_or_create_day(st_month* p_this_month, int day)
{
	st_day* tmp_day = p_this_month->pl_day;
	st_day* previous_tmp_day = NULL;
	st_day* tmp_new_day = NULL;

	//if nothing is present for this month, create the first day struct and return directly
	if (tmp_day == NULL)
	{
		tmp_day = malloc_s(sizeof(st_day));
		tmp_day->day = day;
		tmp_day->pl_next_day = NULL;
		tmp_day->pl_appointment = NULL;

		//update and return tree
		p_this_month->pl_day = tmp_day;
		return tmp_day;
	}


	// search for match in day
	while ((tmp_day->day < day) &&
		(tmp_day->pl_next_day != NULL))
	{
		previous_tmp_day = tmp_day;
		tmp_day = tmp_day->pl_next_day;
	}

	//check if match has been found
	if (tmp_day->day != day)
	{
		//day is not found, create a new day struct in memory
		tmp_new_day = malloc_s(sizeof(st_day));
		tmp_new_day->day = day;
		tmp_new_day->pl_appointment = NULL;

		if ((previous_tmp_day == NULL) && (tmp_day->day > day))
		{	//the wanted day needs to be inserted in the beginning
			tmp_new_day->pl_next_day = tmp_day;
			p_this_month->pl_day = tmp_new_day;
		}
		else
		{
			if (tmp_day->day < day)
			{	//the wanted day needs to be added
				tmp_new_day->pl_next_day = NULL;
				tmp_day->pl_next_day = tmp_new_day;
			}
			else
			{	//the wanted day needs to be inserted
				tmp_new_day->pl_next_day = tmp_day;
				previous_tmp_day->pl_next_day = tmp_new_day;
			}
		}
		//return new created day
		return tmp_new_day;
	}
	//day already present, return pointer 
	return tmp_day;
}


/**
* @brief	This function will create dynamically a new appointment struct in a given days linked-list and sorts it based on the start time of the appointment.
*			The appointment will always be created (overlapping appointments are allowed(update the linked list ).
* @param	p_this_day (st_day*) The address of the current day struct
* @param	p_time_start (st_time*) The time when the appointment will start
* @return	Address to new appointment struct (st_appointment*)
*/
st_appointment* get_and_create_appointment(st_day* p_this_day, st_time* p_time_start)
{
	st_appointment* tmp_appointment = p_this_day->pl_appointment;
	st_appointment* previous_appointment = NULL;
	st_appointment* tmp_new_appointment = NULL;
	int time_start_in_minutes = p_time_start->hour * 60 + p_time_start->minute;

	//if nothing is present on this day, create the first appointment struct with the given time
	if (tmp_appointment == NULL)
	{
		tmp_appointment = malloc_s(sizeof(st_appointment));
		tmp_appointment->time_start.hour = p_time_start->hour;
		tmp_appointment->time_start.minute = p_time_start->minute;
		tmp_appointment->pl_next_appointment = NULL;

		//update and return tree
		p_this_day->pl_appointment = tmp_appointment;
		return tmp_appointment;
	}

	//search for the vincinity place in the list of appointments.
	while ((tmp_appointment->time_start.hour * 60 + tmp_appointment->time_start.minute <= time_start_in_minutes)
		&& (tmp_appointment->pl_next_appointment != NULL))
	{
		previous_appointment = tmp_appointment;
		tmp_appointment = tmp_appointment->pl_next_appointment;
	}
	//here tmp_appointment is the first appointment that occurs after or on the start-time OR it is the last appointment in the list (pl_next_appointment contains NULL).
	//here previous_appointment is the last appointment that occurs before the start-time OR it is NULL.
	int tmp_appointment_time_start_in_minutes = tmp_appointment->time_start.hour * 60 + tmp_appointment->time_start.minute;

	//appointment hour has not been found, create a new appointment struct in memory
	tmp_new_appointment = malloc_s(sizeof(st_appointment));
	tmp_new_appointment->time_start.hour = p_time_start->hour;
	tmp_new_appointment->time_start.minute = p_time_start->minute;

	// define where the appointment needs to be inserted.
	if ((previous_appointment == NULL) &&									//Are we at the first appointment AND...
		(tmp_appointment_time_start_in_minutes > time_start_in_minutes))	//...is our new start-time before this first appointment?
	{	//the appointment needs to be inserted in the beginning
		tmp_new_appointment->pl_next_appointment = tmp_appointment;
		p_this_day->pl_appointment = tmp_new_appointment;
	}
	else
	{
		if (tmp_appointment_time_start_in_minutes <= time_start_in_minutes)
		{	//the appointment needs to be added in the back
			tmp_new_appointment->pl_next_appointment = NULL;
			//tmp_new_appointment->pl_next_appointment = tmp_appointment->pl_next_appointment;
			tmp_appointment->pl_next_appointment = tmp_new_appointment;
		}
		else
		{	//the appointment needs to be inserted
			tmp_new_appointment->pl_next_appointment = tmp_appointment;
			previous_appointment->pl_next_appointment = tmp_new_appointment;
		}
	}
	return tmp_new_appointment;
}


/**
* @brief	This function will add a new appointment to the calendar tree
* @param	p_root (st_root*) The address of the root struct
* @param	p_new_appointment The address of the new appointment struct
* @return	void
*/
void add_appointment_to_tree(st_root* p_root, st_appointment* p_new_appointment)
{
	st_year* p_year;
	st_month* p_month;
	st_day* p_day;
	st_appointment* p_appointment;

	//Find or create the wanted year.
	p_year = get_or_create_year(p_root, p_new_appointment->date.year);
	
	//Find or create the wanted month.
	p_month = get_or_create_month(p_year, p_new_appointment->date.month);

	//Find or create the wanted day.
	p_day = get_or_create_day(p_month, p_new_appointment->date.day);

	//CREATE (not find or create) the wanted appointment.
	//HINT: TWO APPOINTMENTS AT THE SAME TIME ARE ALLOWED. new appointment will be placed behind the already existing appointment
	p_appointment = get_and_create_appointment(p_day, &p_new_appointment->time_start);

	//Fill in all the data of the appointment (but keep the link to the next appointment!).
	//copy the entire appointment data to its new destination struct in the tree.
	st_appointment* pl_backup_next_appointment = p_appointment->pl_next_appointment;
	memcpy(p_appointment, p_new_appointment, sizeof(st_appointment));
	p_appointment->pl_next_appointment = pl_backup_next_appointment;
}


/**
* @brief	This function prints every detail of a given detail
* @prama	p_appointment (st_appointment*) The address of an appointment struct
* @return	void
*/
void print_appointment_details(st_appointment* p_appointment)
{
	{
		printf("  Title: %s\n", p_appointment->p_title);
		printf("  ------");
		for (size_t i = 0; i <= strlen(p_appointment->p_title); i++)	//Fill up with '-' to the total length of the title
		{
			printf("-");
		}
		printf("\n   Description: %s\n", p_appointment->p_description);
		printf("   Location: %s\n", p_appointment->p_location_description);
		printf("   Start-time: %02d:%02d\n", p_appointment->time_start.hour, p_appointment->time_start.minute);
		printf("   End-time: %02d:%02d\n", p_appointment->time_end.hour, p_appointment->time_end.minute);
		printf("   Appointemnt ID: %d\n\n\n", p_appointment->id);
	}
}



/**
* @brief	This function will print all appointments that contains a (sub)string the user gives.
* @param	p_root (st_root*) The address of the root struct.
* @return	void
*/
void print_appointments_with_match(st_root* p_root)
{
	st_year* p_year = p_root->pl_year;

	//check if tree is empty
	if (p_year == NULL)
	{
		printf("Tree is empty! Nothing to display!\n");
		return;
	}

	//get match string
	char match_string[MAX_TITLE_LENGTH + 1];
	user_request_string(match_string, sizeof(match_string), 1, "Give the string you want to search for: ");
	string_to_lower(match_string); //lower the match string (case-insensitive)
	
	int found = 0;
	while (p_year != NULL)
	{
		st_month* p_month = p_year->pl_month;
		while (p_month != NULL)
		{
			st_day* p_day = p_month->pl_day;
			while (p_day != NULL)
			{
				st_appointment* p_appointment = p_day->pl_appointment;
				while (p_appointment != NULL)
				{
					//if match string is present, display appointment
					char lowered_title[MAX_TITLE_LENGTH];
					strcpy(lowered_title, p_appointment->p_title);
					string_to_lower(lowered_title);
					if (strstr(lowered_title, match_string))
					{
						printf("----------------------------------------\n");
						printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
						printf("----------------------------------------\n");
						print_appointment_details(p_appointment);
						found = 1;
					}
					p_appointment = p_appointment->pl_next_appointment;
				}
				p_day = p_day->pl_next_day;
			}
			p_month = p_month->pl_next_month;
		}
		p_year = p_year->pl_next_year;
	}

	if (found == 0)
	{
		printf("There were no appointments with this match string!\n");
	}
}




/**
* @brief	This function prints all appointments (from the tree) or all the appointments in a given range.
* @param	p_root		pointer to the tree 
* @param	print_all	0 = ask user for entering a date-range and print only those appointments meeting the range,
*						NOT 0 = print all appointments.
* @return	void
*/
void print_appointments_in_range_or_all(st_root* p_root, int print_all)
{
	st_date start_date;
	st_date end_date;
	int start_date_in_days=0;
	int start_year_month_in_days = 0;
	int end_date_in_days = 0;
	int end_year_month_in_days = 0;
	
	if (p_root->pl_year == NULL)
	{	//if tree is empty
		printf("Tree is empty! Nothing to print!\n");
		return;
	}

	//preset structures to 0, to avoid garbage in case of print_all
	memset(&start_date, 0, sizeof(start_date));	
	memset(&end_date, 0, sizeof(end_date));

	if (print_all == 0)
	{	//if not print all, ask for date range
		 user_request_date_range(	&start_date,
									&end_date,
									&start_date_in_days,
									&start_year_month_in_days,
									&end_date_in_days,
									&end_year_month_in_days);
	}

	//if print_all == 0 -> only print the appointments whose date are in range
	//if print_all != 0 -> print all the appointments in the tree

	st_year* p_year = p_root->pl_year;
	//set flag in case nothing was found
	int found = 0;

	//browse through years, months and days
	while (p_year != NULL)
	{
		if ((print_all != 0) ||															//if print all is wanted or..
			((start_date.year <= p_year->year) && (p_year->year <= end_date.year)))		//...if year in range.
		{
			st_month* p_month = p_year->pl_month;
			while (p_month != NULL)
			{
				int current_year_month_in_days = 0;
				if (print_all == 0)
				{
					current_year_month_in_days = date_to_int(p_year->year, p_month->month, 0);
				}
				if ((print_all != 0) ||																										//if print all is wanted or..
					((start_year_month_in_days <= current_year_month_in_days) && (current_year_month_in_days <= end_year_month_in_days)))	//...if year+month in range
				{
					st_day* p_day = p_month->pl_day;
					while (p_day != NULL)
					{
						int current_date_in_days = 0;
						if (print_all == 0)
						{
							current_date_in_days = current_year_month_in_days + p_day->day;
						}
						if ((print_all != 0) ||																				//if print all is wanted or..
							((start_date_in_days <= current_date_in_days) && (current_date_in_days <= end_date_in_days)))	//...if year+month+day in range
						{
							printf("----------------------------------------\n");
							printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
							printf("----------------------------------------\n");
							st_appointment* p_appointment = p_day->pl_appointment;
							while (p_appointment != NULL)
							{
								print_appointment_details(p_appointment);
								found = 1;
								p_appointment = p_appointment->pl_next_appointment;
							}
						}
						p_day = p_day->pl_next_day;	//goto next day in linked list
					}
				}
				p_month = p_month->pl_next_month; //goto next month in linked list
			}
		}
		p_year = p_year->pl_next_year; //goto next year in linked list
	}

	if ((found == 0) && (print_all == 0))
	{	//in case nothing was found while searching in a range
		printf("No appointments found in this range!\n");
	}
}



/**
* @brief	Helper function that reads the next string-member of the current line, checks for minimum and maximum stringlength, 
*			if all OK it will allocate memory (using malloc) and it will copy the string-member to its new destination.
*			the address of the nex string will be returned in the passed-in pointer p_dest
*
* @param	str (char*) the address of a given string when starting the search, if NULL it will continue with the previous saved end value.
* @param	p_dest (char**) Address of the pointer that will point to the new allocated string.
* @param	min_length (size_t) The minimum length of the member.
* @param	max_length (size_t) The max length of the member.
* @param	error_code (int) The given error code in case of an import error.
* @return	import_result (int)
*/
static int my_strtok_and_malloc_string(char* str, char** p_dest, size_t min_length, size_t max_length, int error_code)
{
	char* p_member = my_strtok(str, FILE_DELIMITER);	//read the next item from the line (seperated by delimiter).

	if ((strlen(p_member) < min_length) ||				//If item string length is too small or..
		(strlen(p_member) > max_length))				//too long:
	{
		return error_code;							//return error-code
	}

	*p_dest = malloc_s(strlen(p_member) + 1);	// malloc size of string + 1 for nullbyte
	strcpy(*p_dest, p_member);					//copy string into destination
	return IMPORT_OK;							//return OK.
}



/**
* @brief	This function will import a calendar by using a txt file, reading it, and getting every member
*			of an appointment struct. This function will stop when something invalid was read.
*
*			Appointment Record-structure in file:
*			- - - - - - - - - - - - - - - - - - -
*			The contents of one appointment is specified on one line. The different item of the appointment are
*			seperated via a ";" character. (This is the FILE_DELIMITER)
*			the item order is fixed as follows:
*
*				Title;Description;Location;Date;Time-start;Time-end;ID\n
*
*			So the contents of the file looks as follows:
*
*				Title;Description;Location;Date;Time-start;Time-end;ID\n
*				Title;Description;Location;Date;Time-start;Time-end;ID\n
*				Title;Description;Location;Date;Time-start;Time-end;ID\n
*				.
*				.
*
* @param	p_root (st_root*) The address of the root struct
* @param	h_calendar_file (FILE*) The handler to the opened file
* @param	line_counter (int*) Address of the line counter
* @return	import_result (int)
*/
int read_calendar_from_file(st_root* p_root, FILE* h_calendar_file, int* line_counter)
{
	int scanf_result;			//for checking scanf result
	int import_result = 0;		//for keeping errors during import
	st_appointment tmp_appointment;
	char s_line[MAX_TITLE_LENGTH + MAX_LOCATION_LENGTH + MAX_DESCRIPTION_LENGTH + DATE_STR_LENGTH + (TIME_STR_LENGTH * 2) + ID_STR_LENGTH + 10]; //use the size of the struct plus some extra characters for the separators.
	*line_counter = 1;

	//First delete current tree.
	remove_appointments_in_range_or_all(p_root, 1, 0);

	//get each line (entry) and convert to struct
	while (fgets(s_line, sizeof(s_line), h_calendar_file) != NULL)
	{

		//clear tmp struct
		memset(&tmp_appointment, 0, sizeof(tmp_appointment));

		// - - - - - - - - - - - - - - - -
		//split the line in struct members
		// - - - - - - - - - - - - - - - -
		char* p_member;
		//get title (first member)
		if ((import_result = my_strtok_and_malloc_string(s_line, &tmp_appointment.p_title, 1, MAX_TITLE_LENGTH, IMPORT_ERROR_TITLE)) != IMPORT_OK)
		{
			break;	//Cancel import-loop if error occured.
		}
	
		//get description (second member)
		if ((import_result = my_strtok_and_malloc_string(NULL, &tmp_appointment.p_description, 0, MAX_DESCRIPTION_LENGTH, IMPORT_ERROR_DESCRIPTION)) != IMPORT_OK)
		{
			break;	//Cancel import-loop if error occured.
		}

		//get location (third member)
		if ((import_result = my_strtok_and_malloc_string(NULL, &tmp_appointment.p_location_description, 0, MAX_LOCATION_LENGTH, IMPORT_ERROR_LOCATION)) != IMPORT_OK)
		{
			break;	//Cancel import-loop if error occured.
		}
		
		//get date (fourth member)
		//TODO: make this a function my_strtok_and_get_date ??
		p_member = my_strtok(NULL, FILE_DELIMITER);
		scanf_result = sscanf(p_member, "%d/%d/%d", &tmp_appointment.date.year, &tmp_appointment.date.month, &tmp_appointment.date.day);
		if ((scanf_result != 3) ||	//bad structure?
			(is_date_valid(&tmp_appointment.date) != 0))	//check for invalid data
		{
			import_result = IMPORT_ERROR_DATE;//Mark as error
			break;	//Cancel import-loop.
		}

		//get time start (fifth member)
		//TODO: make this a function my_strtok_and_get_time ??
		p_member = my_strtok(NULL, FILE_DELIMITER);
		scanf_result = sscanf(p_member, "%d:%d", &tmp_appointment.time_start.hour, &tmp_appointment.time_start.minute);
		if ((scanf_result != 2) ||	//bad structure?
			(is_time_valid(&tmp_appointment.time_start) != 0))	//invalid time??
		{
			import_result = IMPORT_ERROR_START_TIME;	//Mark as error
			break;	//Cancel import-loop.
		}

		//get time end (sixth member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
		scanf_result = sscanf(p_member, "%d:%d", &tmp_appointment.time_end.hour, &tmp_appointment.time_end.minute);
		if ((scanf_result != 2) ||	//bad structure?
			(is_time_valid(&tmp_appointment.time_end) != 0) ||
			(is_end_time_valid_to_start_time(&tmp_appointment.time_end, &tmp_appointment.time_start)))	//invalid time??
		{
			import_result = IMPORT_ERROR_END_TIME;	//Mark as error
			break;	//Cancel import-loop.
		}

		//get unique id (last member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
		scanf_result = sscanf(p_member, "%d", &tmp_appointment.id);
		if ((scanf_result != 1)) 	//bad structure?
		{
			import_result = IMPORT_ERROR_ID;	//Mark as error
			break;	//Cancel import-loop.
		}

		// - - - - - - - - - - - - - - - - - - - 
		// Save Temp-struct to the tree structure
		// - - - - - - - - - - - - - - - - - - - 
		add_appointment_to_tree(p_root, &tmp_appointment);

		//Increment line counter and repeat loop.
		(*line_counter)++;

	}

	return import_result;
}


/**
* @brief	This function will take the current calendar tree and write it to an opened file.
* @param	p_root (st_root*) The address of the root struct
* @param	h_calendar_file (FILE*) A file handler
* @return	void
*/
void write_calendar_to_file(st_root* p_root, FILE* h_calendar_file)
{
	st_year* p_year = p_root->pl_year;

	while (p_year != NULL)
	{
		st_month* p_month = p_year->pl_month;
		while (p_month != NULL)
		{
			st_day* p_day = p_month->pl_day;
			while (p_day != NULL)
			{
				st_appointment* p_appointment = p_day->pl_appointment;
				while (p_appointment != NULL)
				{
					fprintf(h_calendar_file, "%s;%s;%s;%04d/%02d/%02d;%02d:%02d;%02d:%02d;%d\n", p_appointment->p_title, p_appointment->p_description,
						p_appointment->p_location_description, p_appointment->date.year, p_appointment->date.month,
						p_appointment->date.day, p_appointment->time_start.hour, p_appointment->time_start.minute,
						p_appointment->time_end.hour, p_appointment->time_end.minute, p_appointment->id);
					p_appointment = p_appointment->pl_next_appointment;
				}
				p_day = p_day->pl_next_day;
			}
			p_month = p_month->pl_next_month;
		}
		p_year = p_year->pl_next_year;
	}
}




/**
* @brief	This function removes all appointments (from the tree) or all the appointments in a given range.
* @param	p_root (st_root*)	pointer to the tree
* @param	remove_all (int)	0 = ask user for entering a date-range and remove only those appointments meeting the range,
*								NOT 0 = remove all appointments.
* @param	print_details (int)	0 = don't print function details, NOT 0 = do print function details
*								This is for in case this function gets called from the import function and to avoid unnecessary prints
* @return	void
*/
void remove_appointments_in_range_or_all(st_root* p_root, int remove_all, int print_details)
{
	st_date start_date;
	st_date end_date;
	int start_date_in_days = 0;
	int end_date_in_days = 0;
	int start_year_month_in_days = 0;
	int end_year_month_in_days = 0;

	st_year* p_year = p_root->pl_year;
	//if tree is already empty
	if (p_year == NULL)
	{
		if (print_details != 0)
		{	
			printf("Calendar is empty! Nothing to delete!\n");
		}
		
		return;
	}
	

	//preset structures to 0, to avoid garbage in case of remove_all
	memset(&start_date, 0, sizeof(start_date));
	memset(&end_date, 0, sizeof(end_date));

	if (remove_all == 0)
	{	//if not remove all, ask for date range
		user_request_date_range(&start_date,
								&end_date,
								&start_date_in_days,
								&start_year_month_in_days,
								&end_date_in_days,
								&end_year_month_in_days);
	}

	//if remove_all == 0 -> only remove the appointments whose date are in range
	//if remove_all != 0 -> remove all appointments in the tree


	//set flag in case nothing was found
	int found = 0;

	//browse through years, months and days
	st_year* p_last_year_before_range = NULL;
	while (p_year != NULL)
	{
		//create backup for year
		st_year* backup_pl_next_year = p_year->pl_next_year;

		if ((remove_all != 0) ||														//if remove all is wanted or..
			((start_date.year <= p_year->year) && (p_year->year <= end_date.year)))		//...if year in range
		{
			st_month* p_month = p_year->pl_month;
			st_month* p_last_month_before_range = NULL;
			while (p_month != NULL)
			{
				//create backup for month
				st_month* backup_pl_next_month = p_month->pl_next_month;
				int current_year_month_in_days = 0;
				if (remove_all == 0)
				{
					current_year_month_in_days = date_to_int(p_year->year, p_month->month, 0);
				}
				if ((remove_all != 0) ||																									//if remove all is wanted or..	
					((start_year_month_in_days <= current_year_month_in_days) && (current_year_month_in_days <= end_year_month_in_days)))	//...if year+month in range
					//if ((start_date.month <= p_month->month) && (p_month->month <= end_date.month))
				{
					st_day* p_day = p_month->pl_day;
					st_day* p_last_day_before_range = NULL;
					while (p_day != NULL)
					{
						//create backup for day
						st_day* backup_pl_next_day = p_day->pl_next_day;
						int current_date_in_days = 0;
						if (remove_all == 0)
						{
							current_date_in_days = current_year_month_in_days + p_day->day;
						}
						if ((remove_all == 0) ||																			//if remove all is wanted or..
							((start_date_in_days <= current_date_in_days) && (current_date_in_days <= end_date_in_days)))	//...if year+month+day in range
							//if ((start_date.day <= p_day->day) && (p_day->day <= end_date.day))
						{
							st_appointment* p_appointment = p_day->pl_appointment;
							while (p_appointment != NULL)
							{
								//if there is an appointment found, delete it and goto next

								st_appointment* backup_pl_next_appointment = p_appointment->pl_next_appointment;	//backup first link to next one.
		
								//before we free the allocated appointment, free first the allocated strings
								free(p_appointment->p_title);
								free(p_appointment->p_description);
								free(p_appointment->p_location_description);
								free(p_appointment);
								p_appointment = backup_pl_next_appointment;	//take next apointment and repeat loop as long as not NULL

								found++;
							}

							//all appointments have been freed, now free day and re-link last day before range
							free(p_day);
							if (p_last_day_before_range != NULL)
							{	//another day is before the deleted day -> relink to next day
								p_last_day_before_range->pl_next_day = backup_pl_next_day;
							}
							else
							{	//the deleted day was the first of the list -> relink month to next day (that will now become first day)
								p_month->pl_day = backup_pl_next_day;
							}
						}
						else
						{	//when day is not in range, refresh backup pointer
							p_last_day_before_range = p_day;
						}
						p_day = backup_pl_next_day;	//goto next day in linked list
					}
					//if all days in the month have been freed, free month
					if (p_month->pl_day == NULL)
					{
						free(p_month);

						if (p_last_month_before_range != NULL)
						{	//another month is before the deleted month -> relink to next month
							p_last_month_before_range->pl_next_month = backup_pl_next_month;
						}
						else
						{	//the deleted month was the first of the list -> relink year to next month (that will now become first month)
							p_year->pl_month = backup_pl_next_month;
						}
					}
				}
				else
				{	//when month is out of range, refresh backup pointer
					p_last_month_before_range = p_month;
				}
				p_month = backup_pl_next_month; //goto next month in linked list
			}

			//if all months in the year have been freed, free year
			if (p_year->pl_month == NULL)
			{
				free(p_year);

				if (p_last_year_before_range != NULL)
				{	//another year is before the deleted year -> relink to next year
					p_last_year_before_range->pl_next_year = backup_pl_next_year;
				}
				else
				{	//the deleted year was the first of the list -> relink root to next year (that will now become first year)
					p_root->pl_year = backup_pl_next_year;
				}
			}
		}
		else
		{	//when year is out of range, refresh backup pointer
			p_last_year_before_range = p_year;
		}
		p_year = backup_pl_next_year; //goto next year in linked list
	}

	//if no appointments were found in range
	if ((found == 0) && (remove_all == 0))
	{
		printf("No appointments were found within this range! Nothing has been deleted.\n");
	}
	else if (remove_all == 0)
	{
		printf("%d appointments deleted!\n", found);
	}
	else
	{
		printf("Calendar completely removed!\n");
		//root points to NULL (init root)
		init_root(p_root);
	}
}



// TODO: import and export file (remove the absolute paths of the default file) (done)
// TODO: compile for Linux (done)
// TODO: add appointment function (done)
// TODO: print calendar function (done)
// TODO: remove calendar function (done)
// TODO: CATCH MALLOC ERRORS (done)
// TODO: CHECK FGETS AND SCANF ERRORS (LIKE IF ITS EMPTY OR IF IT HAS EXCEEDED) (done for fgets)

