CC = gcc
CFLAGS = -I../shared -Iinclude -Wall
LDFLAGS = -lws2_32

SRCS = src/main.c src/server.c src/book_management.c src/user_management.c src/utils.c
OBJS = $(SRCS:.c=.o)
TARGET = server.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean 