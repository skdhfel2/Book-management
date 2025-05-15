#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include <direct.h>  // mkdir on Windows
#include "../include/constants.h"
#include "../include/protocol.h"
#include "../include/utils.h"
#include "../include/client_core.h"

extern SOCKET sock;

void save_to_history(const char* cmd) {
    _mkdir("history"); // Windows용 mkdir (이미 있으면 무시)
    FILE* file = fopen("history/history.txt", "a");
    if (!file) return;

    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char time_buf[32];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(file, "%s > %s\n", time_buf, cmd);
    fclose(file);
}

void send_and_log(const char* msg) {
    send(sock, msg, strlen(msg), 0);
    save_to_history(msg);
}

void send_login_command(const char* id, const char* pw) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//LOGIN//%s//%s", id, pw);
    printf("[DEBUG] Sending login command: %s\n", msg);  // 디버그 메시지 추가
    send_and_log(msg);
}

void send_logout_command() {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s", CMD_LOGOUT);
    send_and_log(msg);
}

void send_quit_command() {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s", CMD_QUIT);
    send_and_log(msg);
}

void send_add_user_command(const char* id, const char* pw) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s//%s//%s", CMD_ADD_USER, id, pw);
    send_and_log(msg);
}

void send_delete_user_command(const char* id) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s//%s", CMD_DELETE_USER, id);
    send_and_log(msg);
}

void send_update_user_command(const char* id, const char* new_pw) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s//%s//%s", CMD_UPDATE_USER, id, new_pw);
    send_and_log(msg);
}

void send_search_book_command(const char* full_cmd) {
    send_and_log(full_cmd);  // keyword에 완성된 명령어가 들어온다고 가정
}

void send_add_book_command(const char* title, const char* author, float rating) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s//%s//%s//%.2f", CMD_ADD_BOOK, title, author, rating);
    send_and_log(msg);
}

void send_delete_book_command(int id) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s//%d", CMD_DELETE_BOOK, id);
    send_and_log(msg);
}

void send_update_book_command(int id, const char* title, const char* author, float rating) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s//%d//%s//%s//%.2f", CMD_UPDATE_BOOK, id, title, author, rating);
    send_and_log(msg);
}

void send_rank_book_command(const char* mode) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s//%s", CMD_RANK_BOOK, mode);
    send_and_log(msg);
}

void send_custom_command(const char* cmd) {
    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "CL//%s", cmd);
    send_and_log(msg);
}