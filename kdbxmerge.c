#include <config.h>

#include "tools_passwd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char* argv[])
{
  char* bagger;
  
  if (tools_get_key_tty(NULL,&bagger,0,0))
    return 1;
  
  printf("%s\n", bagger);
   
  return 0;
}
