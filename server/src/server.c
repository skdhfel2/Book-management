#include "../include/utils.h"
#include "../include/user_management.h"
#include "../include/book_management.h"
#include <signal.h>
#include <time.h>
#include <process.h>

#define CLIENT_TIMEOUT 30  // 30초 타임아웃
#define HEARTBEAT_INTERVAL 10  // 10초마다 하트비트

SOCKET clientSocks[MAX_CLNT];
int clientCount = 0;
HANDLE hMutex;
SOCKET serverSock;
time_t last_heartbeat[MAX_CLNT];  // 마지막 하트비트 시간 저장

void cleanup_server(void) {
    // 모든 클라이언트 연결 종료
    for(int i = 0; i < clientCount; i++) {
        closesocket(clientSocks[i]);
    }
    
    // 서버 소켓 종료
    closesocket(serverSock);
    
    // 관리 시스템 정리
    cleanup_user_management();
    cleanup_book_management();
    
    // 윈속 종료
    WSACleanup();
    
    printf("\n서버가 정상적으로 종료되었습니다.\n");
}

void signal_handler(int signo) {
    if(signo == SIGINT) {
        printf("\n서버를 종료합니다...\n");
        cleanup_server();
        exit(0);
    }
}

// 클라이언트 연결 상태 확인
void check_client_connections(void) {
    time_t current_time = time(NULL);
    
    WaitForSingleObject(hMutex, INFINITE);
    for(int i = 0; i < clientCount; i++) {
        if(current_time - last_heartbeat[i] > CLIENT_TIMEOUT) {
            printf("Client %d timed out, removing connection\n", i);
            closesocket(clientSocks[i]);
            
            // 클라이언트 목록에서 제거
            for(int j = i; j < clientCount - 1; j++) {
                clientSocks[j] = clientSocks[j + 1];
                last_heartbeat[j] = last_heartbeat[j + 1];
            }
            clientCount--;
            i--;  // 현재 인덱스 다시 확인
        }
    }
    ReleaseMutex(hMutex);
}

// 하트비트 처리
void handle_heartbeat(SOCKET clientSock) {
    WaitForSingleObject(hMutex, INFINITE);
    for(int i = 0; i < clientCount; i++) {
        if(clientSocks[i] == clientSock) {
            last_heartbeat[i] = time(NULL);
            break;
        }
    }
    ReleaseMutex(hMutex);
}

unsigned WINAPI HandleClient(void* arg) {
    SOCKET clientSock = *((SOCKET*)arg);
    char buffer[1024];
    char* command;
    char* data;
    char* response;
    
    // 클라이언트 연결 시간 초기화
    WaitForSingleObject(hMutex, INFINITE);
    for(int i = 0; i < clientCount; i++) {
        if(clientSocks[i] == clientSock) {
            last_heartbeat[i] = time(NULL);
            break;
        }
    }
    ReleaseMutex(hMutex);
    
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        if(recv(clientSock, buffer, sizeof(buffer), 0) <= 0) {
            break;
        }
        
        command = strtok(buffer, "|");
        data = strtok(NULL, "|");
        
        if(!command) continue;
        
        // 하트비트 처리
        if(strcmp(command, "HEARTBEAT") == 0) {
            handle_heartbeat(clientSock);
            continue;
        }
        
        // 기존 명령어 처리
        if(strcmp(command, "LOGIN") == 0) {
            response = handle_login(data);
        }
        else if(strcmp(command, "LOGOUT") == 0) {
            response = handle_logout(clientSock);
        }
        else if(strcmp(command, "ADD_USER") == 0) {
            response = handle_add_user(data);
        }
        else if(strcmp(command, "DELETE_USER") == 0) {
            response = handle_delete_user(data);
        }
        else if(strcmp(command, "UPDATE_USER") == 0) {
            response = handle_update_user(data);
        }
        else if(strcmp(command, "SEARCH_USER") == 0) {
            response = handle_search_user(data);
        }
        else if(strcmp(command, "LIST_USERS") == 0) {
            response = handle_list_users();
        }
        else if(strcmp(command, "ADD_BOOK") == 0) {
            response = handle_add_book(data);
        }
        else if(strcmp(command, "DELETE_BOOK") == 0) {
            response = handle_delete_book(data);
        }
        else if(strcmp(command, "UPDATE_BOOK") == 0) {
            response = handle_update_book(data);
        }
        else if(strcmp(command, "SEARCH_BOOK") == 0) {
            response = handle_search_book(data);
        }
        else if(strcmp(command, "LIST_BOOKS") == 0) {
            response = handle_list_books();
        }
        else {
            response = strdup("Unknown command");
        }
        
        if(response) {
            send(clientSock, response, strlen(response), 0);
            free(response);
        }
    }
    
    // 클라이언트 연결 종료 처리
    WaitForSingleObject(hMutex, INFINITE);
    for(int i = 0; i < clientCount; i++) {
        if(clientSock == clientSocks[i]) {
            while(i++ < clientCount - 1) {
                clientSocks[i] = clientSocks[i + 1];
                last_heartbeat[i] = last_heartbeat[i + 1];
            }
            break;
        }
    }
    clientCount--;
    ReleaseMutex(hMutex);
    closesocket(clientSock);
    return 0;
}

// 주기적으로 클라이언트 연결 상태 확인
unsigned WINAPI ConnectionMonitor(void* arg) {
    while(1) {
        check_client_connections();
        Sleep(1000);  // 1초마다 확인
    }
    return 0;
}

int main() {
    WSADATA wsaData;
    SOCKET clientSock;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrSize;
    HANDLE hThread;
    
    // 시그널 핸들러 등록
    signal(SIGINT, signal_handler);
    
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");
    
    hMutex = CreateMutex(NULL, FALSE, NULL);
    serverSock = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(55555);
    
    if(bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");
    
    if(listen(serverSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");
    
    // data 디렉토리 생성
    #ifdef _WIN32
    system("mkdir data 2>nul");
    #else
    system("mkdir -p data");
    #endif
    
    // 사용자 관리와 도서 관리 초기화
    printf("시스템 초기화 중...\n");
    
    if(!init_user_management()) {
        ErrorHandling("사용자 관리 시스템 초기화 실패");
    }
    printf("사용자 관리 시스템 초기화 완료\n");
    
    if(!init_book_management()) {
        ErrorHandling("도서 관리 시스템 초기화 실패");
    }
    printf("도서 관리 시스템 초기화 완료\n");
    
    printf("Server started...\n");
    
    // 연결 모니터링 스레드 시작
    HANDLE hMonitor = (HANDLE)_beginthreadex(NULL, 0, ConnectionMonitor, NULL, 0, NULL);
    
    while(1) {
        clientAddrSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrSize);
        
        WaitForSingleObject(hMutex, INFINITE);
        clientSocks[clientCount++] = clientSock;
        ReleaseMutex(hMutex);
        
        hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClient, (void*)&clientSock, 0, NULL);
        printf("Connected client IP: %s\n", inet_ntoa(clientAddr.sin_addr));
    }
    
    cleanup_server();
    return 0;
} 