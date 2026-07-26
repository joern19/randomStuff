#ifndef CRYPT_H
#define CRYPT_H

#include <stdio.h>
#include <nss.h>
#include <pk11sdr.h>

char* encode(char* binary, size_t binaryLength);
SECItem decode(char* encoded);
SECItem decrypt(SECItem encrypted);
SECItem encrypt(PK11SlotInfo* slot, SECItem plaintext);

#endif