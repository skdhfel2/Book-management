#include <stdio.h>
#include <string.h>
#include "../include/constants.h"
#include "../include/response_handler.h"
#include "../include/utils.h"

extern int is_logged_in; // 로그인 상태 공유

void handle_server_response(const char *response)
{
    if (strncmp(response, "SL//", 4) == 0)
    {
        int success = response[4] - '0';
        int is_login = response[6] - '0';

        if (success)
        {
            if (is_login)
            {
                is_logged_in = 1;
                print_success("로그인 성공! 메인 메뉴로 이동합니다.");
            }
            else if (strncmp(response + 4, "1 0", 3) == 0 && strstr(response, "LOGOUT") != NULL)
            {
                is_logged_in = 0;
                print_success("로그아웃되었습니다.");
            }
            // 다른 성공 응답은 무시
        }
        else
        {
            if (is_login)
            {
                print_error("로그인 실패: ID 또는 비밀번호를 확인하세요.");
            }
            else
            {
                // 사용자 추가 실패 시 에러 메시지
                if (strstr(response, "Maximum user limit reached") != NULL)
                {
                    print_error("사용자 추가 실패: 최대 사용자 수(3명)에 도달했습니다.");
                }
                else if (strstr(response, "ID already exists") != NULL)
                {
                    print_error("사용자 추가 실패: 이미 존재하는 ID입니다.");
                }
                else
                {
                    print_error("요청이 실패했습니다. 입력값 또는 권한을 확인하세요.");
                }
            }
        }
    }
    else if (strncmp(response, "BL//", 4) == 0)
    {
        static int ranking = 1; // 정적 변수로 순위 유지

        char temp[BUF_SIZE];
        strncpy(temp, response + 4, BUF_SIZE - 1);
        temp[BUF_SIZE - 1] = '\0';

        char *id = strtok(temp, "/");
        char *title = strtok(NULL, "/");
        char *author = strtok(NULL, "/");
        char *rating = strtok(NULL, "\n");

        if (id && title && author && rating)
        {
            if (strcmp(id, "0") == 0)
            {
                print_error(title); // "No matching books found" 메시지
                ranking = 1;        // 순위 초기화
            }
            else
            {
                // 검색 결과는 ID를 그대로 표시, 랭킹 조회는 순위 표시
                if (strstr(response, "RANK") != NULL)
                {
                    printf("%d / %s / %s / %s\n", ranking++, title, author, rating);
                }
                else
                {
                    printf("%s / %s / %s / %s\n", id, title, author, rating);
                }
                fflush(stdout);
            }
        }
    }
    else if (strncmp(response, "MSG//", 5) == 0)
    {
        // ✅ 여기가 바로 도서 추가 등 사용자 메시지 출력
        printf("[📘] %s\n", response + 5);
    }
}
