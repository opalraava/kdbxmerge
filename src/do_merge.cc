#include "askpass_tty.h"
#include "kdbxmerge.h"

#include <cassert>

#include <iostream>
#include <fstream>

#include <QFile>

#include "core/Database.h"
#include "keys/PasswordKey.h"
#include "keys/CompositeKey.h"
#include "format/KeePass2Reader.h"
#include "format/KeePass2Writer.h"

static char* askpass(const std::string& prompt, bool verify = false, int timeout = 0);
static void merge(Database* output, Database* db, const char* groupname, struct kdbxmerge_options *x);




/***
   This is, in a way, the actual main function.

   At entry, we know the filenames and options, and when this function returns, main() returns
   with it's return value.

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
      char* pass = askpass(std::string("Enter passphrase for ") + name + ": ");
      password.setPassword(pass);
      key.addKey(password);
      burn(pass,strlen(pass));
      free(pass);

      KeePass2Reader reader;
      Database* db = reader.readDatabase(&db_file,key);

      if (reader.hasError()) {
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
  char* pass = askpass(std::string("Enter new passphrase for ") + output_filename + ": ",true);
  password.setPassword(pass);
  key.addKey(password);
  burn(pass,strlen(pass));
  free(pass);
  
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
  return 0;
}


/*
 * Static functions
 */


static char* askpass(const std::string& prompt, bool verify, int timeout)
{
  char* key = NULL;
  
  int err = askpass_tty(prompt.c_str(), &key, timeout, verify?1:0);
  if (err != 0)
    throw err;
  
  return key;
}


static void merge(Database* output, Database* db, const char* groupname, struct kdbxmerge_options *x)
{
  (void)output;
  (void)db;
  (void)groupname;
  (void)x;
}

