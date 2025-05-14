CC = gcc
CFLAGS = -I../shared -Iinclude -Wall
LDFLAGS = -lws2_32

SRCS = src/main.c src/menu.c src/utils.c src/client.c
OBJS = $(SRCS:.c=.o)
TARGET = client.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean 