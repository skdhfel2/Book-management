#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/constants.h"
#include "../include/user_management.h"

int authenticate_user(const char* id, const char* password) {
    FILE* file = fopen(USERS_FILE, "r");
    if (!file) return 0;

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        char* token_id = strtok(line, "//");
        char* token_pw = strtok(NULL, "\n");
        if (token_id && token_pw && strcmp(token_id, id) == 0 && strcmp(token_pw, password) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int add_user(const char* id, const char* password) {
    FILE* check = fopen(USERS_FILE, "r");
    if (!check) return 0;
    char line[128];
    while (fgets(line, sizeof(line), check)) {
        char* token_id = strtok(line, "//");
        if (token_id && strcmp(token_id, id) == 0) {
            fclose(check);
            return 0; // 중복 ID
        }
    }
    fclose(check);

    FILE* file = fopen(USERS_FILE, "a");
    if (!file) return 0;
    fprintf(file, "%s//%s\n", id, password);
    fclose(file);
    return 1;
}

int delete_user(const char* id) {
    FILE* file = fopen(USERS_FILE, "r");
    FILE* temp = fopen("./data/temp_users.txt", "w");
    if (!file || !temp) return 0;

    char line[128];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char line_copy[128];
        strcpy(line_copy, line);
        char* token_id = strtok(line_copy, "//");
        if (token_id && strcmp(token_id, id) == 0) {
            found = 1; // 삭제 대상 발견
            continue;
        }
        fputs(line, temp);
    }
    fclose(file);
    fclose(temp);

    remove(USERS_FILE);
    rename("./data/temp_users.txt", USERS_FILE);
    return found;
}

int update_user(const char* id, const char* new_password) {
    FILE* file = fopen(USERS_FILE, "r");
    FILE* temp = fopen("./data/temp_users.txt", "w");
    if (!file || !temp) return 0;

    char line[128];
    int updated = 0;
    while (fgets(line, sizeof(line), file)) {
        char* token_id = strtok(line, "//");
        if (token_id && strcmp(token_id, id) == 0) {
            fprintf(temp, "%s//%s\n", id, new_password);
            updated = 1;
        } else {
            char* token_pw = strtok(NULL, "\n");
            if (token_pw)
                fprintf(temp, "%s//%s\n", token_id, token_pw);
        }
    }
    fclose(file);
    fclose(temp);

    remove(USERS_FILE);
    rename("./data/temp_users.txt", USERS_FILE);
    return updated;
}