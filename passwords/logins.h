#ifndef LOGINS_H
#define LOGINS_H

#include <cjson/cJSON.h>

typedef struct {
  int loginCounter;
  cJSON* logins;
  cJSON* root;
} Logins;

Logins makeLogins();
void addLogin(Logins* logins, char* id, char* hostname, char* username, char* password);
char* buildStringAndFree(Logins* logins);

#endif