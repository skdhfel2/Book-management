#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "../include/constants.h"
#include "../include/protocol.h"
#include "../include/book_management.h"  // 🔸 검색 함수 포함
#include "../include/file_io.h"  // 파일 입출력 함수 포함

#define MAX_BOOKS 700
#define MAX_USERS 3

typedef struct {
    int id;
    char title[128];
    char author[64];
    float rating;
} Book;

extern SOCKET clientSocks[MAX_CLNT];
extern int clientCount;
extern HANDLE hMutex;

unsigned WINAPI HandleClient(void* arg) {
    SOCKET clientSock = *((SOCKET*)arg);
    char msg[BUF_SIZE];
    int strLen;

    while ((strLen = recv(clientSock, msg, BUF_SIZE - 1, 0)) > 0) {
        msg[strLen] = '\0';
        printf("[📨] Received: %s\n", msg);

        // 명령어 파싱 (예: CL//SEARCH//title//author//id)
        char* cmd = strtok(msg, "//");
        char* sub_cmd = strtok(NULL, "//");

        printf("[🔍] Parsed command: cmd='%s', sub_cmd='%s'\n", 
            cmd ? cmd : "NULL", 
            sub_cmd ? sub_cmd : "NULL");

        if (!cmd || !sub_cmd) {
            printf("[❌] Invalid command format\n");
            send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
            continue;
        }

        // 🔸 종료 처리
        if (strcmp(sub_cmd, CMD_QUIT) == 0) {
            printf("[❌] Client requested quit\n");
            break;
        }

        // 🔸 로그인 처리
        else if (strcmp(sub_cmd, CMD_LOGIN) == 0) {
            char* id = strtok(NULL, "//");
            char* pw = strtok(NULL, "//");
            
            if (!id || !pw) {
                printf("[❌] Login failed: Invalid format\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            printf("[🔑] Login attempt: id='%s', pw='%s'\n", id, pw);

            // 사용자 인증
            char users[MAX_USERS][2][MAX_USER_LEN];
            int user_count = load_users_to_memory(users, MAX_USERS);
            int authenticated = 0;

            printf("[📊] Loaded %d users\n", user_count);
            for (int i = 0; i < user_count; i++) {
                printf("[👤] Checking user %d: id='%s', pw='%s'\n", 
                    i, users[i][0], users[i][1]);
                if (strcmp(users[i][0], id) == 0 && strcmp(users[i][1], pw) == 0) {
                    authenticated = 1;
                    break;
                }
            }

            if (authenticated) {
                printf("[✅] Login successful: %s\n", id);
                send(clientSock, "SL//1 1\n", strlen("SL//1 1\n"), 0);
            } else {
                printf("[❌] Login failed: Invalid credentials\n");
                send(clientSock, "SL//0 1\n", strlen("SL//0 1\n"), 0);
            }
        }

        // 🔸 로그아웃 처리
        else if (strcmp(sub_cmd, CMD_LOGOUT) == 0) {
            printf("[✅] Logout successful\n");
            send(clientSock, "SL//1 0\n", strlen("SL//1 0\n"), 0);
            continue;  // 다음 명령어 대기
        }

        // 🔸 사용자 추가
        else if (strcmp(sub_cmd, CMD_ADD_USER) == 0) {
    char* id = strtok(NULL, "//");
    char* pw = strtok(NULL, "//");

    if (!id || !pw) {
        printf("[❌] Add user failed: Invalid format\n");
        send(clientSock, "SL//0 0 Invalid format\n", strlen("SL//0 0 Invalid format\n"), 0);
        continue;
    }

    char users[MAX_USERS][2][MAX_USER_LEN];
    int user_count = load_users_to_memory(users, MAX_USERS);

    // 사용자 수 제한 확인
    if (user_count >= MAX_USERS) {
        printf("[❌] Add user failed: Maximum user limit reached (%d/%d)\n", user_count, MAX_USERS);
        send(clientSock, "SL//0 0 Maximum user limit reached\n", strlen("SL//0 0 Maximum user limit reached\n"), 0);
        continue;
    }

    // ID 중복 체크
    int is_duplicate = 0;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i][0], id) == 0) {
            is_duplicate = 1;
            break;
        }
    }

    if (is_duplicate) {
        printf("[❌] Add user failed: ID already exists\n");
        send(clientSock, "SL//0 0 ID already exists\n", strlen("SL//0 0 ID already exists\n"), 0);
        continue;
    }

    // 새 사용자 추가
    strncpy(users[user_count][0], id, MAX_USER_LEN);
    strncpy(users[user_count][1], pw, MAX_USER_LEN);

    if (save_users_from_memory(users, user_count + 1)) {
        printf("[✅] Add user successful: %s (Total users: %d/%d)\n", id, user_count + 1, MAX_USERS);
        send(clientSock, "SL//1 0\n", strlen("SL//1 0\n"), 0);
    } else {
        printf("[❌] Add user failed: Save error\n");
        send(clientSock, "SL//0 0 Save error\n", strlen("SL//0 0 Save error\n"), 0);
    }
}

        // 🔸 사용자 삭제
        else if (strcmp(sub_cmd, CMD_DELETE_USER) == 0) {
            char* id = strtok(NULL, "//");
            
            if (!id) {
                printf("[❌] Delete user failed: Invalid format\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            char users[MAX_USERS][2][MAX_USER_LEN];
            int user_count = load_users_to_memory(users, MAX_USERS);
            int found = 0;

            // 사용자 찾기 및 삭제
            for (int i = 0; i < user_count; i++) {
                if (strcmp(users[i][0], id) == 0) {
                    found = 1;
                    // 마지막 사용자를 현재 위치로 이동
                    if (i < user_count - 1) {
                        strncpy(users[i][0], users[user_count-1][0], MAX_USER_LEN);
                        strncpy(users[i][1], users[user_count-1][1], MAX_USER_LEN);
                    }
                    break;
                }
            }

            if (found && save_users_from_memory(users, user_count - 1)) {
                printf("[✅] Delete user successful: %s\n", id);
                send(clientSock, "SL//1 0\n", strlen("SL//1 0\n"), 0);
            } else {
                printf("[❌] Delete user failed: User not found or save error\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
            }
        }

        // 🔸 사용자 비밀번호 수정
        else if (strcmp(sub_cmd, CMD_UPDATE_USER) == 0) {
            char* id = strtok(NULL, "//");
            char* new_pw = strtok(NULL, "//");
            
            if (!id || !new_pw) {
                printf("[❌] Update user failed: Invalid format\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            char users[MAX_USERS][2][MAX_USER_LEN];
            int user_count = load_users_to_memory(users, MAX_USERS);
            int found = 0;

            // 사용자 찾기 및 비밀번호 수정
            for (int i = 0; i < user_count; i++) {
                if (strcmp(users[i][0], id) == 0) {
                    strncpy(users[i][1], new_pw, MAX_USER_LEN);
                    found = 1;
                    break;
                }
            }

            if (found && save_users_from_memory(users, user_count)) {
                printf("[✅] Update user successful: %s\n", id);
                send(clientSock, "SL//1 0\n", strlen("SL//1 0\n"), 0);
            } else {
                printf("[❌] Update user failed: User not found or save error\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
            }
        }

        // 🔸 도서 검색 처리
        else if (strcmp(sub_cmd, CMD_SEARCH_BOOK) == 0) {
            char* title  = strtok(NULL, "//");
            char* author = strtok(NULL, "//");
            char* id_str = strtok(NULL, "//");

            printf("[🔍] Search parameters: title='%s', author='%s', id='%s'\n",
                title ? title : "NULL",
                author ? author : "NULL",
                id_str ? id_str : "NULL");

            if (!title)  title  = "";
            if (!author) author = "";
            
            int id = -1;
            if (id_str && id_str[0] != '\0') {
                id = atoi(id_str);
            }

            Book books[MAX_BOOKS];
            int book_count = load_books_to_memory(books, MAX_BOOKS);
            printf("[📚] Loaded %d books for search\n", book_count);
            
            if (book_count == 0) {
                printf("[❌] No books loaded\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            int found = 0;
            char response[BUF_SIZE];

            // 검색 결과 전송 시작
            sprintf(response, "SL//1 0\n");
            send(clientSock, response, strlen(response), 0);

            // 검색 조건에 맞는 도서 찾기
            for (int i = 0; i < book_count; i++) {
                if ((strlen(title) == 0 || strstr(books[i].title, title)) &&
                    (strlen(author) == 0 || strstr(books[i].author, author)) &&
                    (id == -1 || books[i].id == id)) {
                    
                    sprintf(response, "BL//%d/%s/%s/%.2f\n",
                            books[i].id, books[i].title, books[i].author, books[i].rating);
                    send(clientSock, response, strlen(response), 0);
                    found = 1;
                }
            }

            if (!found) {
                printf("[❌] No matching books found\n");
                sprintf(response, "BL//0/No matching books found//\n");
                send(clientSock, response, strlen(response), 0);
            } else {
                printf("[✅] Search successful, found %d matches\n", found);
            }
            continue;  // 다음 명령어 대기
        }

        // 🔸 도서 랭킹 조회
        else if (strcmp(sub_cmd, CMD_RANK_BOOK) == 0) {
            Book books[MAX_BOOKS];
            int book_count = load_books_to_memory(books, MAX_BOOKS);
            printf("[📚] Loaded %d books for ranking\n", book_count);
            
            if (book_count == 0) {
                printf("[❌] No books loaded\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            // 평점 기준으로 정렬
            for (int i = 0; i < book_count - 1; i++) {
                for (int j = 0; j < book_count - i - 1; j++) {
                    if (books[j].rating < books[j + 1].rating) {
                        Book temp = books[j];
                        books[j] = books[j + 1];
                        books[j + 1] = temp;
                    }
                }
            }

            // 검색 결과 전송 시작
            char response[BUF_SIZE];
            sprintf(response, "SL//1 0\n");
            send(clientSock, response, strlen(response), 0);

            // 모든 도서 전송
            for (int i = 0; i < book_count; i++) {
                sprintf(response, "BL//%d/%s/%s/%.2f\n",
                        books[i].id, books[i].title, books[i].author, books[i].rating);
                send(clientSock, response, strlen(response), 0);
                printf("[📖] Sent book: %d - %s by %s (%.2f)\n", 
                    books[i].id, books[i].title, books[i].author, books[i].rating);
            }
            printf("[✅] Sent %d ranked books\n", book_count);
            continue;  // 다음 명령어 대기
        }

        // 🔸 도서 추가
        else if (strcmp(sub_cmd, CMD_ADD_BOOK) == 0) {
            char* title = strtok(NULL, "//");
            char* author = strtok(NULL, "//");
            char* rating_str = strtok(NULL, "//");
            
            if (!title || !author || !rating_str) {
                printf("[❌] Add book failed: Invalid format\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            float rating = (float)atof(rating_str);
            if (rating < 0.0f || rating > 5.0f) {
                printf("[❌] Add book failed: Invalid rating range\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            Book books[MAX_BOOKS];
            int book_count = load_books_to_memory(books, MAX_BOOKS);
            printf("[📚] Loaded %d existing books\n", book_count);
            
            // 새 도서 ID 할당
            int new_id = 1;
            for (int i = 0; i < book_count; i++) {
                if (books[i].id >= new_id) {
                    new_id = books[i].id + 1;
                }
            }

            // 새 도서 추가
            Book new_book;
            new_book.id = new_id;
            strncpy(new_book.title, title, sizeof(new_book.title));
            strncpy(new_book.author, author, sizeof(new_book.author));
            new_book.rating = rating;

            books[book_count] = new_book;
            printf("[📖] Adding new book: %d - %s by %s (%.2f)\n", 
                new_id, title, author, rating);
            
            if (save_books_from_memory(books, book_count + 1)) {
                printf("[✅] Add book successful\n");
                send(clientSock, "SL//1 0\n", strlen("SL//1 0\n"), 0);
                // 사용자에게 친절한 메시지 전송
                char msg[BUF_SIZE];
                sprintf(msg, "MSG//도서 '%s' 추가가 완료되었습니다.\n", title);
                send(clientSock, msg, strlen(msg), 0);
}
            } else {
                printf("[❌] Add book failed: Save error\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
            }
            continue;
        }

        // 🔸 도서 수정
        else if (strcmp(sub_cmd, CMD_UPDATE_BOOK) == 0) {
            char* id_str = strtok(NULL, "//");
            char* title = strtok(NULL, "//");
            char* author = strtok(NULL, "//");
            char* rating_str = strtok(NULL, "//");
            
            if (!id_str || !title || !author || !rating_str) {
                printf("[❌] Update book failed: Invalid format\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            int id = atoi(id_str);
            float rating = (float)atof(rating_str);
            if (rating < 0.0f || rating > 5.0f) {
                printf("[❌] Update book failed: Invalid rating range\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            Book books[MAX_BOOKS];
            int book_count = load_books_to_memory(books, MAX_BOOKS);
            int found = 0;

            // 도서 찾기 및 수정
            for (int i = 0; i < book_count; i++) {
                if (books[i].id == id) {
                    strncpy(books[i].title, title, sizeof(books[i].title));
                    strncpy(books[i].author, author, sizeof(books[i].author));
                    books[i].rating = rating;
                    found = 1;
                    printf("[📖] Updated book: %d - %s by %s (%.2f)\n", 
                        id, title, author, rating);
                    break;
                }
            }

            if (found && save_books_from_memory(books, book_count)) {
                printf("[✅] Update book successful\n");
                send(clientSock, "SL//1 0\n", strlen("SL//1 0\n"), 0);
            } else {
                printf("[❌] Update book failed: Book not found or save error\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
            }
            continue;
        }

        // 🔸 도서 삭제
        else if (strcmp(sub_cmd, CMD_DELETE_BOOK) == 0) {
            char* id_str = strtok(NULL, "//");
            
            if (!id_str) {
                printf("[❌] Delete book failed: Invalid format\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
                continue;
            }

            int id = atoi(id_str);
            Book books[MAX_BOOKS];
            int book_count = load_books_to_memory(books, MAX_BOOKS);
            int found = 0;

            // 도서 찾기 및 삭제
            for (int i = 0; i < book_count; i++) {
                if (books[i].id == id) {
                    found = 1;
                    // 마지막 도서를 현재 위치로 이동
                    if (i < book_count - 1) {
                        books[i] = books[book_count - 1];
                    }
                    break;
                }
            }

            if (found && save_books_from_memory(books, book_count - 1)) {
                printf("[✅] Delete book successful: %d\n", id);
                send(clientSock, "SL//1 0\n", strlen("SL//1 0\n"), 0);
            } else {
                printf("[❌] Delete book failed: Book not found or save error\n");
                send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
            }
            continue;
        }

        // 🔸 알 수 없는 명령
        else {
            printf("[❌] Unknown command: %s\n", sub_cmd);
            send(clientSock, "SL//0 0\n", strlen("SL//0 0\n"), 0);
        }
    }

    printf("[🔴] Client disconnected\n");

    WaitForSingleObject(hMutex, INFINITE);
    for (int i = 0; i < clientCount; ++i) {
        if (clientSock == clientSocks[i]) {
            for (; i < clientCount - 1; ++i)
                clientSocks[i] = clientSocks[i + 1];
            break;
        }
    }
    clientCount--;
    ReleaseMutex(hMutex);
    closesocket(clientSock);
    return 0;
}
