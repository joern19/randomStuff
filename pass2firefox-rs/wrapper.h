#include <nss.h>
#include <pk11pub.h>
#include <pk11sdr.h>
#include <secitem.h>
#include <secerr.h>
#include <secport.h>
#include <prerror.h>
#include <seccomon.h>

#include <gpgme.h>
#include <gpg-error.h>

char* get_error_c_wrapper() {
  return PORT_ErrorToString(PORT_GetError());
}

SECItem make_sec_item(unsigned char* data, int len) {
  SECItem item = { siBuffer, data: data, len: len };
  return item;
}
