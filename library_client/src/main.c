#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include "../include/constants.h"
#include "../include/utils.h"
#include "../include/client_menu.h"
#include "../include/response_handler.h"
#include "../include/client_core.h"

SOCKET sock;
int is_logged_in = 0;

unsigned WINAPI send_thread(void* arg);
unsigned WINAPI recv_thread(void* arg);

int main() {
    WSADATA wsaData;
    SOCKADDR_IN serverAddr;
    char serverIp[32] = "127.0.0.1";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        print_error("WSAStartup() failed");
        return 1;
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        print_error("Connection failed");
        return 1;
    }
    print_success("Connected to server");

    HANDLE hSend = (HANDLE)_beginthreadex(NULL, 0, send_thread, NULL, 0, NULL);
    HANDLE hRecv = (HANDLE)_beginthreadex(NULL, 0, recv_thread, NULL, 0, NULL);

    WaitForSingleObject(hSend, INFINITE);
    WaitForSingleObject(hRecv, INFINITE);

    closesocket(sock);
    WSACleanup();
    return 0;
}

unsigned WINAPI send_thread(void* arg) {
    while (1) {
        if (!is_logged_in)
            show_login_menu();
        else
            show_main_menu();
    }
    return 0;
}

unsigned WINAPI recv_thread(void* arg) {
    char buf[BUF_SIZE];
    int len;
    while ((len = recv(sock, buf, BUF_SIZE - 1, 0)) > 0) {
        buf[len] = '\0';
        handle_server_response(buf);
    }
    return 0;
}
