AC_INIT([kdbxmerge], [0.1.0], [opalraava@riseup.net], [], [https://github.com/opalraava/kbdxmerge/])
AC_CONFIG_SRCDIR(src/kdbxmerge.cc)
AC_PREREQ(2.59)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CXX

AX_CHECK_ZLIB
AX_LIB_GCRYPT([yes])

AX_HAVE_QT
if test "x$have_qt" == "xno"; then
  AC_MSG_ERROR([Qt5 not detected])
fi

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
