#ifndef CLIENT_MENU_H
#define CLIENT_MENU_H

#include "utils.h"

// 메뉴 상태
typedef enum {
    MENU_LOGIN,
    MENU_MAIN,
    MENU_BOOK_MANAGEMENT,
    MENU_USER_MANAGEMENT
} MenuState;

// 메뉴 표시 함수
void show_main_menu(void);
void show_user_menu(void);
void show_book_menu(void);
void show_history(void);

// 메뉴 처리 함수
void handle_user_menu(void);
void handle_book_menu(void);

// 관리자 메뉴
void show_admin_menu(void);
void show_book_management_menu(void);
void show_user_management_menu(void);
void handle_admin_menu(int choice);

// 도서 관리 기능
void search_book_menu(void);
void add_book_menu(void);
void delete_book_menu(void);
void update_book_menu(void);
void show_book_ranking_menu(void);

// 사용자 관리 기능
void add_user_menu(void);
void delete_user_menu(void);
void update_user_menu(void);

// 유틸리티 함수
void clear_screen(void);
void print_error(const char* msg);
void print_success(const char* msg);
void print_table_header(const char* headers[], int count);
void print_table_row(const char* values[], int count);

#endif 