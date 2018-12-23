#include <config.h>

#include "tools_passwd.h"
#include "kdbxmerge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include <vector>
#include <string>
#include <iostream>

#include <QCoreApplication>
#include "crypto/Crypto.h"








static struct option const long_options[] =
  {
    {"help",no_argument,NULL,'h'},
    {"version",no_argument,NULL, 'V'},
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
  printf("  -V, --version  output version information and exit\n");

  printf("\n");
  printf("Use this program to merge several KeePass .kdbx databases into a new,\n");
  printf("nonexistent single one. The last argument is a newly created .kdbx\n");
  printf("database with the contents of the others in subgroups of the root node\n");
  printf("\n");
  
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

  QCoreApplication app(argc, argv);
  
  if (!Crypto::init()) {
    qFatal("Fatal error while testing the cryptographic functions:\n%s",
	   qPrintable(Crypto::errorString()));
  }

  while ((c = getopt_long(argc,argv,"hV",long_options, NULL)) != -1)
    {
      switch (c)
	{
	case 'V':
	  version(0);
	  return 0;
	case 'h':
	  usage(EXIT_SUCCESS);
	  return 0;
	default:
	  usage(EXIT_FAILURE);
	  return 0;
	}
    }

  if (argc - optind <= 1)
    {
      printf("kdbxmerge: missing file operands\n");
      printf("Try 'kdbxmerge --help' for more information.\n");
      exit(EXIT_FAILURE);
    }



  std::vector<std::string> input_filenames(argv + optind, argv + argc - 1);
  std::string output_filename = argv[argc - 1];
  
  std::string prompt;
  for (auto input_filename : input_filenames)
    {
      prompt = std::string("Enter passphrase for ") + input_filename + ": ";
      char* key = NULL;
      int retval = tools_get_key_tty(prompt.c_str(),&key,0,0);
    }

  prompt = std::string("Enter new passphrase for ") + output_filename + ": ";
  char* output_key = NULL;
  int retval = tools_get_key_tty(prompt.c_str(),&output_key,0,1);

  return 0;
}
