#include <stdio.h>
#include <string.h>
#include "../include/utils.h"

void print_success(const char* msg) {
    printf("\033[0;32m[SUCCESS]\033[0m %s\n", msg);
}

void print_error(const char* msg) {
    printf("\033[0;31m[ERROR]\033[0m %s\n", msg);
}

void log_info(const char* msg) {
    printf("[INFO] %s\n", msg);
}

void trim_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}
