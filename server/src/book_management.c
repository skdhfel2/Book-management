#include "../include/book_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 1000
#define BOOKS_FILE "data/books.dat"

static Book books[MAX_BOOKS];
static int book_count = 0;
static int next_book_id = 1;

bool init_book_management(void) {
    // data 디렉토리 생성
    #ifdef _WIN32
    system("mkdir data 2>nul");
    #else
    system("mkdir -p data");
    #endif

    FILE* file = fopen(BOOKS_FILE, "rb");
    if(file) {
        size_t read_count = fread(books, sizeof(Book), MAX_BOOKS, file);
        if(read_count > 0) {
            book_count = read_count;
            printf("도서 데이터 %d개를 불러왔습니다.\n", book_count);
            
            // 다음 도서 ID 설정
            for(int i = 0; i < book_count; i++) {
                if(books[i].id >= next_book_id) {
                    next_book_id = books[i].id + 1;
                }
            }
            fclose(file);
            return true;
        }
        fclose(file);
    }
    
    // 파일이 없거나 비어있으면 기본 도서 데이터 추가
    printf("기본 도서 데이터를 초기화합니다...\n");
    book_count = 0;
    next_book_id = 1;
    
    // 기본 도서 데이터 추가
    add_book("자바의 정석", "남궁성", "도우출판", "9788994492032", 4.5);
    add_book("이것이 자바다", "신용권", "한빛미디어", "9788968481475", 4.3);
    add_book("자바 프로그래밍", "김철수", "정보문화사", "9788956746425", 4.0);
    add_book("자바 기초부터 실전까지", "이영희", "길벗", "9788966189994", 4.2);
    add_book("자바 완벽 가이드", "박지성", "위키북스", "9788968481482", 4.4);
    
    // 파일에 저장
    file = fopen(BOOKS_FILE, "wb");
    if(file) {
        size_t write_count = fwrite(books, sizeof(Book), book_count, file);
        if(write_count == book_count) {
            printf("기본 도서 데이터 %d개가 저장되었습니다.\n", book_count);
            fclose(file);
            return true;
        }
        fclose(file);
    }
    
    printf("도서 데이터 초기화 중 오류가 발생했습니다.\n");
    return false;
}

void cleanup_book_management(void) {
    FILE* file = fopen(BOOKS_FILE, "wb");
    if(file) {
        size_t write_count = fwrite(books, sizeof(Book), book_count, file);
        if(write_count == book_count) {
            printf("도서 데이터 %d개가 저장되었습니다.\n", book_count);
        } else {
            printf("도서 데이터 저장 중 오류가 발생했습니다.\n");
        }
        fclose(file);
    }
}

bool add_book(const char* title, const char* author, const char* publisher, const char* isbn, float rating) {
    if(book_count >= MAX_BOOKS) {
        return false;
    }
    
    Book* book = &books[book_count];
    book->id = next_book_id++;
    strncpy(book->title, title, sizeof(book->title) - 1);
    strncpy(book->author, author, sizeof(book->author) - 1);
    strncpy(book->publisher, publisher, sizeof(book->publisher) - 1);
    strncpy(book->isbn, isbn, sizeof(book->isbn) - 1);
    book->rating = rating;
    
    book_count++;
    return true;
}

bool delete_book(int id) {
    for(int i = 0; i < book_count; i++) {
        if(books[i].id == id) {
            // 마지막 도서를 현재 위치로 이동
            if(i < book_count - 1) {
                books[i] = books[book_count - 1];
            }
            book_count--;
            return true;
        }
    }
    return false;
}

bool update_book(int id, const char* title, const char* author, const char* publisher, const char* isbn, float rating) {
    for(int i = 0; i < book_count; i++) {
        if(books[i].id == id) {
            strncpy(books[i].title, title, sizeof(books[i].title) - 1);
            strncpy(books[i].author, author, sizeof(books[i].author) - 1);
            strncpy(books[i].publisher, publisher, sizeof(books[i].publisher) - 1);
            strncpy(books[i].isbn, isbn, sizeof(books[i].isbn) - 1);
            books[i].rating = rating;
            return true;
        }
    }
    return false;
}

Book* search_book(int id) {
    for(int i = 0; i < book_count; i++) {
        if(books[i].id == id) {
            Book* book = malloc(sizeof(Book));
            if(book) {
                *book = books[i];
            }
            return book;
        }
    }
    return NULL;
}

Book* search_book_by_title(const char* title) {
    char search_title[100];
    strncpy(search_title, title, sizeof(search_title) - 1);
    search_title[sizeof(search_title) - 1] = '\0';
    
    // 검색어 앞뒤 공백 제거
    char* start = search_title;
    while(isspace(*start)) start++;
    char* end = search_title + strlen(search_title) - 1;
    while(end > start && isspace(*end)) {
        *end = '\0';
        end--;
    }
    if(start != search_title) {
        memmove(search_title, start, strlen(start) + 1);
    }
    
    for(int i = 0; i < book_count; i++) {
        if(strstr(books[i].title, search_title)) {
            Book* book = malloc(sizeof(Book));
            if(book) {
                *book = books[i];
            }
            return book;
        }
    }
    return NULL;
}

