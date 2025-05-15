#ifndef FILE_IO_H
#define FILE_IO_H

#include "constants.h"

// 사용자 정보를 메모리로 로드
int load_users_to_memory(char users[][2][MAX_USER_LEN], int max_users);

// 메모리의 사용자 정보를 파일에 저장
int save_users_from_memory(char users[][2][MAX_USER_LEN], int user_count);

#endif // FILE_IO_H 