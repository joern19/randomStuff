#include "crypt.h"

SECItem makeItem(char* data) {
  int len = 0;
  if (data != NULL) {
    len = strlen(data);
  }
  SECItem item = {
    .type = siBuffer,
    .data = (unsigned char *) data,
    .len  = len,
  };
  return item;
}

char* encode(char* binary, size_t binaryLength) {
  char* result = PL_Base64Encode(binary, binaryLength, NULL);
  if (result == NULL) {
    fprintf(stderr, "base64 encode failed!\n");
  }
  return result;
}

SECItem decode(char* encoded) {
  size_t decodedLen = (strlen(encoded) / 4) * 3;
  if (encoded[strlen(encoded)-1] == '=') {
    decodedLen--;
  }
  if (encoded[strlen(encoded)-2] == '=') {
    decodedLen--;
  }
  unsigned char *decoded = malloc(decodedLen);
  PL_Base64Decode(encoded, strlen(encoded), (char *)decoded);
  if (decoded == NULL) {
    fprintf(stderr, "base64 decode failed!\n");
  }
  SECItem encrypted = {
    .type = siBuffer,
    .data = decoded,
    .len = decodedLen
  };
  return encrypted;
}

SECItem decrypt(SECItem encrypted) {
  SECItem plaintext = makeItem(NULL);
  SECStatus decryptResult = PK11SDR_Decrypt(&encrypted, &plaintext, NULL);
  if (decryptResult != SECSuccess) {
    fprintf(stderr, "SDR decrypt failed: %d %d\n", decryptResult, PORT_GetError());
    return makeItem(NULL);
  }
  return plaintext;
}

SECItem encrypt(PK11SlotInfo* slot, SECItem plaintext) {
  SECItem encrypted = makeItem(NULL);
  // Empty keyid means "use the SDR default key".
  SECItem keyid = makeItem(NULL);

  SECStatus encryptStatus = PK11SDR_EncryptWithMechanism(slot, &keyid, CKM_AES_CBC, &plaintext, &encrypted, NULL);
  if (encryptStatus!= SECSuccess) {
    fprintf(stderr, "SDR encrypt failed\n");
    return makeItem(NULL);
  }
  return encrypted;
} 

char* encryptEncode(PK11SlotInfo* slot, char* plaintext) {
  SECItem encrypted = encrypt(slot, makeItem(plaintext));
  char* result = encode((char*) encrypted.data, encrypted.len);
  SECITEM_FreeItem(&encrypted, PR_FALSE);
  return result;
}