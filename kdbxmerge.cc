#include <config.h>

#include <iostream>
#include <boost/version.hpp>
#include <boost/algorithm/string.hpp>

void usage(void);

int
main(int argc, char* argv[])
{
  std::cout
    << "Using Boost "     
    << BOOST_VERSION / 100000     << "." // major version
    << BOOST_VERSION / 100 % 1000 << "." // minor version
    << BOOST_VERSION % 100		 // patch level
    << std::endl;


  usage();
  
  return 0;
}

void usage(void)
{
  std::cout
    << "Usage: kdbxmerge [-p password] [-k keyfile] input.kdbx... [-p password] [-k keyfile] output.kdbx\n"
    << "\n"
    << "Example: kdbxmerge -p a a.kdbx -p b b.kdbx -p c c.kdbx -p target target.kdbx\n"
    ;
  
}
