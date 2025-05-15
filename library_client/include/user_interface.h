#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

void get_user_input(char* id, char* pw);
void get_book_input(char* title, char* author, float* rating);
void get_book_id_input(int* id);
void print_error(const char* message);
void print_success(const char* message);
void trim_newline(char* str);

#endif