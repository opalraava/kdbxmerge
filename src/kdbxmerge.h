#ifndef INCLUDED_KDBXMERGE_H
#define INCLUDED_KDBXMERGE_H

#include <climits>

struct kdbxmerge_options
{
  int verbose;
};

enum
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

#define GETOPT_HELP_OPTION_DECL			\
  "help", no_argument, NULL, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, NULL, GETOPT_VERSION_CHAR

#define HELP_OPTION_DESCRIPTION \
  "      --help     display this help and exit\n"
#define VERSION_OPTION_DESCRIPTION \
  "      --version  output version information and exit\n"

#endif
