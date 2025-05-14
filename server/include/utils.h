#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <stdbool.h>
#include <winsock2.h>
#include "../shared/constants.h"

#define BUF_SIZE 1024
#define MAX_USERS 100
#define MAX_BOOKS 1000
#define MAX_CLNT 256

// 에러 처리 함수
void ErrorHandling(const char* message);

// 문자열 처리 함수
char* trim(char* str);

// 입력 검증 함수
bool is_valid_number(const char* str);
bool is_valid_rating(float rating);

// 파일 처리 함수
bool save_to_file(const char* filename, const char* data);
char* read_from_file(const char* filename);

// 메시지 처리 함수
char* create_message(const char* command, const char* data);
void parse_message(const char* msg, char* command, char* data);

#endif 