#include <config.h>

#include "tools_passwd.h"
#include "kdbxmerge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

static struct option const long_options[] =
  {
    {"help",no_argument,NULL,'h'},
    {"version",no_argument,NULL, 'v'},
    {NULL,0,NULL,0}
  };

static void
kdbxmerge_options_init(struct kdbxmerge_options *x)
{
  x->dummy = 0;
}

void
usage(int status)
{
  printf("kdbxmerge [OPTION]... SOURCE... DEST\n\n");

  printf("  -h, --help     display this help and exit\n");
  printf("  -v, --version  output version information and exit\n");
  
  exit(status);
}

void
version(int status)
{
  printf("kdbxmerge (" PACKAGE_NAME ") " PACKAGE_VERSION "\n");
  printf("Copyright (C) 2018 Opal Raava <opalraava@riseup.net>\n");
  printf("Licence GPLv2+\n");
  printf("This is free software; you are free to change and redistribute it.\n");
  printf("There is NO WARANTY, to the extent permitted by law.\n");
  
  exit(status);
}

int
main(int argc, char* argv[])
{
  int c;
  struct kdbxmerge_options x;
  
  kdbxmerge_options_init(&x);

  while ((c = getopt_long(argc,argv,"hv",long_options, NULL)) != -1)
    {
      switch (c)
	{
	case 'v':
	  version(0);
	case 'h':
	  usage(EXIT_SUCCESS);
	default:
	  usage(EXIT_FAILURE);
	}
    }
  
  return 0;
}
