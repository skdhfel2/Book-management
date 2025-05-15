CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lws2_32

SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin
TARGET = $(BINDIR)/client.exe

OBJ = \
  $(BUILDDIR)/utils.o \
  $(BUILDDIR)/user_interface.o \
  $(BUILDDIR)/main.o \
  $(BUILDDIR)/client_menu.o \
  $(BUILDDIR)/response_handler.o \
  $(BUILDDIR)/client_core.o

# 기본 빌드
all: $(TARGET)

# 실행 파일 만들기
$(TARGET): $(OBJ)
	@if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); fi
	$(CC) -o $@ $^ $(LDFLAGS)

# 오브젝트 파일 만들기
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@if [ ! -d $(BUILDDIR) ]; then mkdir $(BUILDDIR); fi
	$(CC) $(CFLAGS) -c $< -o $@

# 실행
run: all
	./$(TARGET)

# 정리
clean:
	rm -rf $(BUILDDIR) $(BINDIR)
