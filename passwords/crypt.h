#ifndef CRYPT_H
#define CRYPT_H

#include <stdio.h>
#include <nss.h>
#include <pk11sdr.h>
#include <plbase64.h>

char* encode(char* binary, size_t binaryLength);
SECItem decode(char* encoded);
SECItem decrypt(SECItem encrypted);
SECItem encrypt(PK11SlotInfo* slot, SECItem plaintext);
char* encryptEncode(PK11SlotInfo* slot, char* plaintext);

#endif