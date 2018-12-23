/*
 * Copyright 2018 Bert van der Weerd, GPL version 2 licence.
 */

#ifndef INCLUDED_ASKPASS_TTY_H
#define INCLUDED_ASKPASS_TTY_H
#ifdef __cplusplus
extern "C" {
#endif

  /* maximum size of a password/passphrase. 
     The actually maximum size is one char less than this. */
#define ASKPASS_TTY_PASSWORD_SIZE_MAX (1*1024)

  /* 'timeout' is in seconds, and be sure to free(*key) when done. For
     added security you could overwrite the key when done with it.
     'prompt' can be NULL, and `timeout` and `verify` can both be 0 */
  int askpass_tty(const char* prompt, char** key, int timeout, int verify);
  
#ifdef __cplusplus
}
#endif
#endif

