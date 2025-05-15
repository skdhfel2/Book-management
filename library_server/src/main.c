#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include "../include/constants.h"
#include "../include/protocol.h"
#include "../include/book_management.h"
#include "../include/server_core.h"

void ErrorHandling(const char* msg);

SOCKET clientSocks[MAX_CLNT];
int clientCount = 0;
HANDLE hMutex;
SOCKET serverSock;  // 전역으로 옮김

// ✅ 서버 종료 처리 함수 (S15)
BOOL WINAPI console_handler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
        printf("\n[🔴] 서버 종료 요청 수신... 연결 정리 중...\n");

        // 클라이언트 소켓 모두 닫기
        for (int i = 0; i < clientCount; i++) {
            closesocket(clientSocks[i]);
        }

        closesocket(serverSock);
        CloseHandle(hMutex);
        WSACleanup();

        printf("[✅] 모든 연결과 자원이 안전하게 정리되었습니다.\n");
        exit(0);
    }
    return TRUE;
}

int main() {
    WSADATA wsaData;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrSize;
    SOCKET clientSock;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // ✅ 종료 처리 핸들러 등록
    SetConsoleCtrlHandler(console_handler, TRUE);

    hMutex = CreateMutex(NULL, FALSE, NULL);

    serverSock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    if (listen(serverSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");

    printf("[🔵] Server started on port %d\n", SERVER_PORT);

    while (1) {
        clientAddrSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrSize);

        WaitForSingleObject(hMutex, INFINITE);
        clientSocks[clientCount++] = clientSock;
        ReleaseMutex(hMutex);

        _beginthreadex(NULL, 0, HandleClient, (void*)&clientSock, 0, NULL);
        printf("[🟢] Connected Client IP: %s\n", inet_ntoa(clientAddr.sin_addr));
    }

    // 이 아래는 보통 실행되지 않지만, 안전상 남겨둠
    closesocket(serverSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(const char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
