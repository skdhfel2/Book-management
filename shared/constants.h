#ifndef CONSTANTS_H
#define CONSTANTS_H

// 버퍼 크기
#define BUF_SIZE 1024

// 최대 도서 수
#define MAX_BOOKS 1000

// 최대 사용자 수
#define MAX_USERS 100

// 최대 클라이언트 수
#define MAX_CLNT 10

// 서버 포트
#define SERVER_PORT 55555

// 서버 IP
#define SERVER_IP "127.0.0.1"

// 하트비트 간격 (초)
#define HEARTBEAT_INTERVAL 10

// 클라이언트 타임아웃 (초)
#define CLIENT_TIMEOUT 30

// 파일 경로
#define BOOKS_FILE "data/books.dat"
#define USERS_FILE "data/users.txt"
#define HISTORY_FILE "data/history.txt"

#endif // CONSTANTS_H 