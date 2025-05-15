#ifndef BOOK_MANAGEMENT_H
#define BOOK_MANAGEMENT_H

#include <winsock2.h>

int search_book(SOCKET sock, const char* title_kw, const char* author_kw, const char* id_kw);  
int add_book(const char* title, const char* author, float rating);
int delete_book(int book_id);
int update_book(int book_id, const char* new_title, const char* new_author, float new_rating);
int send_book_rank(SOCKET sock);

#endif
