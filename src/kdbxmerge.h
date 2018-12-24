#ifndef INCLUDED_KDBXMERGE_H
#define INCLUDED_KDBXMERGE_H

#include <vector>
#include <string>

struct kdbxmerge_options
{
  /* as of now, we don't have options, and we don't want an empty struct */
  int dummy;
};

int main_do_merge(std::vector<std::string> input_filenames, std::string output_filename,
		  struct kdbxmerge_options *x);

#endif

