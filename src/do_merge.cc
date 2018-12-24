#include "askpass_tty.h"
#include "kdbxmerge.h"

#include <iostream>
#include <fstream>

/* This is in a way the actual main function. At entry, we know the filenames and options */
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
	  exit(EXIT_FAILURE);
	}
    }
  
  if (std::ifstream(output_filename.c_str()).good())
    {
      std::cout << "kdbxmerge: error: output file "
		<< output_filename << " exists. Not overwriting existing file.\n";
      exit(EXIT_FAILURE);
    }
  

  
  ////////////////////////////
  
  int retval;
  
  std::string prompt;
  for (auto input_filename : input_filenames)
    {
      prompt = std::string("Enter passphrase for ") + input_filename + ": ";
      char* key = NULL;
      retval = askpass_tty(prompt.c_str(),&key,0,0);
      burn(key,strlen(key));
      free(key);
    }

  prompt = std::string("Enter new passphrase for ") + output_filename + ": ";
  char* output_key = NULL;
  retval = askpass_tty(prompt.c_str(),&output_key,0,1);
  burn(output_key,strlen(output_key));
  free(output_key);

  
  
  (void) retval;
  (void) x->dummy; 		// not having paramaters of yet  
  return 0;
}
