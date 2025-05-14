#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <winsock2.h>
#include "../shared/constants.h"
#include "../shared/protocol.h"
#include <windows.h>
#include <process.h>
#include <stdbool.h>
#include <time.h>

#define BUF_SIZE 1024
#define MAX_HISTORY 100
#define HEARTBEAT_INTERVAL 10

// 명령어 히스토리 구조체
typedef struct {
    char command[BUF_SIZE];
    char data[BUF_SIZE];
    time_t timestamp;
} CommandHistory;

// 소켓 초기화 및 정리
bool init_client_socket(void);
void cleanup_client_socket(void);

// 메시지 송수신
bool send_message(const char* command, const char* data);
char* receive_message(void);

// 입력 처리
char* get_input(const char* prompt);
bool validate_input(const char* input, const char* type);

// 히스토리 관리
void add_to_history(const char* command, const char* data);
void show_history(void);
void clear_history(void);
char* get_last_command(void);

// 문자열 처리
char* trim(const char* str);

// 유틸리티 함수
void print_error(const char* msg);
void print_success(const char* msg);

#endif // CLIENT_UTILS_H 