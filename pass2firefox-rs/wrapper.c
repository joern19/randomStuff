#include "wrapper.h"

const char* get_error_c_wrapper() {
  return PORT_ErrorToString(PORT_GetError());
}

SECItem make_sec_item(unsigned char* data, int len) {
  SECItem item = { siBuffer, data: data, len: len };
  return item;
}
