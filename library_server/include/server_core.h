#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <winsock2.h>
#include <windows.h>
#include "constants.h"

extern SOCKET clientSocks[MAX_CLNT];
extern int clientCount;
extern HANDLE hMutex;

unsigned WINAPI HandleClient(void* arg);

#endif 