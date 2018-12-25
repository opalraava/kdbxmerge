#include <config.h>

#include "kdbxmerge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include <QCoreApplication>
#include "crypto/Crypto.h"



static struct option const long_options[] =
  {
    {"verbose",no_argument,NULL,'v'},
    {"help",no_argument,NULL,'h'},
    {"version",no_argument,NULL, 'V'},
    {NULL,0,NULL,0}
  };

static void
kdbxmerge_options_init(struct kdbxmerge_options *x)
{
  x->verbose = 0;
}

void
usage()
{
  printf("kdbxmerge [OPTION]... SOURCE... DEST\n\n");

  printf("  -v, --verbose  explain what's being done\n");
  printf("  -h, --help     display this help and exit\n");
  printf("  -V, --version  output version information and exit\n");

  printf("\n");
  printf("Use this program to merge several KeePass .kdbx databases into a new,\n");
  printf("nonexistent single one. The last argument is a newly created .kdbx\n");
  printf("database with the contents of the others in subgroups of the root node.\n");
  printf("\n");
}

void
version()
{
  printf("kdbxmerge (" PACKAGE_NAME ") " PACKAGE_VERSION "\n");
  printf("Copyright (C) 2018 Opal Raava <opalraava@riseup.net>\n");
  printf("Licence GPLv2+\n");
  printf("This is free software; you are free to change and redistribute it.\n");
  printf("There is NO WARANTY, to the extent permitted by law.\n");
}

int
main(int argc, char* argv[])
{
  int c;
  struct kdbxmerge_options x;
  
  kdbxmerge_options_init(&x);

  QCoreApplication app(argc, argv);

  // if we just type the command, we want help.
  if (argc == 1) {
    usage();
    return EXIT_SUCCESS;
  }
  
  if (!Crypto::init()) {
    qFatal("Fatal error while testing the cryptographic functions:\n%s",
	   qPrintable(Crypto::errorString()));
  }

  while ((c = getopt_long(argc,argv,"vhV",long_options, NULL)) != -1)
    {
      switch (c)
	{
	case 'v':
	  x.verbose = 1;
	  break;
	  
	case 'V':
	  version();
	  return EXIT_SUCCESS;
	  
	case 'h':
	  usage();
	  return EXIT_SUCCESS;
	  
	default:
	  usage();
	  return EXIT_FAILURE;
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

  return main_do_merge(input_filenames, output_filename, &x);
}
