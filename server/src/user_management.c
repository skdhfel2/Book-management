#include "../include/user_management.h"
#include <ctype.h>

static User users[MAX_USERS];
static int user_count = 0;
static HANDLE user_mutex;

int init_user_management(void) {
    user_mutex = CreateMutex(NULL, FALSE, NULL);
    if(user_mutex == NULL) return 0;
    
    // 파일이 없으면 새로 생성
    if(!load_users_from_file()) {
        // 기본 관리자 계정 생성
        strcpy(users[0].id, "admin");
        strcpy(users[0].password, "admin");
        users[0].is_admin = 1;
        user_count = 1;
        save_users_to_file();
    }
    return 1;
}

int add_user(const char* id, const char* password) {
    WaitForSingleObject(user_mutex, INFINITE);
    
    // ID 중복 체크
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].id, id) == 0) {
            ReleaseMutex(user_mutex);
            return 0;
        }
    }
    
    // 최대 사용자 수 체크
    if(user_count >= MAX_USERS) {
        ReleaseMutex(user_mutex);
        return 0;
    }
    
    // 새 사용자 추가
    strcpy(users[user_count].id, id);
    strcpy(users[user_count].password, password);
    users[user_count].is_admin = 0;
    user_count++;
    
    // 파일에 저장
    save_users_to_file();
    
    ReleaseMutex(user_mutex);
    return 1;
}

int delete_user(const char* id) {
    WaitForSingleObject(user_mutex, INFINITE);
    
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].id, id) == 0) {
            // 마지막 사용자를 현재 위치로 이동
            if(i < user_count - 1) {
                users[i] = users[user_count - 1];
            }
            user_count--;
            save_users_to_file();
            ReleaseMutex(user_mutex);
            return 1;
        }
    }
    
    ReleaseMutex(user_mutex);
    return 0;
}

int update_user(const char* id, const char* new_password) {
    WaitForSingleObject(user_mutex, INFINITE);
    
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].id, id) == 0) {
            strcpy(users[i].password, new_password);
            save_users_to_file();
            ReleaseMutex(user_mutex);
            return 1;
        }
    }
    
    ReleaseMutex(user_mutex);
    return 0;
}

int authenticate_user(const char* id, const char* password) {
    WaitForSingleObject(user_mutex, INFINITE);
    
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].id, id) == 0 && 
           strcmp(users[i].password, password) == 0) {
            ReleaseMutex(user_mutex);
            return 1;
        }
    }
    
    ReleaseMutex(user_mutex);
    return 0;
}

int is_admin_user(const char* id) {
    WaitForSingleObject(user_mutex, INFINITE);
    
    for(int i = 0; i < user_count; i++) {
        if(strcmp(users[i].id, id) == 0) {
            int is_admin = users[i].is_admin;
            ReleaseMutex(user_mutex);
            return is_admin;
        }
    }
    
    ReleaseMutex(user_mutex);
    return 0;
}

void cleanup_user_management(void) {
    if(user_mutex != NULL) {
        CloseHandle(user_mutex);
    }
}

int save_users_to_file(void) {
    char buffer[BUF_SIZE] = "";
    char temp[BUF_SIZE];
    
    for(int i = 0; i < user_count; i++) {
        snprintf(temp, BUF_SIZE, "%s,%s,%d\n", 
                users[i].id, users[i].password, users[i].is_admin);
        strcat(buffer, temp);
    }
    
    return save_to_file("data/users.txt", buffer);
}

int load_users_from_file(void) {
    char* data = read_from_file("data/users.txt");
    if(data == NULL) return 0;
    
    char* line = strtok(data, "\n");
    user_count = 0;
    
    while(line != NULL && user_count < MAX_USERS) {
        char* id = strtok(line, ",");
        char* password = strtok(NULL, ",");
        char* is_admin = strtok(NULL, ",");
        
        if(id && password && is_admin) {
            strcpy(users[user_count].id, id);
            strcpy(users[user_count].password, password);
            users[user_count].is_admin = atoi(is_admin);
            user_count++;
        }
        
        line = strtok(NULL, "\n");
    }
    
    free(data);
    return 1;
}

// 핸들러 함수 구현
char* handle_login(const char* data) {
    char id[50], password[50];
    if(sscanf(data, "%[^|]|%s", id, password) != 2) {
        return strdup("ERROR|잘못된 로그인 형식입니다.");
    }
    
    if(authenticate_user(id, password)) {
        return strdup("SUCCESS|로그인 성공");
    }
    return strdup("ERROR|아이디 또는 비밀번호가 일치하지 않습니다.");
}

char* handle_logout(SOCKET clientSock) {
    return strdup("SUCCESS|로그아웃 성공");
}

char* handle_add_user(const char* data) {
    char id[50], password[50];
    if(sscanf(data, "%[^|]|%s", id, password) != 2) {
        return strdup("ERROR|잘못된 사용자 추가 형식입니다.");
    }
    
    if(add_user(id, password)) {
        return strdup("SUCCESS|사용자 추가 성공");
    }
    return strdup("ERROR|사용자 추가 실패");
}

char* handle_delete_user(const char* data) {
    if(delete_user(data)) {
        return strdup("SUCCESS|사용자 삭제 성공");
    }
    return strdup("ERROR|사용자 삭제 실패");
}

char* handle_update_user(const char* data) {
    char id[50], new_password[50];
    if(sscanf(data, "%[^|]|%s", id, new_password) != 2) {
        return strdup("ERROR|잘못된 사용자 수정 형식입니다.");
    }
    
    if(update_user(id, new_password)) {
        return strdup("SUCCESS|사용자 수정 성공");
    }
    return strdup("ERROR|사용자 수정 실패");
}

char* handle_search_user(const char* data) {
    // 사용자 검색 구현
    return strdup("SUCCESS|사용자 검색 결과");
}

char* handle_list_users(void) {
    // 사용자 목록 조회 구현
    return strdup("SUCCESS|사용자 목록");
} 