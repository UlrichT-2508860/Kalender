#pragma once
#ifndef UTILS_H
#define UTILS_H



//function prototypes
void clearscreen(void);
void* malloc_s(size_t size);
void flush_keyboard_input(void);
void user_wait_for_Enter_Press(void);
char getchar_s(void);
char* my_strtok(char* str, const char* delimiter);
void string_to_lower(char* string);
void fgets_s(char* p_destination, int max_size, FILE* stream, int mandatory_input, char* p_message);
void user_request_string(char* p_destination, int max_size, int mandatory_input, char* p_message);
int user_request_confirmation(char* p_message);

#endif 