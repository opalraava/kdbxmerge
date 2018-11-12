/*
 * Copyright 2018 Bert van der Weerd, GPL version 2 licence.
 */

#ifndef INCLUDED_TOOLS_PASSWD_H
#define INCLUDED_TOOLS_PASSWD_H

/* maximum size of a password/passphrase. The actually maximum size is one char less than this. */
#define TOOLS_PASSWORD_SIZE_MAX (1*1024)

/* 'timeout' is in seconds, be sure to free(*key) when done.
 * 'prompt' can be NULL, and timeout and verify can both be 0 */
int tools_get_key_tty(const char* prompt, char** key, int timeout, int verify);

#endif

