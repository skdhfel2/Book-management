#ifndef PROTOCOL_H
#define PROTOCOL_H

// 명령어 정의
#define CMD_LOGIN "LOGIN"
#define CMD_LOGOUT "LOGOUT"
#define CMD_ADD_USER "ADD_USER"
#define CMD_DELETE_USER "DELETE_USER"
#define CMD_UPDATE_USER "UPDATE_USER"
#define CMD_SEARCH_USER "SEARCH_USER"
#define CMD_LIST_USERS "LIST_USERS"
#define CMD_ADD_BOOK "ADD_BOOK"
#define CMD_DELETE_BOOK "DELETE_BOOK"
#define CMD_UPDATE_BOOK "UPDATE_BOOK"
#define CMD_SEARCH_BOOK "SEARCH_BOOK"
#define CMD_LIST_BOOKS "LIST_BOOKS"
#define CMD_HEARTBEAT "HEARTBEAT"

// 응답 상태
#define RESP_SUCCESS "SUCCESS"
#define RESP_ERROR "ERROR"

// 메시지 구분자
#define MSG_DELIMITER "|"
#define MSG_SECTION_DELIMITER "//"

// 메시지 생성 함수
char* create_message(const char* command, const char* data);

// 메시지 파싱 함수
void parse_message(const char* msg, char* command, char* data);

#endif // PROTOCOL_H 