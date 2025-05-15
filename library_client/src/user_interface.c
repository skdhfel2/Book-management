#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/utils.h"
#include "../include/user_interface.h"

void get_user_input(char* id, char* pw) {
    printf("ID 입력: ");
    fgets(id, MAX_ID_LEN, stdin);
    trim_newline(id);

    printf("비밀번호 입력: ");
    fgets(pw, MAX_PW_LEN, stdin);
    trim_newline(pw);
}

void get_book_input(char* title, char* author, float* rating) {
    char buffer[32];

    printf("도서 제목: ");
    fgets(title, MAX_TITLE_LEN, stdin);
    trim_newline(title);

    printf("저자 이름: ");
    fgets(author, MAX_AUTHOR_LEN, stdin);
    trim_newline(author);

    printf("평점(0.0 ~ 5.0): ");
    fgets(buffer, sizeof(buffer), stdin);
    *rating = atof(buffer);
}

void get_book_id_input(int* id) {
    char buffer[16];
    printf("도서 순번(ID): ");
    fgets(buffer, sizeof(buffer), stdin);
    *id = atoi(buffer);
}
