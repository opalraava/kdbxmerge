AC_INIT([kdbxmerge], [0.1.0], [opalraava@riseup.net], [], [https://github.com/opalraava/kdbxmerge/])
AC_CONFIG_SRCDIR(kdbxmerge.cc)
AC_PREREQ(2.59)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

AC_PROG_CXX
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_17

AX_BOOST_BASE([],, [AC_MSG_ERROR([kdbxmerge needs Boost, but it was not found])])
AX_BOOST_SYSTEM


AC_CONFIG_FILES(Makefile)
AC_OUTPUT
