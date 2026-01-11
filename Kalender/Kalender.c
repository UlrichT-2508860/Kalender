#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"
#include "Kalender.h"






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
	return (int) current_time;

}


/**
* @brief	Gets the max amount of days depending on the month (and leap year)
* @param	year (int) A given year
* @param	month (int) A given month
* @return	The max amount of months (int)
*/
int get_max_days_of_month(int year, int month)
{
	//calculate max days:
	unsigned int max_days;
	switch (month)
	{
		case FEB:

			//check for leap year
			if ((year % 400 == 0) ||
				((year % 4 == 0) && (year % 100 != 0)))
			{
				max_days = 29;
			}
			else
			{
				max_days = 28;
			}
			break;
		case JAN:
		case MAR:
		case MAY:
		case JUL:
		case AUG:
		case OCT:
		case DEC:
			max_days = 31;
			break;
		default:
			max_days = 30;
			break;
	}
	return max_days;
}

/**
* @brief	Converts a given date into the amount of days
* @param	year (int) A given year
* @param	month (int) A given month
* @param	day (int) A given day
* @return	The number of days (int)
*/
int date_to_int(int year, int month, int day)
{
	int number_of_days = (year * 365) +		//add default number of days per year
		((year - 1) / 4) +		//add leap days for all leap years.
		day;				//add day in last month.

	//add days of past months
	for (int i = 1; i < month; i++)
	{
		number_of_days += get_max_days_of_month(year, i);
	}
	return number_of_days;
}

/**
* @brief	Checks if a given date struct is a valid date
* @param	p_date (st_date*) The address of a date struct
* @return	0 if date is valid, -1 if not valid (int)
*/
int is_date_valid(st_date* p_date)
{
	//check year (only positive numbers)
	if (p_date->year < 0)
	{
		return -1;
	}

	//check month (1..12)
	if ((p_date->month < 1) ||
		(p_date->month > 12))
	{
		return -1;
	}

	//calculate max days:
	unsigned int max_days = get_max_days_of_month(p_date->year, p_date->month);
	//check day
	if ((p_date->day < 1) ||
		(p_date->day > max_days))
	{
		return -1;
	}

	//date is valid
	return 0;
}

/**
* @brief	Check is a given time stuct is valid
* @param	p_time (st_time*) The address of a time struct
* @return	0 if time is valid, -1 if not valid
*/
int is_time_valid(st_time* p_time)
{
	//check for hours
	if ((p_time->hour < 0) ||
		(p_time->hour > 23))
	{
		return -1;
	}

	//check for minutes
	if ((p_time->minute < 0) ||
		(p_time->minute > 59))
	{
		return -1;
	}

	//time is valid
	return 0;
}

/**
 * @brief	Checks if the end time falls before the start time
 * @param	p_end_time pointer to the end time struct
 * @param	p_start_time pointer to the start time struct
 * @return	0 if valid, -1 if not valid
 */
int is_end_time_valid_to_start_time(st_time* p_end_time, st_time* p_start_time)
{
	if (p_end_time->hour * 60 + p_end_time->minute >
		p_start_time->hour * 60 + p_start_time->minute)
	{
		return 0;
	}
	return -1;
}



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
		tmp_year = malloc_s(sizeof(st_month));
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


		// 1) The wanted year needs to be inserted in the beginning
		if ((previous_tmp_year == NULL) && (tmp_year->year > year)) 
		{
			tmp_new_year->pl_next_year = tmp_year;
			p_this_root->pl_year = tmp_new_year;
		}
		else
		{
			if (tmp_year->year < year)
			{
				// 1) The wanted year is higher than the current year -> add new entry
				tmp_year->pl_next_year = tmp_new_year;
				tmp_new_year->pl_next_year = NULL;
			}
			else
			{
				// 1) The wanted year is lower than the current year. -> insert new entry (rewiring).
				tmp_new_year->pl_next_year = tmp_year;
				previous_tmp_year->pl_next_year = tmp_new_year;
			}
		}

	}
	else
	{
		return tmp_year;
	}

	//return p_this_root->pl_year;
	return tmp_new_year;
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

		//the wanted month needs to be inserted in the beginning
		if ((previous_tmp_month == NULL) && (tmp_month->month > month))
		{
			tmp_new_month->pl_next_month = tmp_month;
			p_this_year->pl_month = tmp_new_month;
		}
		else
		{
			//the wanted month needs to be added
			if (tmp_month->month < month)
			{
				tmp_new_month->pl_next_month = NULL;
				tmp_month->pl_next_month = tmp_new_month;
			}

			//the wanted month needs to be inserted
			else
			{
				tmp_new_month->pl_next_month = tmp_month;
				previous_tmp_month->pl_next_month = tmp_new_month;

			}
		}
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

		//the wanted day needs to be inserted in the beginning
		if ((previous_tmp_day == NULL) && (tmp_day->day > day))
		{
			tmp_new_day->pl_next_day = tmp_day;
			p_this_month->pl_day = tmp_new_day;
		}
		else
		{
			//the wanted day needs to be added
			if (tmp_day->day < day)
			{
				tmp_new_day->pl_next_day = NULL;
				tmp_day->pl_next_day = tmp_new_day;
			}

			//the wanted day needs to be inserted
			else
			{
				tmp_new_day->pl_next_day = tmp_day;
				previous_tmp_day->pl_next_day = tmp_new_day;

			}
		}
		return tmp_new_day;
	}

	//day already present, return pointer 
	return tmp_day;


}

