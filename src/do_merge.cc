#include "askpass_tty.h"
#include "kdbxmerge.h"

/* This is in a way the actual main function. At entry, we know the filenames and options */
int main_do_merge(std::vector<std::string> input_filenames, std::string output_filename,
		  struct kdbxmerge_options *x)
{
  int retval;
  (void) x->dummy; 		// not having paramaters of yet
  
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
  burn(key,strlen(key));
  free(key);
  (void) retval;
  
  return 0;
}
