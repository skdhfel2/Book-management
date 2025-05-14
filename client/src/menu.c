#include "../include/menu.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_main_menu(void) {
    printf("\n=== 도서관 관리 시스템 ===\n");
    printf("1. 사용자 메뉴\n");
    printf("2. 도서 메뉴\n");
    printf("3. 명령어 히스토리\n");
    printf("0. 종료\n");
    printf("=======================\n");
}

void show_user_menu(void) {
    printf("\n=== 사용자 메뉴 ===\n");
    printf("1. 로그인\n");
    printf("2. 회원가입\n");
    printf("3. 비밀번호 변경\n");
    printf("4. 회원 탈퇴\n");
    printf("0. 이전 메뉴\n");
    printf("=================\n");
    
    int choice;
    printf("선택: ");
    scanf("%d", &choice);
    getchar();
    
    handle_user_menu(choice);
}

void show_book_menu(void) {
    printf("\n=== 도서 메뉴 ===\n");
    printf("1. 도서 검색\n");
    printf("2. 도서 목록 조회\n");
    printf("3. 인기 도서 조회\n");
    printf("0. 이전 메뉴\n");
    printf("================\n");
    
    int choice;
    printf("선택: ");
    scanf("%d", &choice);
    getchar();
    
    handle_book_menu(choice);
}

void show_admin_menu(void) {
    printf("\n=== 관리자 메뉴 ===\n");
    printf("1. 도서 관리\n");
    printf("2. 회원 관리\n");
    printf("3. 로그아웃\n");
    printf("==================\n");
    printf("선택: ");
}

void show_book_management_menu(void) {
    printf("\n=== 도서 관리 메뉴 ===\n");
    printf("1. 도서 추가\n");
    printf("2. 도서 수정\n");
    printf("3. 도서 삭제\n");
    printf("4. 도서 검색\n");
    printf("5. 도서 목록 조회\n");
    printf("6. 이전 메뉴로\n");
    printf("====================\n");
    printf("선택: ");
}

void show_user_management_menu(void) {
    printf("\n=== 회원 관리 메뉴 ===\n");
    printf("1. 회원 추가\n");
    printf("2. 회원 정보 수정\n");
    printf("3. 회원 삭제\n");
    printf("4. 회원 검색\n");
    printf("5. 회원 목록 조회\n");
    printf("6. 이전 메뉴로\n");
    printf("====================\n");
    printf("선택: ");
}

void show_loan_management_menu(void) {
    printf("\n=== 대출 관리 메뉴 ===\n");
    printf("1. 대출 승인\n");
    printf("2. 반납 처리\n");
    printf("3. 연체 관리\n");
    printf("4. 대출 내역 조회\n");
    printf("5. 이전 메뉴로\n");
    printf("====================\n");
    printf("선택: ");
}

void show_statistics_menu(void) {
    printf("\n=== 통계 조회 메뉴 ===\n");
    printf("1. 인기 도서 순위\n");
    printf("2. 회원별 대출 현황\n");
    printf("3. 도서별 대출 현황\n");
    printf("4. 연체 현황\n");
    printf("5. 이전 메뉴로\n");
    printf("====================\n");
    printf("선택: ");
}

void handle_user_menu(int choice) {
    char* input;
    char* response;
    
    switch(choice) {
        case 1: // 로그인
            input = get_input("ID: ");
            if(input) {
                char* password = get_input("비밀번호: ");
                if(password) {
                    char login_data[BUF_SIZE];
                    snprintf(login_data, BUF_SIZE, "%s|%s", input, password);
                    send_message("LOGIN", login_data);
                    response = receive_message();
                    if(response) {
                        printf("\n%s\n", response);
                        free(response);
                    }
                    free(password);
                }
                free(input);
            }
            break;
            
        case 2: // 회원가입
            input = get_input("ID: ");
            if(input) {
                char* password = get_input("비밀번호: ");
                if(password) {
                    char register_data[BUF_SIZE];
                    snprintf(register_data, BUF_SIZE, "%s|%s", input, password);
                    send_message("ADD_USER", register_data);
                    response = receive_message();
                    if(response) {
                        printf("\n%s\n", response);
                        free(response);
                    }
                    free(password);
                }
                free(input);
            }
            break;
            
        case 3: // 비밀번호 변경
            input = get_input("ID: ");
            if(input) {
                char* new_password = get_input("새 비밀번호: ");
                if(new_password) {
                    char update_data[BUF_SIZE];
                    snprintf(update_data, BUF_SIZE, "%s|%s", input, new_password);
                    send_message("UPDATE_USER", update_data);
                    response = receive_message();
                    if(response) {
                        printf("\n%s\n", response);
                        free(response);
                    }
                    free(new_password);
                }
                free(input);
            }
            break;
            
        case 4: // 회원 탈퇴
            input = get_input("ID: ");
            if(input) {
                send_message("DELETE_USER", input);
                response = receive_message();
                if(response) {
                    printf("\n%s\n", response);
                    free(response);
                }
                free(input);
            }
            break;
            
        case 0: // 이전 메뉴
            return;
            
        default:
            printf("잘못된 선택입니다.\n");
    }
}

