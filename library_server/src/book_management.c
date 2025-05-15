#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../include/constants.h"
#include "../include/book_management.h"

#define MAX_BOOKS 1000

typedef struct {
    int id;
    char title[128];
    char author[64];
    float rating;
} Book;

extern int load_books_to_memory(Book books[], int max);
extern int save_books_from_memory(Book books[], int count);

int search_book(SOCKET sock, const char* title_kw, const char* author_kw, const char* id_kw) {
    Book books[MAX_BOOKS];
    int count = load_books_to_memory(books, MAX_BOOKS);
    if (count == 0) return 0;

    int id_filter = -1;  // 기본값을 -1로 설정
    if (strlen(id_kw) > 0) {  // id_kw가 비어있지 않을 때만 변환
        id_filter = atoi(id_kw);
    }
    
    int matched = 0;
    char buf[BUF_SIZE];

    for (int i = 0; i < count; i++) {
        int title_match = strlen(title_kw) == 0 || strstr(books[i].title, title_kw);
        int author_match = strlen(author_kw) == 0 || strstr(books[i].author, author_kw);
        int id_match = id_filter == -1 || books[i].id == id_filter;

        if (title_match && author_match && id_match) {
            snprintf(buf, sizeof(buf), "%d\t%s\t%s\t%.2f\n",
                    books[i].id, books[i].title, books[i].author, books[i].rating);
            send(sock, buf, strlen(buf), 0);
            matched = 1;
        }
    }

    return matched;
}


int add_book(const char* title, const char* author, float rating) {
    Book books[MAX_BOOKS];
    int count = load_books_to_memory(books, MAX_BOOKS);
    if (count >= MAX_BOOKS) return 0;

    int new_id = 1;
    for (int i = 0; i < count; i++) {
        if (books[i].id >= new_id) new_id = books[i].id + 1;
    }

    books[count].id = new_id;
    strncpy(books[count].title, title, sizeof(books[count].title));
    strncpy(books[count].author, author, sizeof(books[count].author));
    books[count].rating = rating;
    return save_books_from_memory(books, count + 1);
}

int delete_book(int book_id) {
    Book books[MAX_BOOKS];
    int count = load_books_to_memory(books, MAX_BOOKS);
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (books[i].id == book_id) {
            found = 1;
            for (int j = i; j < count - 1; j++) {
                books[j] = books[j + 1];
            }
            break;
        }
    }
    if (found) return save_books_from_memory(books, count - 1);
    return 0;
}

int update_book(int book_id, const char* new_title, const char* new_author, float new_rating) {
    Book books[MAX_BOOKS];
    int count = load_books_to_memory(books, MAX_BOOKS);
    for (int i = 0; i < count; i++) {
        if (books[i].id == book_id) {
            strncpy(books[i].title, new_title, sizeof(books[i].title));
            strncpy(books[i].author, new_author, sizeof(books[i].author));
            books[i].rating = new_rating;
            return save_books_from_memory(books, count);
        }
    }
    return 0;
}

int send_book_rank(SOCKET sock) {
    Book books[MAX_BOOKS];
    int count = load_books_to_memory(books, MAX_BOOKS);
    if (count == 0) return 0;

    // 간단한 버블 정렬 (평점 내림차순)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (books[j].rating < books[j + 1].rating) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }

    char buf[BUF_SIZE];
    for (int i = 0; i < count && i < 10; i++) {
        snprintf(buf, sizeof(buf), "%d\t%s\t%s\t%.2f\n",
              books[i].id, books[i].title, books[i].author, books[i].rating);
        send(sock, buf, strlen(buf), 0);
    }
    return 1;
}