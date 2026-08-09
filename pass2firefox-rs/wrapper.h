#ifndef WRAPPER_H
#define WRAPPER_H

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

const char* get_error_c_wrapper();
SECItem make_sec_item(unsigned char* data, int len);

#endif
