#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55555

static SOCKET client_socket;
static CommandHistory history[MAX_HISTORY];
static int history_count = 0;
static int history_index = 0;
static HANDLE heartbeat_thread;

// 에러 메시지 정의
static const char* ERROR_MESSAGES[] = {
    "서버 연결에 실패했습니다.",
    "메모리 할당에 실패했습니다.",
    "잘못된 입력 형식입니다.",
    "서버와의 통신에 실패했습니다.",
    "권한이 없습니다.",
    "해당 항목을 찾을 수 없습니다.",
    "이미 존재하는 항목입니다.",
    "시스템 오류가 발생했습니다."
};

void print_error(const char* msg) {
    printf("\n[오류] %s\n", msg);
}

void print_success(const char* msg) {
    printf("\n[성공] %s\n", msg);
}

// 하트비트 전송 스레드
unsigned WINAPI SendHeartbeat(void* arg) {
    while(1) {
        send_message("HEARTBEAT", "");
        Sleep(HEARTBEAT_INTERVAL * 1000);  // 10초마다 하트비트 전송
    }
    return 0;
}

bool init_client_socket(void) {
    WSADATA wsaData;
    SOCKADDR_IN serverAddr;
    
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        print_error("WSAStartup() 실패");
        return false;
    }
    
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(client_socket == INVALID_SOCKET) {
        print_error("소켓 생성 실패");
        WSACleanup();
        return false;
    }
    
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);
    
    printf("서버에 연결을 시도합니다... (IP: %s, Port: %d)\n", SERVER_IP, SERVER_PORT);
    
    if(connect(client_socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        print_error("서버 연결 실패");
        closesocket(client_socket);
        WSACleanup();
        return false;
    }
    
    printf("서버에 연결되었습니다.\n");
    
    // 하트비트 스레드 시작
    heartbeat_thread = (HANDLE)_beginthreadex(NULL, 0, SendHeartbeat, NULL, 0, NULL);
    if(heartbeat_thread == NULL) {
        print_error("하트비트 스레드 생성 실패");
        closesocket(client_socket);
        WSACleanup();
        return false;
    }
    
    return true;
}

void cleanup_client_socket(void) {
    if(heartbeat_thread != NULL) {
        TerminateThread(heartbeat_thread, 0);
        CloseHandle(heartbeat_thread);
    }
    
    if(client_socket != INVALID_SOCKET) {
        closesocket(client_socket);
    }
    WSACleanup();
}

bool send_message(const char* command, const char* data) {
    char buffer[BUF_SIZE];
    snprintf(buffer, BUF_SIZE, "%s|%s", command, data);
    
    if(send(client_socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        print_error("메시지 전송 실패");
        return false;
    }
    
    // 명령어 히스토리에 추가
    add_to_history(command, data);
    return true;
}

char* receive_message(void) {
    char buffer[BUF_SIZE];
    int bytes_received = recv(client_socket, buffer, BUF_SIZE - 1, 0);
    
    if(bytes_received == SOCKET_ERROR) {
        print_error("메시지 수신 실패");
        return NULL;
    }
    
    if(bytes_received == 0) {
        print_error("서버와의 연결이 끊어졌습니다.");
        return NULL;
    }
    
    buffer[bytes_received] = '\0';
    return strdup(buffer);
}

char* create_message(const char* command, const char* data) {
    char* msg = (char*)malloc(BUF_SIZE);
    if(msg == NULL) return NULL;
    
    snprintf(msg, BUF_SIZE, "CL // %s // %s", command, data);
    return msg;
}

void parse_message(const char* msg, char* command, char* data) {
    char* temp = strdup(msg);
    char* token = strtok(temp, "//");
    
    if(token != NULL) {
        token = strtok(NULL, "//");
        if(token != NULL) {
            strcpy(command, trim(token));
            token = strtok(NULL, "//");
            if(token != NULL) {
                strcpy(data, trim(token));
            }
        }
    }
    
    free(temp);
}

char* get_input(const char* prompt) {
    char* input = (char*)malloc(BUF_SIZE);
    if(input == NULL) return NULL;
    
    printf("%s", prompt);
    fgets(input, BUF_SIZE, stdin);
    input[strcspn(input, "\n")] = 0;
    
    return input;
}

int validate_input(const char* input, const char* type) {
    if(input == NULL || *input == '\0') return 0;
    
    if(!strcmp(type, "number")) {
        while(*input) {
            if(!isdigit(*input)) return 0;
            input++;
        }
        return 1;
    }
    else if(!strcmp(type, "rating")) {
        float rating = atof(input);
        return rating >= 0.0f && rating <= 5.0f;
    }
    else if(!strcmp(type, "alphanumeric")) {
        while(*input) {
            if(!isalnum(*input)) return 0;
            input++;
        }
        return 1;
    }
    
    return 0;
}

void add_to_history(const char* command, const char* data) {
    if(history_count >= MAX_HISTORY) {
        // 가장 오래된 명령어 제거
        for(int i = 0; i < history_count - 1; i++) {
            history[i] = history[i + 1];
        }
        history_count--;
    }
    
    CommandHistory* entry = &history[history_count];
    strncpy(entry->command, command, BUF_SIZE - 1);
    strncpy(entry->data, data, BUF_SIZE - 1);
    entry->timestamp = time(NULL);
    
    history_count++;
    history_index = history_count;
}

void show_history(void) {
    printf("\n=== 명령 히스토리 ===\n");
    for(int i = 0; i < history_count; i++) {
        char* time_str = ctime(&history[i].timestamp);
        if(time_str) {
            time_str[strlen(time_str)-1] = '\0';  // 개행문자 제거
            printf("%d. [%s] %s | %s\n", 
                   i + 1, time_str, history[i].command, history[i].data);
        }
    }
    printf("====================\n");
}

void clear_history(void) {
    history_count = 0;
    history_index = 0;
}

char* get_last_command(void) {
    if(history_count == 0) {
        return NULL;
    }
    
    history_index--;
    if(history_index < 0) {
        history_index = 0;
    }
    
    return history[history_index].command;
}

char* trim(const char* str) {
    if(str == NULL) return NULL;
    
    char* result = strdup(str);
    if(result == NULL) return NULL;
    
    // 앞쪽 공백 제거
    char* start = result;
    while(isspace(*start)) start++;
    
    // 뒤쪽 공백 제거
    char* end = result + strlen(result) - 1;
    while(end > start && isspace(*end)) {
        *end = '\0';
        end--;
    }
    
    // 앞쪽 공백이 있었다면 문자열 이동
    if(start != result) {
        memmove(result, start, strlen(start) + 1);
    }
    
    return result;
} 