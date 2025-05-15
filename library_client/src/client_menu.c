#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../include/constants.h"     
#include "../include/client_menu.h"
#include "../include/user_interface.h"
#include "../include/utils.h"
#include "../include/client_core.h"

extern int is_logged_in;
extern SOCKET sock;  // sock 변수 선언 추가

void show_login_menu() {
    while (!is_logged_in) {  // 로그인 성공할 때까지 반복
        char id[MAX_ID_LEN];
        char pw[MAX_PW_LEN];

        printf("\n==== 로그인 ====\n");
        printf("ID 입력: ");
        fgets(id, MAX_ID_LEN, stdin);
        trim_newline(id);

        printf("비밀번호 입력: ");
        fgets(pw, MAX_PW_LEN, stdin);
        trim_newline(pw);

        send_login_command(id, pw);
        
        // 서버 응답을 기다리기 위해 잠시 대기
        Sleep(100);
    }
}

void show_main_menu() {
    if (!is_logged_in) {
        show_login_menu();
        return;
    }

    while (is_logged_in) {
        printf("\n==== 메인 메뉴 ====\n");
        printf("1. 도서 관리\n");
        printf("2. 사용자 관리\n");
        printf("3. 로그아웃\n");
        printf("4. 종료\n");
        printf("선택: ");

        char input[8];
        fgets(input, sizeof(input), stdin);
        trim_newline(input);  // 개행 문자 제거

        switch (atoi(input)) {
            case 1: 
                if (is_logged_in) show_book_menu(); 
                break;
            case 2: 
                if (is_logged_in) show_user_menu(); 
                break;
            case 3: 
                if (is_logged_in) {
                    send_logout_command();
                    is_logged_in = 0;
                    return;
                }
                break;
            case 4: 
                send_quit_command();
                exit(0);
            default: 
                print_error("잘못된 선택입니다. 1~4 중 선택하세요.");
                break;
        }
    }
}

void show_book_menu() {
    while (1) {
        printf("\n==== 도서 관리 메뉴 ====\n");
        printf("1. 도서 검색\n");
        printf("2. 도서 추가\n");
        printf("3. 도서 삭제\n");
        printf("4. 도서 수정\n");
        printf("5. 도서 랭킹 조회\n");
        printf("0. 뒤로가기\n");
        printf("선택: ");

        char input[8];
        char keyword[MAX_TITLE_LEN], title[MAX_TITLE_LEN], author[MAX_AUTHOR_LEN];
        float rating;
        int id;

        if (fgets(input, sizeof(input), stdin) == NULL) {
            print_error("입력 오류가 발생했습니다.");
            continue;
        }
        trim_newline(input);

        int choice = atoi(input);
        if (choice == 0) {
            return;
        }

        switch (choice) {
            case 1: {
                    printf("\n==== 도서 검색 ====\n");
                    printf("검색 조건을 입력하세요 (빈칸은 엔터)\n");
                    
                    char title[MAX_TITLE_LEN] = {0};
                    char author[MAX_AUTHOR_LEN] = {0};
                    char id_str[8] = {0};
                    
                    printf("제목: ");
                    fgets(title, MAX_TITLE_LEN, stdin);
                    trim_newline(title);
                    
                    printf("저자: ");
                    fgets(author, MAX_AUTHOR_LEN, stdin);
                    trim_newline(author);
                    
                    printf("도서 번호: ");
                    fgets(id_str, sizeof(id_str), stdin);
                    trim_newline(id_str);
                    
                    char full_cmd[BUF_SIZE];
                    sprintf(full_cmd, "CL//SEARCH//%s//%s//%s", title, author, id_str);
                    printf("\n[검색 결과]\n");
                    printf("번호 / 제목 / 저자 / 평점\n");
                    printf("------------------------\n");
                    send_search_book_command(full_cmd);
                    Sleep(100);  // 서버 응답 대기
                    break;
                }
            case 2:
                get_book_input(title, author, &rating);
                send_add_book_command(title, author, rating);
                break;
            case 3:
                get_book_id_input(&id);
                send_delete_book_command(id);
                break;
            case 4:
                get_book_id_input(&id);
                get_book_input(title, author, &rating);
                send_update_book_command(id, title, author, rating);
                break;
            case 5:
                printf("\n[도서 랭킹]\n");
                printf("순위 / 제목 / 저자 / 평점\n");
                printf("------------------------\n");
                send_rank_book_command("TOP10");
                Sleep(100);  // 서버 응답 대기
                break;
            default:
                print_error("잘못된 선택입니다. 0~5 중 선택하세요.");
                break;
        }
    }
}

void show_user_menu() {
    while (1) {
        printf("\n==== 사용자 관리 메뉴 ====\n");
        printf("1. 사용자 추가\n");
        printf("2. 사용자 삭제\n");
        printf("3. 비밀번호 수정\n");
        printf("0. 뒤로가기\n");
        printf("선택: ");

        char input[8];
        char id[MAX_ID_LEN], pw[MAX_PW_LEN];

        fgets(input, sizeof(input), stdin);
        switch (atoi(input)) {
            case 1:
                get_user_input(id, pw);
                send_add_user_command(id, pw);
                break;
            case 2:
                printf("삭제할 ID: ");
                fgets(id, sizeof(id), stdin);
                trim_newline(id);
                send_delete_user_command(id);
                break;
            case 3:
                printf("수정할 ID: ");
                fgets(id, sizeof(id), stdin);
                trim_newline(id);
                printf("새 비밀번호: ");
                fgets(pw, sizeof(pw), stdin);
                trim_newline(pw);
                send_update_user_command(id, pw);
                break;
            case 0:
                return;
            default:
                print_error("잘못된 선택입니다."); break;
        }
    }
}
