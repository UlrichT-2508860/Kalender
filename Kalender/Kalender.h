#pragma once
#ifndef KALENDER_H
#define KALENDER_H


//define constants
#define MAX_TITLE_LENGTH (100 + 1)  //one extra character for holding the nullbyte
#define MAX_DESCRIPTION_LENGTH (300 + 1)
#define MAX_LOCATION_LENGTH (100 + 1)
#define DATE_STR_LENGTH (10)	//import file: character size of date.
#define TIME_STR_LENGTH (5)		//import file: character size of time.
#define ID_STR_LENGTH	(16)	//import file: character size of unique ID

#define FILE_DELIMITER ";"

#define USE_MALLOCS	//to use mem-allocated string messages in the appointment

//initialize the structs

typedef struct st_root
{
	struct st_year* pl_year;

} st_root;

typedef struct st_year
{
	int year;

	struct st_month* pl_month;
	struct st_year* pl_next_year;

} st_year;

typedef struct st_month
{
	int month;

	struct st_day* pl_day;
	struct st_month* pl_next_month;

} st_month;

typedef struct st_day
{
	int day;

	struct st_appointment* pl_appointment;
	struct st_day* pl_next_day;

} st_day;

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

typedef struct st_appointment
{
	int id; 
	st_date date;
	st_time time_start;
	st_time time_end;

	
#ifdef USE_MALLOCS
	char *p_title;
	char *p_description;
	char *p_location_description;

#else
	char title[MAX_TITLE_LENGTH];
	char description[MAX_DESCRIPTION_LENGTH];
	char location_description[MAX_LOCATION_LENGTH];
#endif
	struct st_appointment* pl_next_appointment;
} st_appointment;

//initialize enums
enum menu_choices
{
	EXIT_PROGRAM = 0,
	CREATE_APPOINTMENT = 1,
	REMOVE_APPOINTMENT_RANGE = 2,
	EMPTY_CALENDAR = 3,
	DISPLAY_CALENDAR = 4,
	DISPLAY_CALENDAR_RANGE = 5,
	WRITE_CALENDAR_MATCH = 6,
	IMPORT_CALENDAR = 7,
	EXPORT_CALENDAR = 8
};

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

enum error_codes
{
	IMPORT_ERROR_TITLE = 1,
	IMPORT_ERROR_DESCRIPTION,
	IMPORT_ERROR_LOCATION,
	IMPORT_ERROR_DATE,
	IMPORT_ERROR_START_TIME,
	IMPORT_ERROR_END_TIME,
	IMPORT_ERROR_ID
};

//function prototypes

int generate_id(void);
int get_max_days_of_month(int year, int month);
int date_to_int(int year, int month, int day);
int is_date_valid(st_date* p_date);
int is_time_valid(st_time* p_time);
int is_end_time_valid_to_start_time(st_time* p_end_time, st_time* p_start_time);

void init_root(st_root* p_root);
st_year* get_or_create_year(st_root* p_this_root, int year);
st_month* get_or_create_month(st_year* p_this_year, int month);
st_day* get_or_create_day(st_month* p_this_month, int day);
st_appointment* get_and_create_appointment(st_day* p_this_day, st_time* p_time_start);
void add_appointment_to_tree(st_root* p_root, st_appointment* p_new_appointment);
void print_appointment_details(st_appointment* p_appointment);
void print_appointments_with_match(st_root* p_root); //TODO:  FIX THIS WEIRD WARNING

void user_request_time(st_time* p_time, char* p_message);
void user_request_time_range(st_date* p_start_time, st_date* p_end_time);
void user_request_date(st_date* p_date, char* p_message);
void user_request_date_range(	st_date* p_start_date,
								st_date* p_end_date,
								int* p_start_date_in_days,
								int* p_start_year_month_in_days,
								int* p_end_date_in_days,
								int* p_end_year_month_in_days);


void print_appointments_in_range_or_all(st_root* p_root, int print_all);
//int write_menu_and_get_option(void);
//int process_menu_option(st_root* p_root, int choice);

void remove_appointments_in_range_or_all(st_root* p_root, int remove_all, int print_details);
void import_calendar_file(st_root* p_root, char* filename);
void export_calendar_file(st_root* p_root, char* filename);
void add_appointment_manually(st_root* p_root);

#endif
