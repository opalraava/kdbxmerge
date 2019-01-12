AC_INIT([kdbxmerge], [0.1.0], [opalraava@riseup.net], [], [https://github.com/opalraava/kbdxmerge/])
AC_CONFIG_SRCDIR(src/kdbxmerge.cc)
AC_PREREQ(2.59)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CXX

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11

# isues with zlib under mingw64
# AX_CHECK_ZLIB

# snippet taken from : https://github.com/simsong/tcpflow/blob/master/configure.ac
#
# ZLIB is required for decompressing
#  Note you cannot put comments in the AC_MSG_ERROR for some reason
AC_CHECK_LIB([z],[uncompress],,
  AC_MSG_ERROR([zlib libraries not installed; try installing zlib-dev zlib-devel zlib1g-dev or libz-dev]))
AC_CHECK_HEADERS([zlib.h])




AX_LIB_GCRYPT([yes])

AX_HAVE_QT
if test "x$have_qt" == "xno"; then
  AC_MSG_ERROR([Qt5 not detected])
fi

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
