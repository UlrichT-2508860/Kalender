#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Kalender.h"


//#define USE_MALLOCS

void flush_keyboard_input(void)
{
	char c;
	do
	{
		c = getchar();
	} while (c != '\n');

}

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


int generate_id(void)
{
	//source: https://www.geeksforgeeks.org/c/time-h-header-file-in-c-with-examples/

	time_t current_time = time(NULL); //get current time
	//return (int)(current_time % 1000000000);
	return (int) current_time;

}


void user_request_date(st_date* p_date, char* p_message)
{
	//print a message (if not NULL) and get the date.
	if (p_message != NULL)
	{
		printf(p_message);
	}
	scanf("%d/%d/%d", &p_date->year, &p_date->month, &p_date->day);
	flush_keyboard_input();	//flush input

	//check for invalid input
	while (is_date_valid(p_date) != 0)
	{
		printf("INVALID DATE! TRY AGAIN AND USE THE GIVEN FORMAT: (YYYY/MM/DD): ");
		scanf("%d/%d/%d", &p_date->year, &p_date->month, &p_date->day);
		flush_keyboard_input();	//flush garbage input
	}
}



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
			max_days = 31;
			break;
		case MAR:
			max_days = 31;
			break;
		case MAY:
			max_days = 31;
			break;
		case JUL:
			max_days = 31;
			break;
		case AUG:
			max_days = 31;
			break;
		case OCT:
			max_days = 31;
			break;
		case DEC:
			max_days = 31;
			break;
		default:
			max_days = 30;
			break;
	}
	return max_days;
}


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
	//switch (p_date->month)
	//{
	//case FEB:
	//	
	//	//check for leap year
	//	if ((p_date->year % 400 == 0) ||
	//		((p_date->year % 4 == 0) && (p_date->year % 100 != 0)))
	//	{
	//		max_days = 29;
	//	}
	//	else
	//	{
	//		max_days = 28;
	//	}
	//	break;
	//case JAN:
	//	max_days = 31;
	//	break;
	//case MAR:
	//	max_days = 31;
	//	break;
	//case MAY:
	//	max_days = 31;
	//	break;
	//case JUL:
	//	max_days = 31;
	//	break;
	//case AUG:
	//	max_days = 31;
	//	break;
	//case OCT:
	//	max_days = 31;
	//	break;
	//case DEC:
	//	max_days = 31;
	//	break;
	//default:
	//	max_days = 30;
	//	break;
	//}

	//check day
	if ((p_date->day < 1) ||
		(p_date->day > max_days))
	{
		return -1;
	}

	//date is valid
	return 0;
}

int is_time_valid(st_time* p_time)
{
	//check for hours
	if ((p_time->hour < 0) ||
		(p_time->hour > 23))
	{
		return -1;
	}

	//check for minutes
	if ((p_time->hour < 0) ||
		(p_time->minute > 59))
	{
		return -1;
	}

	//time is valid
	return 0;
}

/**
 * @brief Checks if the end time falls before the start time
 * @param p_end_time pointer to the end time struct
 * @param p_start_time pointer to the start time struct
 * @return 0 if valid, -1 if not valid
 */
int is_end_time_valid_to_start_time(st_time* p_end_time, st_time* p_start_time)
{
	if (p_end_time->hour * 60 + p_end_time->minute >=
		p_start_time->hour * 60 + p_start_time->minute)
	{
		return 0;
	}
	return -1;
}


