#ifndef INCLUDED_KDBXMERGE_H
#define INCLUDED_KDBXMERGE_H

#include <vector>
#include <string>

struct kdbxmerge_options
{
  int verbose;
};

int main_do_merge(std::vector<std::string> input_filenames, std::string output_filename,
		  struct kdbxmerge_options *x);

#endif

