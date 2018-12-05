#include "tools_passwd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int
main()
{
  char* key;

  char* prompt = NULL;
  int verify = 0;
  int timeout = 0;
  
  int r = tools_get_key_tty(prompt, &key, timeout, verify);
  
  if (verify && r == -EPERM)
    printf("Passwords do not match.\n");
  else if (r == -EINVAL)
    printf("error: tools_get_key() failed with -EINVAL, so interactive_pass() returned non-zero value. (errno=%d) -> %s\n", errno, strerror(errno));
  else if (r) 
    printf("error: tools_get_key() returned non-zero value: %d: %s\n", r, strerror(-r));
  else
    printf("\n---\nGot key \"%s\".\n", key);

  free(key);			/* don't forget this, perhaps bzero() it */
  
  return EXIT_SUCCESS;
}
