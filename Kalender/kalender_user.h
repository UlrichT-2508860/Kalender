#pragma once
#ifndef KALENDER_USER_H
#define KALENDER_USER_H
void user_request_time(st_time* p_time, char* p_message);
void user_request_time_range(st_time* p_start_time, st_time* p_end_time);
void user_request_date(st_date* p_date, char* p_message);
void user_request_date_range(st_date* p_start_date,
								st_date* p_end_date,
								int* p_start_date_in_days,
								int* p_start_year_month_in_days,
								int* p_end_date_in_days,
								int* p_end_year_month_in_days);
void add_appointment_manually(st_root* p_root);
void import_calendar_file(st_root* p_root, char* filename);
void export_calendar_file(st_root* p_root, char* filename);
#endif