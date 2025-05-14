#include "../include/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

void ErrorHandling(const char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

char* trim(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int is_valid_number(const char* str) {
    if(str == NULL || *str == '\0') return 0;
    while(*str) {
        if(!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int is_valid_rating(float rating) {
    return rating >= 0.0f && rating <= 5.0f;
}

int save_to_file(const char* filename, const char* data) {
    FILE* file = fopen(filename, "w");
    if(!file) return 0;
    
    fprintf(file, "%s", data);
    fclose(file);
    return 1;
}

char* read_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if(!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(size + 1);
    if(!buffer) {
        fclose(file);
        return NULL;
    }
    
    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';
    
    fclose(file);
    return buffer;
}

char* create_message(const char* command, const char* data) {
    char* msg = (char*)malloc(BUF_SIZE);
    if(msg == NULL) return NULL;
    
    snprintf(msg, BUF_SIZE, "SL // %s // %s", command, data);
    return msg;
}

void parse_message(const char* msg, char* command, char* data) {
    char* temp = strdup(msg);
    char* token = strtok(temp, "//");
    
    if(token != NULL) {
        token = strtok(NULL, "//");
        if(token != NULL) {
            strcpy(command, trim(token));
            token = strtok(NULL, "//");
            if(token != NULL) {
                strcpy(data, trim(token));
            }
        }
    }
    
    free(temp);
} 