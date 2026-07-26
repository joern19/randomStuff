#include "logins.h"

Logins makeLogins() {
  cJSON* root = cJSON_CreateObject();

  Logins logins = {
    .loginCounter = 0,
    .logins = cJSON_AddArrayToObject(root, "logins"),
    .root = root,
  };
  return logins;
}

void addLogin(Logins* logins, char* id, char* hostname, char* username, char* password) {
  logins->loginCounter++;
  cJSON* login = cJSON_CreateObject();

  cJSON_AddStringToObject(login, "hostname", hostname);
  cJSON_AddStringToObject(login, "encryptedUsername", username);
  cJSON_AddStringToObject(login, "encryptedPassword", password);
  cJSON_AddStringToObject(login, "guid", id);

  cJSON_AddItemToArray(logins->logins, login);
}

char* buildStringAndFree(Logins* logins) {
  cJSON_AddNumberToObject(logins->root, "version", 3);

  char *json = cJSON_PrintUnformatted(logins->root);
  cJSON_Delete(logins->root);
  return json;
}
