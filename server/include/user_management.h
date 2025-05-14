#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <stdbool.h>
#include <winsock2.h>
#include "../shared/constants.h"

// 사용자 구조체 정의
typedef struct {
    char id[50];
    char password[50];
    int is_admin;
} User;

// 사용자 관리 함수
bool init_user_management(void);
void cleanup_user_management(void);
bool add_user(const char* id, const char* password);
bool delete_user(const char* id);
bool update_user(const char* id, const char* new_password);
bool authenticate_user(const char* id, const char* password);
bool is_admin_user(const char* id);
bool save_users_to_file(void);
bool load_users_from_file(void);

// 핸들러 함수
char* handle_login(const char* data);
char* handle_logout(SOCKET clientSock);
char* handle_add_user(const char* data);
char* handle_delete_user(const char* data);
char* handle_update_user(const char* data);
char* handle_search_user(const char* data);
char* handle_list_users(void);

#endif 