Book* search_book_by_author(const char* author) {
    for(int i = 0; i < book_count; i++) {
        if(strstr(books[i].author, author)) {
            Book* book = malloc(sizeof(Book));
            if(book) {
                *book = books[i];
            }
            return book;
        }
    }
    return NULL;
}

Book* get_all_books(int* count) {
    if(book_count == 0) {
        *count = 0;
        return NULL;
    }
    
    Book* book_array = malloc(sizeof(Book) * book_count);
    if(book_array) {
        memcpy(book_array, books, sizeof(Book) * book_count);
        *count = book_count;
    }
    return book_array;
}

Book* get_top_rated_books(int count) {
    if(book_count == 0) {
        return NULL;
    }
    
    // 평점순으로 정렬
    for(int i = 0; i < book_count - 1; i++) {
        for(int j = 0; j < book_count - i - 1; j++) {
            if(books[j].rating < books[j + 1].rating) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
    
    // 요청된 개수만큼 반환
    if(count > book_count) {
        count = book_count;
    }
    
    Book* top_books = malloc(sizeof(Book) * count);
    if(top_books) {
        memcpy(top_books, books, sizeof(Book) * count);
    }
    return top_books;
}

void free_book(Book* book) {
    free(book);
}

void free_book_array(Book* books) {
    free(books);
}

// 핸들러 함수 구현
char* handle_add_book(const char* data) {
    char title[100], author[100], publisher[100], isbn[20];
    float rating;
    
    if(sscanf(data, "%[^|]|%[^|]|%[^|]|%[^|]|%f", 
              title, author, publisher, isbn, &rating) != 5) {
        return strdup("ERROR|잘못된 도서 추가 형식입니다.");
    }
    
    if(add_book(title, author, publisher, isbn, rating)) {
        return strdup("SUCCESS|도서 추가 성공");
    }
    return strdup("ERROR|도서 추가 실패");
}

char* handle_delete_book(const char* data) {
    int id = atoi(data);
    if(delete_book(id)) {
        return strdup("SUCCESS|도서 삭제 성공");
    }
    return strdup("ERROR|도서 삭제 실패");
}

char* handle_update_book(const char* data) {
    int id;
    char title[100], author[100], publisher[100], isbn[20];
    float rating;
    
    if(sscanf(data, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%f", 
              &id, title, author, publisher, isbn, &rating) != 6) {
        return strdup("ERROR|잘못된 도서 수정 형식입니다.");
    }
    
    if(update_book(id, title, author, publisher, isbn, rating)) {
        return strdup("SUCCESS|도서 수정 성공");
    }
    return strdup("ERROR|도서 수정 실패");
}

char* handle_search_book(const char* data) {
    if(!data || strlen(data) == 0) {
        return strdup("ERROR|검색어를 입력해주세요.");
    }
    
    Book* book = search_book_by_title(data);
    if(book) {
        char response[1024];
        snprintf(response, sizeof(response), 
                "SUCCESS|=== 도서 검색 결과 ===\n"
                "ID: %d\n"
                "제목: %s\n"
                "저자: %s\n"
                "출판사: %s\n"
                "ISBN: %s\n"
                "평점: %.1f",
                book->id, book->title, book->author, 
                book->publisher, book->isbn, book->rating);
        free_book(book);
        return strdup(response);
    }
    return strdup("ERROR|도서를 찾을 수 없습니다.");
}

char* handle_list_books(void) {
    int count;
    Book* books = get_all_books(&count);
    if(!books) {
        return strdup("ERROR|도서 목록이 비어있습니다.");
    }
    
    char* response = (char*)malloc(1024 * count);
    if(!response) {
        free_book_array(books);
        return strdup("ERROR|메모리 할당 실패");
    }
    
    strcpy(response, "SUCCESS|=== 도서 목록 ===\n");
    char temp[1024];
    for(int i = 0; i < count; i++) {
        snprintf(temp, sizeof(temp), 
                "[%d] %s\n"
                "저자: %s\n"
                "출판사: %s\n"
                "ISBN: %s\n"
                "평점: %.1f\n"
                "-------------------\n",
                books[i].id, books[i].title, books[i].author,
                books[i].publisher, books[i].isbn, books[i].rating);
        strcat(response, temp);
    }
    
    free_book_array(books);
    return response;
}

void init_book_database(void) {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if(file) {
        fclose(file);
        return;
    }
    
    // data 디렉토리가 없으면 생성
    #ifdef _WIN32
    system("mkdir data 2>nul");
    #else
    system("mkdir -p data");
    #endif
    
    // 기본 도서 데이터 추가
    add_book("자바의 정석", "남궁성", "도우출판", "9788994492032", 4.5);
    add_book("이것이 자바다", "신용권", "한빛미디어", "9788968481475", 4.3);
    add_book("자바 프로그래밍", "김철수", "정보문화사", "9788956746425", 4.0);
    add_book("자바 기초부터 실전까지", "이영희", "길벗", "9788966189994", 4.2);
    add_book("자바 완벽 가이드", "박지성", "위키북스", "9788968481482", 4.4);
    
    // 도서 데이터 저장
    save_books();
    printf("기본 도서 데이터가 초기화되었습니다.\n");
} 