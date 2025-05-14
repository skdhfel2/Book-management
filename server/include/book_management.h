#ifndef BOOK_MANAGEMENT_H
#define BOOK_MANAGEMENT_H

#include <stdbool.h>
#include "../shared/constants.h"

// 도서 구조체 정의
typedef struct {
    int id;
    char title[100];
    char author[100];
    char publisher[100];
    char isbn[20];
    float rating;
} Book;

// 도서 관리 함수
bool init_book_management(void);
void cleanup_book_management(void);
bool add_book(const char* title, const char* author, const char* publisher, const char* isbn, float rating);
bool delete_book(int id);
bool update_book(int id, const char* title, const char* author, const char* publisher, const char* isbn, float rating);
Book* search_book(int id);
Book* search_book_by_title(const char* title);
Book* search_book_by_author(const char* author);
Book* get_all_books(int* count);
Book* get_top_rated_books(int count);
void free_book(Book* book);
void free_book_array(Book* books);

// 도서 데이터 파일 처리
int save_books_to_file(void);
int load_books_from_file(void);

// 핸들러 함수
char* handle_add_book(const char* data);
char* handle_delete_book(const char* data);
char* handle_update_book(const char* data);
char* handle_search_book(const char* data);
char* handle_list_books(void);

#endif 