void handle_book_menu(int choice) {
    char* input;
    char* response;
    
    switch(choice) {
        case 1: // 도서 검색
            input = get_input("검색어: ");
            if(input) {
                send_message("SEARCH_BOOK", input);
                response = receive_message();
                if(response) {
                    printf("\n%s\n", response);
                    free(response);
                }
                free(input);
            }
            break;
            
        case 2: // 도서 목록 조회
            send_message("LIST_BOOKS", "");
            response = receive_message();
            if(response) {
                printf("\n%s\n", response);
                free(response);
            }
            break;
            
        case 3: // 인기 도서 조회
            send_message("RANK", "");
            response = receive_message();
            if(response) {
                printf("\n%s\n", response);
                free(response);
            }
            break;
            
        case 0: // 이전 메뉴
            return;
            
        default:
            printf("잘못된 선택입니다.\n");
    }
}

void handle_admin_menu(int choice) {
    int sub_choice;
    char* input;
    char* response;
    
    switch(choice) {
        case 1: // 도서 관리
            show_book_management_menu();
            scanf("%d", &sub_choice);
            getchar(); // 버퍼 비우기
            
            switch(sub_choice) {
                case 1: // 도서 추가
                    input = get_input("도서 정보를 입력하세요 (제목,저자,출판사,ISBN): ");
                    if(input) {
                        send_message("ADD_BOOK", input);
                        response = receive_message();
                        if(response) {
                            printf("\n%s\n", response);
                            free(response);
                        }
                        free(input);
                    }
                    break;
                    
                case 2: // 도서 수정
                    input = get_input("수정할 도서 정보를 입력하세요 (ID,제목,저자,출판사,ISBN): ");
                    if(input) {
                        send_message("UPDATE_BOOK", input);
                        response = receive_message();
                        if(response) {
                            printf("\n%s\n", response);
                            free(response);
                        }
                        free(input);
                    }
                    break;
                    
                case 3: // 도서 삭제
                    input = get_input("삭제할 도서 ID를 입력하세요: ");
                    if(input) {
                        send_message("DELETE_BOOK", input);
                        response = receive_message();
                        if(response) {
                            printf("\n%s\n", response);
                            free(response);
                        }
                        free(input);
                    }
                    break;
                    
                case 4: // 도서 검색
                    input = get_input("검색어를 입력하세요: ");
                    if(input) {
                        send_message("SEARCH_BOOK", input);
                        response = receive_message();
                        if(response) {
                            printf("\n검색 결과:\n%s\n", response);
                            free(response);
                        }
                        free(input);
                    }
                    break;
                    
                case 5: // 도서 목록 조회
                    send_message("LIST_BOOKS", "");
                    response = receive_message();
                    if(response) {
                        printf("\n도서 목록:\n%s\n", response);
                        free(response);
                    }
                    break;
            }
            break;
            
        case 2: // 회원 관리
            show_user_management_menu();
            scanf("%d", &sub_choice);
            getchar(); // 버퍼 비우기
            
            switch(sub_choice) {
                case 1: // 회원 추가
                    input = get_input("회원 정보를 입력하세요 (ID,이름,전화번호,이메일): ");
                    if(input) {
                        send_message("ADD_USER", input);
                        response = receive_message();
                        if(response) {
                            printf("\n%s\n", response);
                            free(response);
                        }
                        free(input);
                    }
                    break;
                    
                case 2: // 회원 정보 수정
                    input = get_input("수정할 회원 정보를 입력하세요 (ID,이름,전화번호,이메일): ");
                    if(input) {
                        send_message("UPDATE_USER", input);
                        response = receive_message();
                        if(response) {
                            printf("\n%s\n", response);
                            free(response);
                        }
                        free(input);
                    }
                    break;
                    
                case 3: // 회원 삭제
                    input = get_input("삭제할 회원 ID를 입력하세요: ");
                    if(input) {
                        send_message("DELETE_USER", input);
                        response = receive_message();
                        if(response) {
                            printf("\n%s\n", response);
                            free(response);
                        }
                        free(input);
                    }
                    break;
                    
                case 4: // 회원 검색
                    input = get_input("검색어를 입력하세요: ");
                    if(input) {
                        send_message("SEARCH_USER", input);
                        response = receive_message();
                        if(response) {
                            printf("\n검색 결과:\n%s\n", response);
                            free(response);
                        }
                        free(input);
                    }
                    break;
                    
                case 5: // 회원 목록 조회
                    send_message("LIST_USERS", "");
                    response = receive_message();
                    if(response) {
                        printf("\n회원 목록:\n%s\n", response);
                        free(response);
                    }
                    break;
            }
            break;
            
        case 3: // 로그아웃
            send_message("LOGOUT", "");
            response = receive_message();
            if(response) {
                printf("\n%s\n", response);
                free(response);
            }
            break;
            
        default:
            printf("\n잘못된 선택입니다.\n");
    }
} 