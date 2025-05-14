#include "../include/menu.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55555  // 서버 포트 번호 수정

int main() {
    if(!init_client_socket()) {
        printf("서버 연결에 실패했습니다.\n");
        return 1;
    }
    
    printf("서버에 연결되었습니다.\n");
    
    int choice;
    while(1) {
        show_main_menu();
        printf("선택: ");
        scanf("%d", &choice);
        getchar();  // 버퍼 비우기
        
        switch(choice) {
            case 1:
                show_user_menu();
                break;
            case 2:
                show_book_menu();
                break;
            case 3:
                show_history();
                break;
            case 0:
                cleanup_client_socket();
                printf("프로그램을 종료합니다.\n");
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
    
    return 0;
} 