/**
* @brief	This function will create dynamically a new appointment struct in a given days linked-list and sorts it based on the start time of the appointment.
*			The appointment will update the linked list 
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
		//if (tmp_appointment_time_start_in_minutes == time_start_in_minutes)
		//{
		//	__nop();
		//}

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
	//printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
	{
		printf("\n APPOINTMENT: %d\n\n", p_appointment->id);
		//printf("\n");
		printf("  Title: %s\n\n", p_appointment->p_title);
		//printf("\n");
		printf("   Description: %s\n", p_appointment->p_description);
		printf("   Location: %s\n", p_appointment->p_location_description);
		//printf("  Date: %04d/%02d/%02d\n", p_appointment->date.year, p_appointment->date.month, p_appointment->date.day);
		printf("   Start-time: %02d:%02d\n", p_appointment->time_start.hour, p_appointment->time_start.minute);
		printf("   End-time: %02d:%02d\n\n", p_appointment->time_end.hour, p_appointment->time_end.minute);
		//printf("\n");
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

	fgets_s(match_string, sizeof(match_string), stdin, 1, "Give the string you want to search for: ");
	//printf("Give the string you want to search for: ");
	//
	//fgets(match_string, sizeof(match_string), stdin);
	////TODO: GENERALIZE THIS FUNCTION FOR ALMOST EVERY FGETS

	////when invalid input (aka empty), try again
	//while (match_string[0] == '\n' ||
	//	(match_string[strlen(match_string) - 1] != '\n'))
	//{
	//	printf("You gave an empty input or you exceeded the given max length! Please try again: ");
	//	if (match_string[strlen(match_string) - 1] != '\n')
	//	{
	//		flush_keyboard_input();
	//	}
	//	fgets(match_string, MAX_TITLE_LENGTH, stdin);
	//}

	////remove the '\n'
	//match_string[strlen(match_string) - 1] = '\0';

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
				//printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
				st_appointment* p_appointment = p_day->pl_appointment;
				while (p_appointment != NULL)
				{
					//if match string is present, display appointment
					char lowered_title[MAX_TITLE_LENGTH];
					strcpy(lowered_title, p_appointment->p_title);
					string_to_lower(lowered_title);
					if (strstr(lowered_title, match_string))
					//if (strstr(p_appointment->p_title, match_string))
					{
						printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
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
* @brief	This function requests the user to input a time and update a given time struct
* @param	p_time (st_time*) The address to the time struct
* @param	p_message (char*) A pointer to a string to print if it was given
* @return	void
*/
void user_request_time(st_time* p_time, char* p_message)
{
	//print a message (if not NULL) and get the time.
	if (p_message != NULL)
	{
		printf(p_message);
	}
	scanf("%d:%d", &p_time->hour, &p_time->minute);
	flush_keyboard_input();	//flush garbage input

	//check for invalid input
	while (is_time_valid(p_time) != 0)
	{
		printf("INVALID TIME. USE THE GIVEN FORMAT (HH:MM): ");
		scanf("%d:%d", &p_time->hour, &p_time->minute);
		flush_keyboard_input();	//flush garbage input
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
void user_request_time_range(st_date* p_start_time,	st_date* p_end_time)
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
			printf("Please try again (press enter).");
			flush_keyboard_input();	//wait for enter and flush garbage input
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
	 
	if (p_message != NULL)
	{	//print a message (if not NULL)
		printf(p_message);
	}
	scanf("%d/%d/%d", &p_date->year, &p_date->month, &p_date->day);	//get the date
	flush_keyboard_input();	//flush input

	//check for invalid input
	while (is_date_valid(p_date) != 0)
	{
		printf("INVALID DATE! TRY AGAIN AND USE THE GIVEN FORMAT: (YYYY/MM/DD): ");
		scanf("%d/%d/%d", &p_date->year, &p_date->month, &p_date->day);
		flush_keyboard_input();	//flush garbage input
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
void user_request_date_range(	st_date* p_start_date, 
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
		*p_end_date_in_days	= date_to_int(p_end_date->year, p_end_date->month, p_end_date->day);
		//Check if end-date is earlier than start-date.
		if (*p_end_date_in_days < *p_start_date_in_days)
		{
			//dates are NOT OK, repeat while loop until correct dates filled in.
			printf("END-DATE OCCURS EARLIER THAN START-DATE!\n");
			printf("Please try again (press enter).");
			flush_keyboard_input();	//wait for enter and flush garbage input
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
	int dates_valid = 0;
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
							printf("Date: %04d/%02d/%02d\n\n", p_year->year, p_month->month, p_day->day);
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
* @brief	This function will import a calendar by using a txt file, reading it, and getting every member
*			of an appointment struct. This function will crash when something invalid was read, it will delete
*			the root afterwards.
* @param	p_root (st_root*) The address of the root struct
* @param	default_filename (char*) The default filename, could be NULL
* @return	void
*/
void import_calendar_file(st_root* p_root, char* default_filename)
{

	char tmp_filename[512];	//to store eventually manually entered filepath
	int scanf_result;	//for checking scanf result
	int import_error = 0;		//for keeping errors during import
	int line_counter = 1;	
	st_appointment tmp_appointment;
	char s_line[ MAX_TITLE_LENGTH + MAX_LOCATION_LENGTH + MAX_DESCRIPTION_LENGTH + DATE_STR_LENGTH + (TIME_STR_LENGTH*2) + ID_STR_LENGTH + 10]; //use the size of the struct plus some extra characters for the separators.
	//char s_line[sizeof(st_appointment) + 10 + MAX_TITLE_LENGTH + MAX_LOCATION_LENGTH + MAX_DESCRIPTION_LENGTH];
	
	//TODO: MAKE THIS A FUNCTION
	if (default_filename == NULL)
	{	//If no filename is given, request the filepath+filename from user:
		printf("Please give the path to your .txt file: ");
		fgets(tmp_filename, sizeof(tmp_filename), stdin);
		tmp_filename[strlen(tmp_filename) - 1] = '\0';//remove \n
		default_filename = tmp_filename;
	}
	else
	{	//We still offer the choice to override the default filename
		printf("Please give the path to your .txt file or press enter to use the default calendar-file (%s) : ", default_filename);
		fgets(tmp_filename, sizeof(tmp_filename), stdin);
		if (tmp_filename[0] != '\n')
		{
			//user has specified a new filename, so use it.
			tmp_filename[strlen(tmp_filename) - 1] = '\0';//remove \n
			default_filename = tmp_filename;
		}
	}


	FILE* h_calendar_file = fopen(default_filename, "r");

	//in case file has not been found
	if (h_calendar_file == NULL)
	{
		printf("Error with opening file: %s\n", default_filename);
		return;
	}


	//first delete current tree
	remove_appointments_in_range_or_all(p_root, 1, 0);

	//get each line (entry) and convert to struct
	while (fgets(s_line, sizeof(s_line), h_calendar_file) != NULL)
	{

		//clear tmp struct
		memset(&tmp_appointment, NULL, sizeof(tmp_appointment));

		// - - - - - - - - - - - - - - - -
		//split the line in struct members
		// - - - - - - - - - - - - - - - -
		//get title (first member)
		char* p_member = my_strtok(s_line, FILE_DELIMITER);
		if ((strlen(p_member) == 0) || 
			(strlen(p_member) > MAX_TITLE_LENGTH))
		{
			import_error = IMPORT_ERROR_TITLE; //Mark as error
			break;
		}
		tmp_appointment.p_title = malloc_s(strlen(p_member) + 1); // malloc size of string + 1 for nullbyte
		//TODO CHECK IF MALLOC FAILED (NULL)
		strcpy(tmp_appointment.p_title, p_member);

		//get description (second member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
		if (strlen(p_member) > MAX_DESCRIPTION_LENGTH)
		{
			import_error = IMPORT_ERROR_DESCRIPTION;
			break;
		}
		tmp_appointment.p_description = malloc_s(strlen(p_member) + 1); // malloc size of string + 1 for nullbyte
		//TODO CHECK IF MALLOC FAILED (NULL)
		strcpy(tmp_appointment.p_description, p_member);

		//get location (third member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
		if (strlen(p_member) > MAX_LOCATION_LENGTH)
		{
			import_error = IMPORT_ERROR_LOCATION;
			break;
		}
		tmp_appointment.p_location_description = malloc_s(strlen(p_member) + 1); // malloc size of string + 1 for nullbyte
		//TODO CHECK IF MALLOC FAILED (NULL)
		strcpy(tmp_appointment.p_location_description, p_member);

		//get date (fourth member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
		scanf_result = sscanf(p_member, "%d/%d/%d", &tmp_appointment.date.year, &tmp_appointment.date.month, &tmp_appointment.date.day);
		if ((scanf_result != 3) ||	//bad structure?
			(is_date_valid(&tmp_appointment.date) != 0) )	//check for invalid data
		{
			import_error = IMPORT_ERROR_DATE;//Mark as error
			break;	//Cancel import-loop.
		}

		//get time start (fifth member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
		scanf_result = sscanf(p_member, "%d:%d", &tmp_appointment.time_start.hour, &tmp_appointment.time_start.minute);
		if ((scanf_result != 2) ||	//bad structure?
			(is_time_valid(&tmp_appointment.time_start) != 0) )	//invalid time??
		{
			import_error = IMPORT_ERROR_START_TIME;	//Mark as error
			break;	//Cancel import-loop.
		}

		//get time end (sixth member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
		scanf_result = sscanf(p_member, "%d:%d", &tmp_appointment.time_end.hour, &tmp_appointment.time_end.minute);
		if ((scanf_result != 2) ||	//bad structure?
			(is_time_valid(&tmp_appointment.time_end) != 0) ||
			(is_end_time_valid_to_start_time(&tmp_appointment.time_end, &tmp_appointment.time_start)) )	//invalid time??
		{
			import_error = IMPORT_ERROR_END_TIME;	//Mark as error
			break;	//Cancel import-loop.
		}

		//get unique id (last member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
		scanf_result = sscanf(p_member, "%d", &tmp_appointment.id);
		if ((scanf_result != 1)) 	//bad structure?
		{
			import_error = IMPORT_ERROR_ID;	//Mark as error
			break;	//Cancel import-loop.
		}

		// - - - - - - - - - - - - - - - - - - - 
		// Save Temp-struct to the tree structure
		// - - - - - - - - - - - - - - - - - - - 
		// 
		add_appointment_to_tree(p_root, &tmp_appointment);

		line_counter++;

	}
	
	if (import_error != 0)
	{
		printf("Error has occurred at line %d during import!\nCancelling import...\n", line_counter);
		switch (import_error)
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

	char tmp_filename[512];	//to store eventually manually entered filepath

	if (default_filename == NULL)
	{	//If no filename is given, request the filepath+filename from user:
		printf("Please give the path to your file:");
		fgets(tmp_filename, sizeof(tmp_filename), stdin);
		tmp_filename[strlen(tmp_filename) - 1] = '\0';
		default_filename = tmp_filename;
	}
	else
	{	//We still offer the choice to override the default filename
		printf("Please give the path to your file or press enter to use the default calendar-file (%s) : ", default_filename);
		fgets(tmp_filename, sizeof(tmp_filename), stdin);
		if (tmp_filename[0] != '\n')
		{
			//user has specified a new filename, so use it.
			tmp_filename[strlen(tmp_filename) - 1] = '\0';//remove \n
			default_filename = tmp_filename;
		}
	}
	FILE* h_calendar_file = fopen(default_filename, "w");

	st_year* p_year = p_root->pl_year;

	if (p_year == NULL)
	{
		printf("Tree is empty! Nothing to export!\n");
		return;
	}

	while (p_year != NULL)
	{
		st_month* p_month = p_year->pl_month;
		while (p_month != NULL)
		{
			st_day* p_day = p_month->pl_day;
			while (p_day != NULL)
			{
				//printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
				st_appointment* p_appointment = p_day->pl_appointment;
				while (p_appointment != NULL)
				{
					fprintf(h_calendar_file, "%s;%s;%s;%04d/%02d/%02d;%02d:%02d;%02d:%02d\n", p_appointment->p_title, p_appointment->p_description,
						p_appointment->p_location_description, p_appointment->date.year, p_appointment->date.month,
						p_appointment->date.day, p_appointment->time_start.hour, p_appointment->time_start.minute,
						p_appointment->time_end.hour, p_appointment->time_end.minute);
					p_appointment = p_appointment->pl_next_appointment;

				}
				p_day = p_day->pl_next_day;
			}
			p_month = p_month->pl_next_month;
		}
		p_year = p_year->pl_next_year;
	}
	fclose(h_calendar_file);
	printf("Export completed!\n");

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

	//get title of appointment
	fgets_s(title, sizeof(title), stdin, 1, "Give the title of your appointment: ");
	//printf("Give the title of your appointment: ");

	//fgets(title, sizeof(title), stdin);

	//while ((strlen(title) - 1 == 0) ||
	//	(strlen(title) - 1 > MAX_TITLE_LENGTH) ||
	//	title[strlen(title) - 1] != '\n')
	//{
	//	printf("Title of entry was empty or was longer than %d characters! Please try again: ", MAX_TITLE_LENGTH - 1); 
	//	if (title[strlen(title) - 1] != '\n')
	//	{
	//		flush_keyboard_input();
	//	}
	//	fgets(title, sizeof(title), stdin);
	//}
	////remove '\n'
	//title[strlen(title) - 1] = '\0';

	new_appointment.p_title = malloc_s(strlen(title) + 1); // malloc + 1 for nullbyte
	strcpy(new_appointment.p_title, title);

	//get optional description
	fgets_s(description, sizeof(description), stdin, 0, "Give an optional description of your appointment: ");
	//printf("Give an optional description of your appointment: ");

	//fgets(description, sizeof(description), stdin);

	//while (strlen(description) - 1 > MAX_DESCRIPTION_LENGTH)
	//{
	//	printf("Description of entry was longer than %d characters! Please try again: ", MAX_DESCRIPTION_LENGTH - 1);
	//	fgets(title, sizeof(title), stdin);
	//}
	////remove '\n'
	//description[strlen(description) - 1] = '\0';

	new_appointment.p_description = malloc_s(strlen(description) + 1); // malloc + 1 for nullbyte
	strcpy(new_appointment.p_description, description);

	//get optional location description
	fgets_s(location_description, sizeof(location_description), stdin, 0, "Give an optional location descrption of your appointment: ");
	//printf("Give an optional location description of your appointment: ");

	//fgets(location_description, sizeof(location_description), stdin);

	//while (strlen(location_description) - 1 > MAX_LOCATION_LENGTH)
	//{
	//	printf("Location description of entry was longer than %d characters! Please try again: ", MAX_LOCATION_LENGTH - 1);
	//	fgets(title, sizeof(title), stdin);
	//}
	////remove '\n'
	//location_description[strlen(location_description) - 1] = '\0';

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
	int dates_valid = 0;
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
								st_appointment* backup_pl_next_appointment = p_appointment->pl_next_appointment;
		
								//before we free the allocated appointment, free first the allocated strings
								// TODO MAKE THIS A FUNCTION									
								free(p_appointment->p_title);
								free(p_appointment->p_description);
								free(p_appointment->p_location_description);
								free(p_appointment);
								p_appointment = backup_pl_next_appointment;

								found++;
							}

							//appointments have been freed, now free day and re-link last day before range
							free(p_day);

							if (p_last_day_before_range != NULL)
							{
								p_last_day_before_range->pl_next_day = backup_pl_next_day;
							}

							else
							{	//if first day is in range
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
						{
							p_last_month_before_range->pl_next_month = backup_pl_next_month;
						}

						else
						{	//if first month is in range
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
				{
					p_last_year_before_range->pl_next_year = backup_pl_next_year;
				}
				else
				{	//first year is in range
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

//#define remove_all_appointments(p_root)		remove_appointments_in_range_or_all(p_root, 1, 0)


// TODO: import and export file (remove the absolute paths of the default file)
// TODO: add appointment function (done)
// TODO: print calendar function (done)
// TODO: remove calendar function (done)
// TODO: what to do with same time appoitnment (done)
// TODO: CATCH MALLOC ERRORS
// TODO: CHECK FGETS AND SCANF ERRORS (LIKE IF ITS EMPTY OR IF IT HAS EXCEEDED)

