#include "utils_date_time.h"




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
		((year - 1) / 4) +					//add leap days for all leap years that happened before this year.
		day;								//add day in last month.

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
	int max_days = get_max_days_of_month(p_date->year, p_date->month);
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