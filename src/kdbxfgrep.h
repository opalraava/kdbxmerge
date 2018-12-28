#ifndef INCLUDED_KDBXFGREP_H
#define INCLUDED_KDBXFGREP_H

#include <climits>

struct kdbxfgrep_options
{
  int case_sensitive;
  int include_password;
  int include_url;
  int on_unique_print_password;
  int on_unique_print_url;
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
