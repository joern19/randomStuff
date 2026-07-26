#ifndef PASSWORDS_H
#define PASSWORDS_H

#include <stddef.h>
#include <yaml.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <gpgme.h>

typedef struct {
    char* path; // full pass path that may be used as the guid.
    char* domain;
    char* password;
    char* username;
    char* url;
} PasswordEntry;

typedef struct {
    PasswordEntry *entries;
    size_t count;
} PasswordList;

int passwords_load(PasswordList *list);

/*
 * Frees all memory allocated by passwords_load().
 */
void passwords_free(PasswordList *list);

#endif