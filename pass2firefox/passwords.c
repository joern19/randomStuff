#include "passwords.h"

char* malloc_and_copy_string(const char* src) {
  size_t len = strlen(src) + 1;
  char* result = malloc(len);
  memcpy(result, src, len);
  return result;
}

static int decrypt_file(const char *path, char **output, size_t *length) {
    gpgme_ctx_t ctx = NULL;
    gpgme_data_t in = NULL;
    gpgme_data_t out = NULL;

    gpgme_check_version(NULL);

    if (gpgme_new(&ctx)) {
      return -1;
    }
    gpgme_set_protocol(ctx, GPGME_PROTOCOL_OpenPGP);

    gpgme_error_t err;
    if ((err = gpgme_data_new_from_file(&in, path, 1))) {
      goto cleanup;
    }
    if(gpgme_data_new(&out)) {
      goto cleanup;
    }
    if (gpgme_op_decrypt(ctx, in, out)) {
      goto cleanup;
    }

    *length = gpgme_data_seek(out, 0, SEEK_END);
    gpgme_data_seek(out, 0, SEEK_SET);

    *output = malloc(*length + 1);

    gpgme_data_read(out, *output, *length);

    (*output)[*length] = '\0';

    gpgme_data_release(out);
    gpgme_data_release(in);
    gpgme_release(ctx);

    return 0;

    cleanup:
      fprintf(stderr, "gpg decrypt failed: %s (%s)\n", gpgme_strerror(err), gpgme_strsource(err));

      if (out)
        gpgme_data_release(out);
      if (in)
        gpgme_data_release(in);
      if (ctx)
        gpgme_release(ctx);
      return -1;
}

static void parse_yaml_fields(char *yaml, PasswordEntry *entry) {
  yaml_parser_t parser;

  yaml_parser_initialize(&parser);
  yaml_parser_set_input_string(&parser, (unsigned char *)yaml, strlen(yaml));

  char *key = NULL;

  yaml_event_t event;
  while (yaml_parser_parse(&parser, &event)) {
    if (event.type == YAML_SCALAR_EVENT) {
      char *value = (char *)event.data.scalar.value;

      if (key == NULL) {
        key = malloc_and_copy_string(value);
      } else {
        if (strcmp(key, "username") == 0) {
          entry->username = malloc_and_copy_string(value);
        } else if (strcmp(key, "url") == 0) {
          free(entry->url);
          entry->url = malloc_and_copy_string(value);
        }
        free(key);
        key = NULL;
      }
    }

    if (event.type == YAML_STREAM_END_EVENT) {
      break;
    }

    yaml_event_delete(&event);
  }
  yaml_parser_delete(&parser);
  free(key);
}

static int load_file(const char *path, PasswordEntry *entry) {
    char *content = NULL;
    size_t length = 0;

    if (decrypt_file(path, &content, &length) != 0) {
      printf("Failed to decrypt file!");
      return -1;
    }

    int passwordLength = (int)(strchr(content, '\n') - content);
    entry->password = malloc(passwordLength+1);
    memcpy(entry->password, content, passwordLength);
    entry->password[passwordLength+1] = '\0';

    parse_yaml_fields(content+passwordLength, entry);

    free(content);

    return 0;
}

char* join_paths(const char* absolute, const char* relative) {
  size_t absolute_len = strlen(absolute);
  size_t relative_len = strlen(relative);
  char *dir_path = malloc(absolute_len + relative_len + 2);
  memcpy(dir_path, absolute, absolute_len);
  dir_path[absolute_len] = '/';
  memcpy(dir_path + absolute_len+1, relative, relative_len);
  dir_path[absolute_len + relative_len + 1] = '\0';
  return dir_path;
}

static void scan_dir(const char *absolute, const char *relative, PasswordList *list) {
  char *dir_path = join_paths(absolute, relative);
  printf("Scanning '%s' for web passwords.\n", dir_path);

  DIR *d = opendir(dir_path);
  if (!d) {
    printf("This is not a dir or it could not be opened: %s", dir_path);
    return;
  }
  struct dirent *de;
  while ((de = readdir(d))) {
    if (de->d_name[0] == '.') {
      continue;
    }

    char *relative_with_file = join_paths(relative, de->d_name);
    printf("Looking at: %s\n", relative_with_file);
    if (de->d_type == 4) {
      scan_dir(absolute, relative_with_file, list);
      continue;
    }

    // Not quite correct, but whatever.
    if (!strstr(de->d_name, ".gpg")) {
      continue;
    }

    list->entries = realloc(list->entries, sizeof(PasswordEntry) * (list->count + 1));
    PasswordEntry *e = &list->entries[list->count];
    memset(e, 0, sizeof(*e));

    e->path = malloc_and_copy_string(relative_with_file);
    if (strcmp(relative, "web") == 0) {
      size_t len = strlen(de->d_name) - 3; // +1 (\0) -4 (.gpg)
      e->domain = malloc(len);
      memcpy(e->domain, de->d_name, len);
      e->domain[len-1] = '\0';
    } else {
      char* beginning = strrchr(relative, '/')+1;
      e->domain = malloc(strlen(beginning));
      strcpy(e->domain, beginning);
    }
    const char* url_prefix = "https://";
    size_t url_prefix_len = strlen(url_prefix);
    size_t domain_len = strlen(e->domain);
    e->url = malloc(url_prefix_len + domain_len + 1);
    memcpy(e->url, url_prefix, url_prefix_len);
    memcpy(e->url+url_prefix_len, e->domain, domain_len + 1);

    // TODO: Fail if this fails.
    load_file(join_paths(dir_path, de->d_name), e);
    list->count++;
  }
  closedir(d);
}

int passwords_load(PasswordList *list) {
  const char *store = getenv("PASSWORD_STORE_DIR");
  if (!store) {
    printf("PASSWORD_STORE_DIR is not set!!");
    return -1;
  }

  char* suffix = "/web";
  char* path = malloc(strlen(store) + strlen(suffix) + 1);
  strcpy(path, store);
  strcat(path, suffix);

  scan_dir(store, "web", list);

  return 0;
}

void passwords_free(PasswordList *list) {
  for (size_t i = 0; i < list->count; i++) {
    PasswordEntry *e = &list->entries[i];
    free(e->path);
    free(e->domain);
    free(e->password);
    free(e->username);
    free(e->url);
  }
  free(list->entries);
  memset(list, 0, sizeof(*list));
}