#include <stdio.h>
#include <string.h>

#include <nss.h>
#include <pk11pub.h>
#include <pk11sdr.h>
#include <secitem.h>
#include <secerr.h>
#include "crypt.h"
#include "logins.h"
#include "passwords.h"

PK11SlotInfo* setup() {

  PK11SlotInfo *slot = PK11_GetInternalKeySlot();

  if (!slot) {
    fprintf(stderr, "no internal slot\n");
    return 0;
  }

  if (PK11_Authenticate(slot, PR_TRUE, NULL) != SECSuccess) {
    fprintf(stderr, "authentication failed\n");
    return 0;
  }
  return slot;
}

void teardown(PK11SlotInfo* slot) {
  PK11_FreeSlot(slot);
  NSS_Shutdown();
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Example: %s /home/john/.mozilla/firefox/asdf.default-release\n", argv[0]);
    exit(2);
  }

  PasswordList password_list = {
    .count = 0,
    .entries = NULL,
  };
  passwords_load(&password_list);

  if (NSS_Init(argv[1]) != SECSuccess) {
    fprintf(stderr, "NSS_Init failed\n");
    return 1;
  }

  PK11SlotInfo* slot = setup();
  if (slot == NULL) {
    return 1;
  }

  Logins logins = makeLogins();
  for (size_t i = 0; i < password_list.count; i++) {
    PasswordEntry entry = password_list.entries[i];

    char* encryptedUsername = encryptEncode(slot, entry.username);
    char* encryptedPassword = encryptEncode(slot, entry.password);
    addLogin(&logins, entry.path, entry.url, encryptedUsername, encryptedPassword);

  }
  teardown(slot);

  char* loginsPath = malloc(strlen(argv[1]) + 13);
  strcpy(loginsPath, argv[1]);
  memcpy(loginsPath + strlen(argv[1]), "/logins.json", 12);
  printf("Writing logins to %s\n", loginsPath);
  FILE* loginsFile = fopen(loginsPath, "w");
  fprintf(loginsFile, buildStringAndFree(&logins));
  fclose(loginsFile);

  return 0;
}
