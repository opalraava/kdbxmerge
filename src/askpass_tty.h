/*
 * Copyright 2018 Bert van der Weerd, GPL version 2 licence.
 */

#ifndef INCLUDED_ASKPASS_TTY_H
#define INCLUDED_ASKPASS_TTY_H

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define burn(p,len) (memset((p), '\0', (len)))
  
  /* maximum size of a password/passphrase. 
     The actually maximum size is one char less than this. */
#define ASKPASS_TTY_PASSWORD_SIZE_MAX (1*1024)

  
  /*
     NOTE: you are responsible to free(key);

     'timeout' is in seconds, and be sure to free(*key) when done. For
     added security you could overwrite the key when done with it.
     'prompt' can be NULL, and `timeout` and `verify` can both be 0.
     
     return value: 
       if nonzero, an error has occured, and errno is (probably correctly) set.
       if negative, a malloc() system error has occured.
       if positive, one of the tree conditions below occured.
  */

#define ASKPASS_TTY_INPUT_TIMEOUT 1
#define ASKPASS_TTY_IO_ERROR 2	/* see errno value */
#define ASKPASS_TTY_VERIFY_DOESNT_MATCH 3
  
  int askpass_tty(const char* prompt, char** key, int timeout, int verify);

#ifdef __cplusplus
}
#endif

#endif
