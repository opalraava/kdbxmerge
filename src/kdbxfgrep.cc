#include <config.h>

#include "kdbxfgrep.h"
#include "askpass_tty.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <getopt.h>

#include <QCoreApplication>
#include <QFile>

#include "core/Database.h"
#include "core/Entry.h"
#include "core/EntrySearcher.h"
#include "core/Group.h"
#include "core/Uuid.h"
#include "crypto/Crypto.h"
#include "format/KeePass2Reader.h"
#include "format/KeePass2Writer.h"
#include "keys/CompositeKey.h"
#include "keys/PasswordKey.h"

static int main_do_kdbxfgrep(std::string db_name, std::string search_string, struct kdbxfgrep_options *x);
static int kdbxfgrep(Database* db, const char* search_string, struct kdbxfgrep_options *x);

static struct option const long_options[] =
  {
    {"case-insensitve",no_argument,NULL,'i'},
    {"include-password",no_argument,NULL,'p'},
    {"include-url",no_argument,NULL,'u'},
    {"password",no_argument,NULL,'P'},
    {"url",no_argument,NULL,'U'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {NULL,0,NULL,0}
  };

static void
kdbxfgrep_options_init(struct kdbxfgrep_options *x)
{
  x->case_sensitive = 0;
  x->include_password = 0;
  x->include_url = 0;
  x->on_unique_print_password = 0;
  x->on_unique_print_url = 0;
}

void
usage()
{
  printf("kdbxfgrep [OPTION]... FILE SEARCHTERM\n\n");

  printf("  -i, --case-sensitive    perform a case sensitve search (default is case insensitve)\n");
  printf("  -p, --include-password  print the password in the list of results\n");
  printf("  -u, --include-url       print the URL, if any, in the list of results\n");
  printf("  -P, --password if one unique entry is found, print *only* the password to stdout\n");
  printf("  -U, --url      if one unique entry is found, print *only* the URL to stdout\n");
  printf(HELP_OPTION_DESCRIPTION);
  printf(VERSION_OPTION_DESCRIPTION);

  printf("\n");
  printf("This program allows you to search for kdbx entries on the commandline.\n");
  printf("the [-P] and [-U] options can be used to stuff a password or url into another commandline.\n");
  printf("for example: mysqladmin processlist -u root -p$(kdbxfgrep -P ~/sqlpassdb.kdbx root)\n");
  printf("\n");
  printf("SECURITY WARNING: Know what you're doing, printing passwords to stdout can be dangerous.\n");
  printf("Problems can happen if you're logging your console output with 'script', for example.\n");
  printf("\n");
}

void
version()
{
  printf("kdbxfgrep (" PACKAGE_NAME ") " PACKAGE_VERSION "\n");
  printf("Copyright (C) 2018 Opal Raava <opalraava@riseup.net>\n");
  printf("Licence GPLv2+\n");
  printf("This is free software; you are free to change and redistribute it.\n");
  printf("There is NO WARANTY, to the extent permitted by law.\n");
}


int
main(int argc, char* argv[])
{
  int c;
  struct kdbxfgrep_options x;
  
  kdbxfgrep_options_init(&x);

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

  while ((c = getopt_long(argc,argv,"ipuPU",long_options, NULL)) != -1)
    {
      switch (c)
	{
	case 'i':
	  x.case_sensitive = 1;
	  break;
	case 'p':
	  x.include_password = 1;
	  break;
	case 'u':
	  x.include_url = 1;
	  break;
	case 'P':
	  x.on_unique_print_password = 1;
	  break;
	case 'U':
	  x.on_unique_print_url = 1;
	  break;
	  
	case GETOPT_HELP_CHAR:
	  usage();
	  return EXIT_SUCCESS;
	  
	case GETOPT_VERSION_CHAR:
	  version();
	  return EXIT_SUCCESS;
	  
	default:
	  usage();
	  return EXIT_FAILURE;
	}
    }

  if (argc - optind <= 1)
    {
      printf("kdbxfgrep: missing commandline parameters\n");
      printf("Try 'kdbxfgrep --help' for more information.\n");
      return EXIT_FAILURE;
    }
  if (argc - optind > 2)
    {
      printf("kdbxfgrep: extra garbage on the commandline\n");
      printf("Try 'kdbxfgrep --help' for more information.\n");
      return EXIT_FAILURE;
    }
  if (x.on_unique_print_password)
    {
      if (x.on_unique_print_url || x.include_password || x.include_url) {
	printf("kdbxfgrep: incompatible commandline options\n");
	printf("Try 'kdbxfgrep --help' for more information.\n");
	return EXIT_FAILURE;
      }
    }
  if (x.on_unique_print_url)
    {
      if (x.on_unique_print_password || x.include_password || x.include_url) {
	printf("kdbxfgrep: incompatible commandline options\n");
	printf("Try 'kdbxfgrep --help' for more information.\n");
	return EXIT_FAILURE;
      }
    }

  std::string db_name = argv[optind];
  std::string search_string = argv[argc-1];
  
  return main_do_kdbxfgrep(db_name, search_string, &x);
}



static int main_do_kdbxfgrep(std::string db_name, std::string search_string,
			     struct kdbxfgrep_options *x)
{
  // check if the database file exists
  if (!std::ifstream(db_name.c_str()).good())
    {
      std::cout << "kdbxfgrep: error: database file " << db_name << " does not exist.\n";
      return EXIT_FAILURE;
    }

  // try open the database file
  QFile db_file(db_name.c_str());
  assert(db_file.exists());	// we checked existing or not earlier.
      
  if (!db_file.open(QIODevice::ReadOnly)) {
    std::cout << "kdbxfgrep: Unable to open file " << db_name << "\n";
    return EXIT_FAILURE;
  }

  // get the key
  CompositeKey key;
  PasswordKey password;
  char* pass = NULL;
  int retval = askpass_tty((std::string("Enter passphrase for ") + db_name + ": ").c_str(),
			   &pass, 0,0);
  if (retval != 0) {
    if (retval < 0 || retval == ASKPASS_TTY_IO_ERROR) {
      perror(db_name.c_str());
      return EXIT_FAILURE;
    }
    else {
      std::cout << db_name << ": error: Unable to read password from tty\n";
      return EXIT_FAILURE;
    }
  }
  password.setPassword(pass);
  key.addKey(password);
  burn(pass,strlen(pass));
  free(pass);

  // readDatabase()
  KeePass2Reader reader;
  Database* db = reader.readDatabase(&db_file,key);

  if (reader.hasError()) {
    QByteArray ba = reader.errorString().toLocal8Bit();
    const char* s = ba.data();
	
    std::cout << "kdbxfgrep: error reading from " << db_name << ": " << s << std::endl;
    delete db;
    return EXIT_FAILURE;
  }

  retval = kdbxfgrep(db,search_string.c_str(),x);
  delete db;
  return retval;
}

static inline std::string make_str(QString str) {
  return std::string(str.toLocal8Bit().data());
}

static int kdbxfgrep(Database* db, const char* search_string, struct kdbxfgrep_options *x)
{
  Qt::CaseSensitivity sensitivity;
  
  if (x->case_sensitive) {
    sensitivity = Qt::CaseSensitive;
  }
  else {
    sensitivity = Qt::CaseInsensitive;
  }

  QList<Entry*> searchResult =
    EntrySearcher().search(search_string, db->rootGroup(), sensitivity);

  if (x->on_unique_print_password)
    {
      if (searchResult.count() == 1)
	{
	  Entry* entry = *(searchResult.begin());
	  std::string s = make_str(entry->password());
	  
	  if (!s.empty())
	    std::cout << s << std::endl;
	}
    }
  else if (x->on_unique_print_url)
    {
      if (searchResult.count() == 1)
	{
	  Entry* entry = *(searchResult.begin());
	  std::string s = make_str(entry->url());

	  if (!s.empty())
	    std::cout << s << std::endl;
	}
    }
  else
    {  
      for (Entry* entry : searchResult)
	{
	  std::cout << make_str(entry->title()) << ":" << make_str(entry->username());
	  if (x->include_password) {
	    std::cout << ":" << make_str(entry->password());
	  }
	  if (x->include_url) {
	    std::cout << ":" << make_str(entry->url());
	  }
	  std::cout << std::endl;
	}
    }
  
  return 0;
}
  