st_year* get_or_create_year(st_root* p_this_root, int year)
{
	st_year* tmp_year = p_this_root->pl_year;
	st_year* previous_tmp_year = NULL;
	st_year* tmp_new_year = NULL;

	//if nothing is present in the root, create the first year struct and return directly
	if (tmp_year == NULL)
	{
		tmp_year = malloc(sizeof(st_month));
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

	// address	|  year		|	pl_next_year
	//----------+-----------+----------------
	//	1		|	1970	|	0x012345
	//	0x012345|	1980	|	NULL
	//			|			|				

	// case: 1971 is wanted:
	// we exit the while loop with:
	// address	|  year		|	pl_next_year
	//----------+-----------+----------------
	//	1		|	1970	|	0x012345		<- previous_tmp_year
	//	0x012345|	1980	|	NULL			<- tmp_year
	//
	//		after the insertion
	// address	|  year		|	pl_next_year
	//----------+-----------+----------------
	//	1		|	1970	|	0xEEEAAA
	//	0xEEEAAA|	1971	|	0x012345		<- tmp_new_year
	//	0x012345|	1980	|	NULL
	//
	//
	// case2: 1984 is wanted:
	// we exit the while loop with:
	// address	|  year		|	pl_next_year
	//----------+-----------+----------------
	//	1		|	1970	|	0x012345		<- previous_tmp_year
	//	0x012345|	1980	|	NULL			<- tmp_year
	//
	//		after the insertion
	// address	|  year		|	pl_next_year
	//----------+-----------+----------------
	//	1		|	1970	|	0x012345		<- previous_tmp_year
	//	0x012345|	1980	|	NULL			<- tmp_year
	//	1		|	1970	|	0xEEEAAA
	//	0xEEEAAA|	1971	|	0x012345		<- tmp_new_year
	//	0x012345|	1980	|	NULL
	//
	//



	//If we come here, either the year is found or we are at the end of the linked list
	if (tmp_year->year != year)
	{
		//year is not found create new year struct in memory:
		tmp_new_year = malloc(sizeof(st_year));
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




#if 0

		if (tmp_year->pl_next_year == NULL)
		{
			// 1) Are we at the of the list -> add new entry
			tmp_new_year->pl_next_year = NULL;
		}
		else
		{
			// 2) Are we not at the of the list -> insert new entry (rewiring).
			tmp_new_year->pl_next_year = tmp_year;
			previous_tmp_year->pl_next_year = tmp_new_year;
		}



		tmp_year->pl_next_year = malloc(sizeof(st_year));
		tmp_year->pl_next_year->year = year;

		if (tmp_year->pl_next_year != NULL)
		tmp_year->pl_next_year->pl_next_year = previous_tmp_year->pl_next_year;
#endif

	}
	else
	{
		return tmp_year;
	}

	//return p_this_root->pl_year;
	return tmp_new_year;
}



st_month* get_or_create_month(st_year* p_this_year, int month)
{
	st_month* tmp_month = p_this_year->pl_month;
	st_month* previous_tmp_month = NULL;
	st_month* tmp_new_month = NULL;

	//if nothing is present for this year, create the first month struct and return directly
	if (tmp_month == NULL)
	{
		tmp_month = malloc(sizeof(st_month));
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
		tmp_new_month = malloc(sizeof(st_month));
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


st_day* get_or_create_day(st_month* p_this_month, int day)
{
	st_day* tmp_day = p_this_month->pl_day;
	st_day* previous_tmp_day = NULL;
	st_day* tmp_new_day = NULL;

	//if nothing is present for this month, create the first day struct and return directly
	if (tmp_day == NULL)
	{
		tmp_day = malloc(sizeof(st_day));
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
		tmp_new_day = malloc(sizeof(st_day));
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

st_appointment* get_or_create_appointment(st_day* p_this_day, st_time* p_time_start)
{
	st_appointment* tmp_appointment = p_this_day->pl_appointment;
	st_appointment* previous_appointment = NULL;
	st_appointment* tmp_new_appointment = NULL;
	int p_time_start_in_minutes = p_time_start->hour * 60 + p_time_start->minute;

	//if nothing is present on this day, create the first appointment struct with the given time
	if (tmp_appointment == NULL)
	{
		tmp_appointment = malloc(sizeof(st_appointment));
		tmp_appointment->time_start.hour = p_time_start->hour;
		tmp_appointment->time_start.minute = p_time_start->minute;
		tmp_appointment->pl_next_appointment = NULL;

		//update and return tree
		p_this_day->pl_appointment = tmp_appointment;
		return tmp_appointment;
	}

	//search for match in appointment
	//int tmp_appointment_time_in_minutes = tmp_appointment->time_start.hour * 60 + tmp_appointment->time_start.minute;

	while ((tmp_appointment->time_start.hour * 60 + tmp_appointment->time_start.minute < p_time_start_in_minutes)
		&& (tmp_appointment->pl_next_appointment != NULL))
	{
		previous_appointment = tmp_appointment;
		tmp_appointment = tmp_appointment->pl_next_appointment;
	}

	//check if match has been found
	if (tmp_appointment->time_start.hour * 60 + tmp_appointment->time_start.minute != p_time_start_in_minutes)
	{
		//appointment hour has not been found, create a new appointment struct in memory
		tmp_new_appointment = malloc(sizeof(st_appointment));
		tmp_new_appointment->time_start.hour = p_time_start->hour;
		tmp_new_appointment->time_start.minute = p_time_start->minute;

		//the appointment needs to be inserted in the beginning
		if ((previous_appointment == NULL) && (tmp_appointment->time_start.hour * 60 + tmp_appointment->time_start.minute >
			p_time_start->hour * 60 + p_time_start->minute))
		{
			tmp_new_appointment->pl_next_appointment = tmp_appointment;
			p_this_day->pl_appointment = tmp_new_appointment;
		}
		else
		{
			//the appointment needs to be added in the back
			if (tmp_appointment->time_start.hour * 60 + tmp_appointment->time_start.minute <
				p_time_start->hour * 60 + p_time_start->minute)
			{
				tmp_new_appointment->pl_next_appointment = NULL;
				tmp_appointment->pl_next_appointment = tmp_new_appointment;
			}

			//the appointment needs to be inserted
			else
			{
				tmp_new_appointment->pl_next_appointment = tmp_appointment;
				previous_appointment->pl_next_appointment = tmp_new_appointment;

			}
		}
		return tmp_new_appointment;

	}

	
	//return p_this_day->pl_appointment;

}




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

	//Find or create the wanted appointment.
	p_appointment = get_or_create_appointment(p_day, &p_new_appointment->time_start);

	//Fill in all the data of the appointment.
	//todo: fill in "int id";

	//tip: the code below can also be done as follows:
	//struct st_appointment* pl_next_appointment_backup = p_appointment->pl_next_appointment;
	//memcpy(&p_appointment, &p_new_appointment, sizeof(st_appointment));
	//p_appointment->pl_next_appointment = pl_next_appointment_backup;
#ifdef USE_MALLOCS
	memcpy(p_appointment, p_new_appointment, sizeof(st_appointment));


#else
	p_appointment->id = p_new_appointment->id;
	memcpy(&p_appointment->date, &p_new_appointment->date, sizeof(st_date));
	memcpy(&p_appointment->time_start, &p_new_appointment->time_start, sizeof(st_time));
	memcpy(&p_appointment->time_end, &p_new_appointment->time_end, sizeof(st_time));
	strcpy(&p_appointment->title, &p_new_appointment->title);
	strcpy(&p_appointment->description, &p_new_appointment->description);
	strcpy(&p_appointment->location_description, &p_new_appointment->location_description);
#endif


}



void print_appointment_details(st_appointment* p_appointment)
{
	//printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
	{
		printf("\n APPOINTMENT: %d\n\n", p_appointment->id);
		//printf("\n");
		printf("  Title: %s\n\n", p_appointment->title);
		//printf("\n");
		printf("   Description: %s\n", p_appointment->description);
		printf("   Location: %s\n", p_appointment->location_description);
		//printf("  Date: %04d/%02d/%02d\n", p_appointment->date.year, p_appointment->date.month, p_appointment->date.day);
		printf("   Start-time: %02d:%02d\n", p_appointment->time_start.hour, p_appointment->time_start.minute);
		printf("   End-time: %02d:%02d\n\n", p_appointment->time_end.hour, p_appointment->time_end.minute);
		//printf("\n");
	}
}





void print_appointments_from_tree(st_root* p_root)
{
	st_year* p_year = p_root->pl_year;

	if (p_year == NULL)
	{
		printf("Tree is empty! Nothing to display!\n");
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
				printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
				st_appointment* p_appointment = p_day->pl_appointment;
				while (p_appointment != NULL)
				{
					print_appointment_details(p_appointment);
					//printf("\n");
					//printf(" Title: %s\n", p_appointment->title);
					//printf("\n");
					//printf("  Description: %s\n", p_appointment->description);
					//printf("  Location: %s\n", p_appointment->location_description);
					////printf("  Date: %04d/%02d/%02d\n", p_appointment->date.year, p_appointment->date.month, p_appointment->date.day);
					//printf("  Start-time: %02d:%02d\n", p_appointment->time_start.hour, p_appointment->time_start.minute);
					//printf("  End-time: %02d:%02d\n", p_appointment->time_end.hour, p_appointment->time_end.minute);
					//printf("\n");
					p_appointment = p_appointment->pl_next_appointment;

				}
				p_day = p_day->pl_next_day;
			}
			p_month = p_month->pl_next_month;
		}
		p_year = p_year->pl_next_year;
	}
}


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
	char match_string[MAX_TITLE_LENGTH];

	printf("Give the string you want to search for: ");
	
	fgets(match_string, MAX_TITLE_LENGTH, stdin);

	//when invalid input (aka empty), try again
	while (match_string[0] == '\n')
	{
		printf("You gave an empty input! Please try again: ");
		fgets(match_string, MAX_TITLE_LENGTH, stdin);
	}

	//remove the '\n'
	match_string[strlen(match_string) - 1] = '\0';

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
					if (strstr(p_appointment->title, match_string))
					{
						printf("Date: %04d/%02d/%02d\n", p_year->year, p_month->month, p_day->day);
						print_appointment_details(p_appointment);
						found = 1;
						//printf("\n");
						//printf(" Title: %s\n", p_appointment->title);
						//printf("\n");
						//printf("  Description: %s\n", p_appointment->description);
						//printf("  Location: %s\n", p_appointment->location_description);
						////printf("  Date: %04d/%02d/%02d\n", p_appointment->date.year, p_appointment->date.month, p_appointment->date.day);
						//printf("  Start-time: %02d:%02d\n", p_appointment->time_start.hour, p_appointment->time_start.minute);
						//printf("  End-time: %02d:%02d\n", p_appointment->time_end.hour, p_appointment->time_end.minute);
						//printf("\n");
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

void print_calendar_in_range(st_root* p_root)
{
	st_date start_date;
	st_date end_date;
	int dates_valid=0;
	int start_date_in_days;
	int start_year_month_in_days;
	int end_date_in_days;
	int end_year_month_in_days;
	//if tree is empty
	if (p_root->pl_year == NULL)
	{
		printf("Tree is empty! Nothing to print!");
		return;
	}

	do
	{
		//get the start-date
		user_request_date(&start_date, "Please give the date where you want to start searching: (YYYY/MM/DD) ");

		//printf("Please give the date where you want to start searching: (YYYY/MM/DD) ");
		//scanf("%d/%d/%d", &start_date.year, &start_date.month, &start_date.day);
		//flush_keyboard_input();	//flush input
		////check for invalid input
		//while (is_date_valid(&start_date) != 0)
		//{
		//	printf("INVALID DATE! USE THE GIVEN FORMAT: (YYYY/MM/DD) ");
		//	scanf("%d/%d/%d", &start_date.year, &start_date.month, &start_date.day);
		//	flush_keyboard_input();	//flush garbage input
		//}

		//get the end-date
		user_request_date(&end_date, "Please give the date where you want to stop searching: (YYYY/MM/DD) ");

		//printf("Please give the date where you want to stop searching: (YYYY/MM/DD) ");
		//scanf("%d/%d/%d", &end_date.year, &end_date.month, &end_date.day);
		//flush_keyboard_input();	//flush input
		////check for invalid input
		//while (is_date_valid(&end_date) != 0)
		//{
		//	printf("INVALID DATE! USE THE GIVEN FORMAT: (YYYY/MM/DD) ");
		//	scanf("%d/%d/%d", &end_date.year, &end_date.month, &end_date.day);
		//	flush_keyboard_input();	//flush garbage input
		//}
		start_date_in_days = date_to_int(start_date.year, start_date.month, start_date.day);
		end_date_in_days = date_to_int(end_date.year, end_date.month, end_date.day);
		//Check if end-date is earlier than start-date.
		if (end_date_in_days < start_date_in_days)
		/*if ( (end_date.year < start_date.year) ||
			((end_date.year == start_date.year) && (end_date.month < start_date.month)) ||
			((end_date.year == start_date.year) && (end_date.month == start_date.month) && (end_date.day< start_date.day)) )*/
		{
			//dates are NOK, repeat while loop until correct dates filled in.
			printf("END-DATE OCCURS EARLIER THAN START-DATE!\n");
			printf("Please try again (press enter).");
			flush_keyboard_input();	//wait for enter and flush garbage input
		}
		else
		{
			//dates are OK, break off while loop and continue
			start_year_month_in_days = date_to_int(start_date.year, start_date.month, 0);
			end_year_month_in_days = date_to_int(end_date.year, end_date.month, 0);


			dates_valid = 1;
		}
	} while (dates_valid == 0);

	//flush '\n'
	

	//only print the appointments whose date are in range
	st_year* p_year = p_root->pl_year;
	//set flag in case nothing was found
	int found = 0;

	//browse through years, months and days
	while (p_year != NULL)
	{
		if ((start_date.year <= p_year->year) && (p_year->year <= end_date.year))
		{
			st_month* p_month = p_year->pl_month;
			while (p_month != NULL)
			{
				int current_year_month_in_days = date_to_int(p_year->year, p_month->month, 0);
				if ((start_year_month_in_days <= current_year_month_in_days) && (current_year_month_in_days <= end_year_month_in_days))
				//if	((start_date.month <= p_month->month) && (p_month->month <= end_date.month))
				{
					st_day* p_day = p_month->pl_day;
					while (p_day != NULL)
					{
						int current_date_in_days = current_year_month_in_days + p_day->day;
						if ((start_date_in_days <= current_date_in_days) && (current_date_in_days <= end_date_in_days))
						//if ((start_date.day <= p_day->day) && (p_day->day <= end_date.day))
						{
							printf("Date: %04d/%02d/%02d\n\n", p_year->year, p_month->month, p_day->day);
							st_appointment* p_appointment = p_day->pl_appointment;
							while (p_appointment != NULL)
							{
								//if (start_date.year <= p_year->year <= end_date.year)	//note: this doesn't work well in C.. Why##??@@!! 

								//if (((start_date.year <= p_year->year) && (p_year->year <= end_date.year)) &&
								//	((start_date.month <= p_month->month) && (p_month->month <= end_date.month))&&
								//	((start_date.day <= p_day->day) && (p_day->day <= end_date.day)))
								{
									
									print_appointment_details(p_appointment);
									//printf(" Title: %s\n\n", p_appointment->title);
									//printf("  Description: %s\n", p_appointment->description);
									//printf("  Location: %s\n", p_appointment->location_description);
									////printf("  Date: %04d/%02d/%02d\n", p_appointment->date.year, p_appointment->date.month, p_appointment->date.day);
									//printf("  Start-time: %02d:%02d\n", p_appointment->time_start.hour, p_appointment->time_start.minute);
									//printf("  End-time: %02d:%02d\n\n", p_appointment->time_end.hour, p_appointment->time_end.minute);
									found = 1;
								}
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

	if (found == 0)
	{
		printf("No appointments found in this range!\n");
	}
}

void import_calendar_file(st_root* p_root, char* default_filename)
{

	char tmp_filename[512];	//to store eventually manually entered filepath
	int scanf_result;	//for checking scanf result
	int import_error = 0;		//for keeping errors during import
	int line_counter = 1;	
	st_appointment tmp_appointment;
	char s_line[sizeof(st_appointment) + 10]; //use the size of the struct plus some extra characters for the separators.

	
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
	remove_tree(p_root, 0);

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
#ifdef USE_MALLOCS
		tmp_appointment.p_title = malloc(strlen(p_member) + 1); // malloc size of string + 1 for nullbyte
		//TODO CHECK IF MALLOC FAILED (NULL)
		strcpy(tmp_appointment.p_title, p_member);
#else
		strcpy(tmp_appointment.title, p_member);
		
#endif

		//get description (second member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
#ifdef USE_MALLOCS
		tmp_appointment.p_description = malloc(strlen(p_member) + 1); // malloc size of string + 1 for nullbyte
		//TODO CHECK IF MALLOC FAILED (NULL)
		strcpy(tmp_appointment.p_description, p_member);
#else
		strcpy(tmp_appointment.description, p_member);
#endif

		//get location (third member)
		p_member = my_strtok(NULL, FILE_DELIMITER);
#ifdef USE_MALLOCS
		tmp_appointment.p_location_description = malloc(strlen(p_member) + 1); // malloc size of string + 1 for nullbyte
		//TODO CHECK IF MALLOC FAILED (NULL)
		strcpy(tmp_appointment.p_location_description, p_member);
#else
		strcpy(tmp_appointment.location_description, p_member);
#endif

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
				printf("Title of entry was empty or was longer than 100 bytes!\n");
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
		remove_tree(p_root, 0);	//remove partly imported tree 
	}
	else
	{
		printf("Import done!\n");
	}

	fclose(h_calendar_file);
}
void export_calendar_file(st_root* p_root, char* filename)
{
	FILE* h_calendar_file = fopen(filename, "w");

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

					fprintf(h_calendar_file, "%s;%s;%s;%04d/%02d/%02d;%02d:%02d;%02d:%02d\n", p_appointment->title, p_appointment->description,
							p_appointment->location_description, p_appointment->date.year, p_appointment->date.month,
							p_appointment->date.day, p_appointment->time_start.hour, p_appointment->time_start.minute,
							p_appointment->time_end.hour, p_appointment->time_end.minute);

					p_appointment = p_appointment->pl_next_appointment;
					//printf(" Title: %s\n", p_appointment->title);
					//printf("  Description: %s\n", p_appointment->description);
					//printf("  Location: %s\n", p_appointment->location_description);
					////printf("  Date: %04d/%02d/%02d\n", p_appointment->date.year, p_appointment->date.month, p_appointment->date.day);
					//printf("  Start-time: %02d:%02d\n", p_appointment->time_start.hour, p_appointment->time_start.minute);
					//printf("  End-time: %02d:%02d\n", p_appointment->time_end.hour, p_appointment->time_end.minute);
					//p_appointment = p_appointment->pl_next_appointment;

				}
				p_day = p_day->pl_next_day;
			}
			p_month = p_month->pl_next_month;
		}
		p_year = p_year->pl_next_year;
	}
	printf("Export completed!\n");

}
/**
* @brief Adds an appointment to the calendar
* 
*/
void add_appointment_manually(st_root* p_root)
{
	
	st_appointment new_appointment;
	/*st_date date;
	char title[100];
	char description[300];
	char location_description[100];*/

	//get title of appointment
	printf("Give the title of your appointment: ");
	//TODO: why is this needed twice???answer= beware previous scanf!! it still contains the newline in its stdin-buffer
	fgets(&new_appointment.title, sizeof(new_appointment.title), stdin);

	//when invalid input (aka empty), try again
	while (new_appointment.title[0] == '\n')
	{
		printf("You gave an empty input! Please try again: ");
		fgets(&new_appointment.title, sizeof(new_appointment.title), stdin);
	}

	//remove '\n'
	new_appointment.title[strlen(new_appointment.title) - 1] = '\0';
	//new_appointment.title[strcspn(new_appointment.title, "\n")] = '\0';

	//get optional description
	printf("Give an optional description of your appointment: ");
	fgets(&new_appointment.description, sizeof(new_appointment.description), stdin);
	
	new_appointment.description[strlen(new_appointment.description) - 1] = '\0';
	//new_appointment.description[strcspn(new_appointment.description, "\n")] = '\0';

	//get optional location description
	printf("Give an optional location description of your appointment: ");
	fgets(&new_appointment.location_description, sizeof(new_appointment.location_description), stdin);
	
	new_appointment.location_description[strlen(new_appointment.location_description) - 1] = '\0';
	//new_appointment.location_description[strcspn(new_appointment.location_description, "\n")] = '\0';

	//get date
	printf("Give the date of your appointment. (Format: YYYY/MM/DD) : ");
	//scanf("%d", &new_appointment.date.year);
	scanf("%d/%d/%d", &new_appointment.date.year, &new_appointment.date.month, &new_appointment.date.day);

	//in case of invalid date input
	while (is_date_valid(&new_appointment.date) != 0)
	{
		//flush garbage input
		flush_keyboard_input();
		printf("INVALID DATE. USE THE GIVEN FORMAT: (YYYY/MM/DD) ");
		 scanf("%d/%d/%d", &new_appointment.date.year, &new_appointment.date.month, &new_appointment.date.day);
	}

	//get specific time when appointment starts
	printf("Give the specific time when your appointment starts. (Format: HH:MM) : ");
	scanf("%d:%d", &new_appointment.time_start.hour, &new_appointment.time_start.minute);

	//in case of invalid time input
	while (is_time_valid(&new_appointment.time_start) != 0)
	{
		//flush garbage input
		flush_keyboard_input();
		printf("INVALID TIME. USE THE GIVEN FORMAT: (HH:MM) ");
		scanf("%d:%d", &new_appointment.time_start.hour, &new_appointment.time_start.minute);
	}

	//get specific time when appointment ends
	printf("Give the specific time when your appointment ends. (Format: HH:MM) : ");
	scanf("%d:%d", &new_appointment.time_end.hour, &new_appointment.time_end.minute);

	//in case of invalid time input OR time-end is earlier than time-start
	while ((is_time_valid(&new_appointment.time_start) != 0) ||
		new_appointment.time_end.hour * 60 + new_appointment.time_end.minute < new_appointment.time_start.hour * 60 + new_appointment.time_start.minute)
	{
		//flush garbage input
		flush_keyboard_input();

		//when time is an invalid input:
		if (is_time_valid(&new_appointment.time_start) != 0)
		{
			printf("INVALID TIME. USE THE GIVEN FORMAT: (HH:MM) ");
		}
		//when time-end is earlier than time-start:
		else 
		{
			printf("TIME-END OCCURS EARLIER THAN TIME-START!\n");
			printf("Please try again: (HH:MM):");
		}
		
		scanf("%d:%d", &new_appointment.time_end.hour, &new_appointment.time_end.minute);
	}

	//flush keyboard input
	flush_keyboard_input();

	//generate unique id for this appointment
	new_appointment.id = generate_id();

	add_appointment_to_tree(p_root, &new_appointment);
	printf("Appointment has been added to Calendar!\n");

}

void remove_appointments_in_range(st_root* p_root)
{


	
	st_date start_date;
	st_date end_date;
	int dates_valid = 0;
	int start_date_in_days;
	int end_date_in_days;
	int start_year_month_in_days;
	int end_year_month_in_days;

	st_year* p_year = p_root->pl_year;
	//if tree is already empty
	if (p_year == NULL)
	{
		printf("Tree is emtpy! Nothing to delete!\n");
		return;
	}

	do
	{
		//get the start-date
		printf("Please give the date where you want to start searching: (YYYY/MM/DD) ");
		scanf("%d/%d/%d", &start_date.year, &start_date.month, &start_date.day);
		flush_keyboard_input();	//flush input
		//check for invalid input
		while (is_date_valid(&start_date) != 0)
		{
			printf("INVALID DATE! USE THE GIVEN FORMAT: (YYYY/MM/DD) ");
			scanf("%d/%d/%d", &start_date.year, &start_date.month, &start_date.day);
			flush_keyboard_input();	//flush garbage input
		}

		//get the end-date
		printf("Please give the date where you want to stop searching: (YYYY/MM/DD) ");
		scanf("%d/%d/%d", &end_date.year, &end_date.month, &end_date.day);
		flush_keyboard_input();	//flush input
		//check for invalid input
		while (is_date_valid(&end_date) != 0)
		{
			printf("INVALID DATE! USE THE GIVEN FORMAT: (YYYY/MM/DD) ");
			scanf("%d/%d/%d", &end_date.year, &end_date.month, &end_date.day);
			flush_keyboard_input();	//flush garbage input
		}

		start_date_in_days = date_to_int(start_date.year, start_date.month, start_date.day);
		end_date_in_days = date_to_int(end_date.year, end_date.month, end_date.day);

		//Check if end-date is earlier than start-date.
		if (end_date_in_days < start_date_in_days)
		//if ((end_date.year < start_date.year) ||
		//	((end_date.year == start_date.year) && (end_date.month < start_date.month)) ||
		//	((end_date.year == start_date.year) && (end_date.month == start_date.month) && (end_date.day < start_date.day)))
		{
			//dates are NOK, repeat while loop until correct dates filled in.
			printf("END-DATE OCCURS EARLIER THAN START-DATE!\n");
			printf("Please try again (press enter).");
			flush_keyboard_input();	//wait for enter and flush garbage input
		}
		else
		{
			//dates are OK, break off while loop and continue
			start_year_month_in_days = date_to_int(start_date.year, start_date.month, 0);	//create also the date in days of the current year+month
			end_year_month_in_days = date_to_int(end_date.year, end_date.month, 0);			//create also the date in days of the current çyear+month
			dates_valid = 1;
		}
	} while (dates_valid == 0);

	//set flag in case nothing was found
	int found = 0;

	//browse through years, months and days
	
	st_year* p_last_year_before_range = NULL;
	while (p_year != NULL)
	{
		//create backup for year
		st_year* backup_pl_next_year = p_year->pl_next_year;

		if ((start_date.year <= p_year->year) && (p_year->year <= end_date.year))
		{
			st_month* p_month = p_year->pl_month;
			st_month* p_last_month_before_range = NULL;
			while (p_month != NULL)
			{
				//create backup for month
				st_month* backup_pl_next_month = p_month->pl_next_month;
				int current_year_month_in_days = date_to_int(p_year->year, p_month->month, 0);

				if ((start_year_month_in_days <= current_year_month_in_days) && (current_year_month_in_days <= end_year_month_in_days))
				//if ((start_date.month <= p_month->month) && (p_month->month <= end_date.month))
				{
					st_day* p_day = p_month->pl_day;
					st_day* p_last_day_before_range = NULL;
					while (p_day != NULL)
					{
						//create backup for day
						st_day* backup_pl_next_day = p_day->pl_next_day;
						int current_year_month_day_in_days = current_year_month_in_days + p_day->day;
						if ((start_date_in_days <= current_year_month_day_in_days) && (current_year_month_day_in_days <= end_date_in_days))
						//if ((start_date.day <= p_day->day) && (p_day->day <= end_date.day))
						{
							st_appointment* p_appointment = p_day->pl_appointment;
							while (p_appointment != NULL)
							{
								
								//if there are more appointments left, free the first struct and go to next
								if (p_appointment->pl_next_appointment != NULL)
								{
									st_appointment* backup_pl_next_appointment = p_appointment->pl_next_appointment;

									//TODO: FREE MALLOC STRINGS

#ifdef USE_MALLOCS
									//before we free the allocated appointment, free first the allocated strings
									// TODO MAKE THIS A FUNCTION									
									free(p_appointment->p_title);
									free(p_appointment->p_description);
									free(p_appointment->p_location_description);
#endif

									free(p_appointment);
									p_appointment = backup_pl_next_appointment;
								}
		
								else
								{	//when one appointment is left, remove the struct
#ifdef USE_MALLOCS
									//before we free the allocated appointment, free first the allocated strings

									free(p_appointment->p_title);
									free(p_appointment->p_description);
									free(p_appointment->p_location_description);


#endif
									free(p_appointment);
									p_appointment = NULL;
								}


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
	if (found == 0)
	{
		printf("No appointments were found within this range! Nothing has been deleted.\n");
	}
	else
	{
		printf("%d appointments deleted!\n", found);
	}
}


void remove_tree(st_root* p_root, int print_details)
{

	st_year* p_year = p_root->pl_year;
	
	//if tree is already empty
	if ((p_year == NULL) && (print_details != 0))
	{
		printf("Nothing to delete!\n");
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
				

				st_appointment* p_appointment = p_day->pl_appointment;
				while (p_appointment != NULL)
				{
					//if there are more appointments left, free the first struct and go to next
					if (p_appointment->pl_next_appointment != NULL)
					{
						st_appointment* backup_p_appointment = p_appointment->pl_next_appointment;

						//TODO: FREE MALLOC STRINGS 
#ifdef USE_MALLOCS
						//before we free the allocated appointment,free first the allocated strings
						//TODO MAKE THIS A FUNCITON
						free(p_appointment->p_title);
						free(p_appointment->p_description);
						free(p_appointment->p_location_description);
						
						
#endif

						free(p_appointment);
						p_appointment = backup_p_appointment;
					}
					//when one appointment is left, remove the struct
					else
					{
						free(p_appointment);
						p_appointment = NULL;
					}

				}
				//if there are more days left, free the first struct and go to next
				if (p_day->pl_next_day != NULL)
				{
					st_day* backup_p_day = p_day->pl_next_day;
					free(p_day);
					p_day = backup_p_day;
				}
				//when one day is left, remove the struct
				else
				{
					free(p_day);
					p_day = NULL;
				}

				//p_day = p_day->pl_next_day;
			}
			//if there are more months left, free the first struct and go to next
			if (p_month->pl_next_month != NULL)
			{
				st_month* backup_p_month = p_month->pl_next_month;
				free(p_month);
				p_month = backup_p_month;
			}
			//when one month is left, remove the struct
			else
			{
				free(p_month);
				p_month = NULL;
			}

			//p_month = p_month->pl_next_month;
		}
		//if there are more years left, free the first struct and go to next
		if (p_year->pl_next_year != NULL)
		{
			st_year* backup_p_year = p_year->pl_next_year;
			free(p_year);
			p_year = backup_p_year;
		}
		//when one year is left, remove the struct
		else
		{
			free(p_year);
			p_year = NULL;
		}
		//p_year = p_year->pl_next_year;
	}

	//root points to NULL (init root)
	init_root(p_root);

	if (print_details != 0)
	{
		printf("Calendar completely removed!\n");
	}
	
	/*p_root->pl_year = NULL;*/

}

//this function will only be called at the beginning
void init_root(st_root* p_root)
{

	p_root->pl_year = NULL;

}

void init_root_OLD(st_root* p_root)
{

	//st_root *p = malloc(sizeof(st_root));
	//p->pl_year = NULL;

	//Init first year struct
	p_root->pl_year = malloc(sizeof(st_year));
	p_root->pl_year->year = 0;
	p_root->pl_year->pl_next_year = NULL;	//preset next ll_node to NULL.

	//Init first month struct
	p_root->pl_year->pl_month = malloc(sizeof(st_month));
	p_root->pl_year->pl_month->month = 0;
	p_root->pl_year->pl_month->pl_next_month = NULL;	//preset next ll_node to NULL.

	//Init first day struct
	p_root->pl_year->pl_month->pl_day = malloc(sizeof(st_day));
	p_root->pl_year->pl_month->pl_day->day = 0;
	p_root->pl_year->pl_month->pl_day->pl_next_day = NULL;	  //preset next ll_node to NULL.


	//Init first appointment struct
	p_root->pl_year->pl_month->pl_day->pl_appointment = malloc(sizeof(st_appointment));
#if 1
	memset(&p_root->pl_year->pl_month->pl_day->pl_appointment->date,
			0,
			sizeof(st_date));
#else
	p_root->pl_year->pl_month->pl_day->pl_appointment->date.day = 0;
	p_root->pl_year->pl_month->pl_day->pl_appointment->date.month = 0;
	p_root->pl_year->pl_month->pl_day->pl_appointment->date.year = 0;
#endif
	memset(&p_root->pl_year->pl_month->pl_day->pl_appointment->time_start, 0, sizeof(st_time));
	memset(&p_root->pl_year->pl_month->pl_day->pl_appointment->time_end, 0, sizeof(st_time));
	p_root->pl_year->pl_month->pl_day->pl_appointment->pl_next_appointment = NULL;


}


///
///  @brief   This function prints the main menu. The user also inputs the option.
///  @param   void
///  @return  option
/// /
//int write_menu_and_get_option(void)
//{
//	printf("What would you like to do?\n"
//			"[0] Exit Program\n"
//			"[1] Add singular appointment\n"
//			"[2] Remove appointments in a range\n"
//			"[3] Empty Calendar\n"
//			"[4] Write out complete Calendar\n"
//			"[5] Write out appointments in a range\n"
//			"[6] Write out appointments based on textual matching\n"
//			"[7] Import Calendar\n"
//			"[8] Export Calendar\n");
//
//	int choice;
//	if (scanf("%d", &choice) != 1)
//	{
//		//scanf went wrong, return illegal value
//		choice = -1;
//	}
//	//flush all remaining characters for next input
//	char c;
//	do
//	{
//		c = getchar();
//	} while (c != '\n');
//		
//	//while (c != '\n' && c != EOF)
//
//	//c = getchar();
//	//while (c != '\n' && c != EOF) 
//	//{
//	//	c = getchar();
//	//}
//
//
//	//while ((c = getchar()) != '\n' && c != EOF);
//	//getchar();
//
//	return choice;
//	}
//
///**
//* @brief	This function processes the give integer.
//* @param	Choice (int)
//* @return	Processed choice (int)
//*/
//int process_menu_option(st_root* p_root, int choice)
//{
//	switch (choice)
//	{
//	case EXIT_PROGRAM:
//
//		printf("Exiting...");
//
//		return 0;
//
//	case CREATE_APPOINTMENT:
//
//		add_appointment_manually();
//		break;
//
//	case REMOVE_APPOINTMENT_RANGE:
//
//		break;
//
//	case EMPTY_CALENDAR:
//
//		break;
//
//	case DISPLAY_CALENDAR:
//
//		print_appointments_from_tree(p_root);
//
//		break;
//
//	case DISPLAY_CALENDAR_RANGE:
//
//		break;
//
//	case WRITE_CALENDAR_MATCH:
//
//		break;
//
//	case IMPORT_CALENDAR:
//
//		//import_calendar_file("C:\\Users\\ulric\\OneDrive\\Dokumente\\Ulrich Tuts\\Universiteit\\2025 - 2026\\Problem Solving\\Kwartiel 2\\Individueel project\\Kalender\\Kalender_data.txt");
//		import_calendar_file(p_root, "C:\\Program Files Ulrich\\Kalender_data.txt");
//		printf("import done.");
//		break;
//
//	case EXPORT_CALENDAR: 
//
//		break;
//
//	default:
//		printf("Invalid input! Expecting 0-8!\n");
//
//	}
//	
//	return choice;
//
//
//
//}


// TODO: import and export file
// TODO: add appointment function
// TODO: print calendar function
// TODO: remove calendar function
// TODO: what to do with same time appoitnment


