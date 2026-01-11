#pragma once
#ifndef UTILS_H
#define UTILS_H



//function prototypes
void* malloc_s(size_t size);
void fgets_s(char* p_destination, int max_size, FILE* stream, int mandatory_input, char* p_message);
void flush_keyboard_input(void);
char* my_strtok(char* str, const char* delimiter);
void string_to_lower(char* string);


#endif 