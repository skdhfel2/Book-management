#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/file_io.h"

#define MAX_USERS 3
#define MAX_BOOKS 700

typedef struct
{
    int id;
    char title[128];
    char author[64];
    float rating;
} Book;

// 사용자 정보를 메모리로 로드
int load_users_to_memory(char users[][2][MAX_USER_LEN], int max_users)
{
    FILE *file = fopen(USERS_FILE, "r");
    if (!file)
    {
        printf("[❌] Failed to open user file\n");
        return 0;
    }

    int count = 0;
    char line[256];

    while (count < max_users && fgets(line, sizeof(line), file))
    {
        char *id = strtok(line, "/");
        char *pw = strtok(NULL, "\n");

        if (id && pw)
        {
            strncpy(users[count][0], id, MAX_USER_LEN);
            strncpy(users[count][1], pw, MAX_USER_LEN);
            count++;
        }
    }

    fclose(file);
    return count;
}

// 메모리의 사용자 정보를 파일에 저장
int save_users_from_memory(char users[][2][MAX_USER_LEN], int user_count)
{
    FILE *file = fopen(USERS_FILE, "w");
    if (!file)
    {
        printf("[❌] Failed to open user file for writing\n");
        return 0;
    }

    for (int i = 0; i < user_count; i++)
    {
        fprintf(file, "%s/%s\n", users[i][0], users[i][1]);
    }

    fclose(file);
    return 1;
}

// 도서 파일을 메모리로 불러오기
int load_books_to_memory(Book books[], int max)
{
    printf("Trying to open book file: %s\n", BOOKLIST_FILE);
    FILE *file = fopen(BOOKLIST_FILE, "r");
    if (!file)
    {
        printf("[❌] Failed to open book file\n");
        return 0;
    }
    printf("[✅] Successfully opened book file\n");

    char line[BUF_SIZE];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < max)
    {
        // 개행문자 제거
        line[strcspn(line, "\n")] = 0;
        printf("Reading line: '%s'\n", line);

        char *id_str = strtok(line, "//");
        if (!id_str)
        {
            printf("[❌] Failed to parse ID\n");
            continue;
        }

        char *title = strtok(NULL, "//");
        if (!title)
        {
            printf("[❌] Failed to parse title\n");
            continue;
        }

        char *author = strtok(NULL, "//");
        if (!author)
        {
            printf("[❌] Failed to parse author\n");
            continue;
        }

        char *rating_str = strtok(NULL, "//");
        if (!rating_str)
        {
            printf("[❌] Failed to parse rating\n");
            continue;
        }

        Book b;
        b.id = atoi(id_str);
        strncpy(b.title, title, sizeof(b.title));
        strncpy(b.author, author, sizeof(b.author));

        // 평점 파싱
        printf("[🔍] Raw rating string: '%s'\n", rating_str);
        b.rating = (float)atof(rating_str);
        printf("[✅] Parsed rating: %.2f\n", b.rating);

        books[count++] = b;
        printf("Loaded book: %d - %s by %s (%.2f)\n",
               b.id, b.title, b.author, b.rating);
    }

    fclose(file);
    printf("Total books loaded: %d\n", count);
    return count;
}

// 도서 메모리 내용을 파일로 저장
int save_books_from_memory(Book books[], int count)
{
    printf("Trying to save %d books to file\n", count);
    FILE *file = fopen(BOOKLIST_FILE, "w");
    if (!file)
    {
        printf("[❌] Failed to open book file for writing\n");
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%d//%s//%s//%.2f\n",
                books[i].id, books[i].title, books[i].author, books[i].rating);
        printf(" Saved book: %d - %s by %s (%.2f)\n",
               books[i].id, books[i].title, books[i].author, books[i].rating);
    }

    fclose(file);
    printf("[✅] Successfully saved all books\n");
    return 1;
}