#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <winsock2.h>

int authenticate_user(const char *id, const char *password);
int add_user(const char *id, const char *password);
int delete_user(const char *id);
int update_user(const char *id, const char *new_password);

#endif
