#pragma once
#ifndef UTILS_DATE_TIME_H
#define UTILS_DATE_TIME_H

//structs
typedef struct st_date
{
	int year;
	int month;
	int day;

} st_date;

typedef struct st_time
{
	int hour;
	int minute;

} st_time;


//enum
enum months
{
	JAN = 1,
	FEB = 2,
	MAR = 3,
	APR = 4,
	MAY = 5,
	JUN = 6,
	JUL = 7,
	AUG = 8,
	SEP = 9,
	OCT = 10,
	NOV = 11,
	DEC = 12
};


int get_max_days_of_month(int year, int month);
int date_to_int(int year, int month, int day);
int is_date_valid(st_date* p_date);
int is_time_valid(st_time* p_time);
int is_end_time_valid_to_start_time(st_time* p_end_time, st_time* p_start_time);

#endif
