# 도서관 관리 시스템

이 프로젝트는 도서관 관리 시스템을 구현한 소켓 기반의 클라이언트-서버 애플리케이션입니다.

## 주요 기능

### 서버 기능
- 사용자 인증 및 관리
- 도서 정보 관리 (추가, 삭제, 수정, 검색)
- 도서 평점 기반 랭킹 제공
- 다중 클라이언트 동시 접속 지원

### 클라이언트 기능
- 직관적인 메뉴 기반 인터페이스
- 도서 검색 및 관리 기능
- 명령어 자동완성 시스템
- 사용자 관리 기능

## 빌드 및 실행 방법

### 서버 빌드
```bash
cd server/src
gcc -o server server.c user_management.c book_management.c utils.c -I../include
```

### 클라이언트 빌드
```bash
cd client/src
gcc -o client client.c menu.c utils.c -I../include
```

## 프로토콜 형식

### 클라이언트 요청 형식
```
CL // 명령어 // 데이터
```

### 서버 응답 형식
```
SL // 결과코드 // 데이터
```

## 데이터 파일
- users.txt: 사용자 정보 저장
- booklist2.txt: 도서 정보 저장 