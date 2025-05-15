CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lws2_32    # 🔽 추가: 윈속2 라이브러리

SRCDIR = src
BUILDDIR = build
BINDIR = bin
TARGET = $(BINDIR)/server.exe

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

# 기본 빌드
all: $(TARGET)

# 링크 단계
$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 개별 오브젝트 파일 빌드
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 정리
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

# 실행
run: all
	./$(TARGET)