#include <config.h>

#include "kdbxmerge.h"
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
#include "core/Group.h"
#include "core/Uuid.h"
#include "crypto/Crypto.h"
#include "format/KeePass2Reader.h"
#include "format/KeePass2Writer.h"
#include "keys/CompositeKey.h"
#include "keys/PasswordKey.h"



static int main_do_merge(std::vector<std::string> input_filenames, std::string output_filename,
			 struct kdbxmerge_options *x);



static struct option const long_options[] =
  {
    {"verbose",no_argument,NULL,'v'},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
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
  printf(HELP_OPTION_DESCRIPTION);
  printf(VERSION_OPTION_DESCRIPTION);

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

  while ((c = getopt_long(argc,argv,"v",long_options, NULL)) != -1)
    {
      switch (c)
	{
	case 'v':
	  x.verbose = 1;
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
      printf("kdbxmerge: missing file operands\n");
      printf("Try 'kdbxmerge --help' for more information.\n");
      exit(EXIT_FAILURE);
    }

  std::vector<std::string> input_filenames(argv + optind, argv + argc - 1);
  std::string output_filename = argv[argc - 1];

  return main_do_merge(input_filenames, output_filename, &x);
}



/* 
 * main_do_merge() implementation
 */


// this one performs the actual messy merging process
static void merge(Database* output, Database* db, const char* groupname, struct kdbxmerge_options *x);

// convienience helper for askpass_tty()
static inline char* askpass(const std::string& prompt, bool verify = false, int timeout = 0)
{
  char* key = NULL;
  
  int err = askpass_tty(prompt.c_str(), &key, timeout, verify?1:0);
  if (err != 0)
    throw err;
  
  return key;
}


/***
   This is, in a way, the actual main function.

   At entry, we know the filenames and options, and when this function returns, main() returns
   with it's return value.

   This function calls merge() to actually perform the messy merging process.
*/

int main_do_merge(std::vector<std::string> input_filenames, std::string output_filename,
		  struct kdbxmerge_options *x)
{
  
  // First we check if our input files exist, and our output files does not exist.
  // We perform this check so that we don't needlessly ask the user for passwords when
  // we know the program will fail.
  
  for (auto name : input_filenames)
    {
      if (!std::ifstream(name.c_str()).good())
	{
	  std::cout << "kdbxmerge: error: " << name << " does not exist.\n";
	  return EXIT_FAILURE;
	}
    }
  
  if (std::ifstream(output_filename.c_str()).good())
    {
      std::cout << "kdbxmerge: error: output file "
		<< output_filename << " exists. Not overwriting existing file.\n";
      return EXIT_FAILURE;
    }
  

  // Create the output database
  Database* output = new Database();

  // Loop over the input databases to get their contents
  for (auto name : input_filenames)
    {
      QFile db_file(name.c_str());
      assert(db_file.exists());	// we checked existing or not earlier.
      
      if (!db_file.open(QIODevice::ReadOnly)) {
	std::cout << "kdbxmerge: Unable to open file " << name << "\n";
	return EXIT_FAILURE;
      }
      
      CompositeKey key;
      PasswordKey password;
      char* pass = askpass(std::string("Enter passphrase for ") + name + ": "); // should try-catch
      password.setPassword(pass);
      key.addKey(password);
      burn(pass,strlen(pass));
      free(pass);

      KeePass2Reader reader;
      Database* db = reader.readDatabase(&db_file,key);

      if (reader.hasError()) {
	QByteArray ba = reader.errorString().toLocal8Bit();
	const char* s = ba.data();
	
	std::cout << "kdbxmerge: error reading from "
		  << name << ": " << s << std::endl;
	delete db;
	delete output;
	return EXIT_FAILURE;
      }
      
      merge(output, db, name.c_str(),x);
      delete db;
    }

  // Write output database
  CompositeKey key;
  PasswordKey password;
  
  try {
    char* pass = askpass(std::string("Enter new passphrase for ") + output_filename + ": ",true);
    password.setPassword(pass);
    key.addKey(password);
    burn(pass,strlen(pass));
    free(pass);
  } catch (int err) {
    delete output;
    if (err < 0) { std::cout << "kdbxmerge: out of memory\n"; return EXIT_FAILURE; }
    if (err == 2) { std::cout << "kdbxmerge: i/o error with tty\n"; return EXIT_FAILURE; }
    if (err == ASKPASS_TTY_VERIFY_DOESNT_MATCH) {
      std::cout << "kdbxmerge: the two passwords do not match\n"; return EXIT_FAILURE;
    }
  }
  
  
  if (!output->setKey(key)) {
    std::cout << "kdbxmerge: error writing output: "
	      << output_filename << ": " << "Unable to calculate master key" << std::endl;
    delete output;
    return EXIT_FAILURE;
  }

  KeePass2Writer writer;
  
  writer.writeDatabase(output_filename.c_str(), output);
  if (writer.hasError())
    {
      QByteArray ba = writer.errorString().toLocal8Bit();
      const char* s = ba.data();
    
      std::cout << "kdbxmerge: error writing output: "
		<< output_filename << ": " << s << std::endl;
      delete output;
      return EXIT_FAILURE;
    }
  
  delete output;
  return EXIT_SUCCESS;
}


/*
 * Actual merge process, all done by Group::clone()
 */

static void merge(Database* output, Database* db, const char* groupname, struct kdbxmerge_options *x)
{
  Group* g = db->rootGroup()->clone();
  g->setName(groupname);
  g->setParent(output->rootGroup());
  
  (void)x;			// perhaps some statistics could be printed here
}

