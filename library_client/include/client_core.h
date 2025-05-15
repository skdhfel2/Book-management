#ifndef CLIENT_CORE_H
#define CLIENT_CORE_H

void send_login_command(const char* id, const char* pw);
void send_logout_command();
void send_quit_command();

void send_add_user_command(const char* id, const char* pw);
void send_delete_user_command(const char* id);
void send_update_user_command(const char* id, const char* new_pw);

void send_search_book_command(const char* keyword);
void send_add_book_command(const char* title, const char* author, float rating);
void send_delete_book_command(int id);
void send_update_book_command(int id, const char* title, const char* author, float rating);
void send_rank_book_command(const char* mode);

void send_custom_command(const char* cmd);

#endif
