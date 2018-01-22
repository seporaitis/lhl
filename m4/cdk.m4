#
# SYNOPSIS
#
#   MP_WITH_CDK
#
# DESCRIPTION
#
#   Detect Curses Development Kit.
#
#   Defines HAVE_CDK if CDK is found and appends library to LIBS
#   automatically.
#
# LICENCE
#
#   Copyright (c) 2018 Julius Seporaitis <julius@seporaitis.net>
#

AC_DEFUN([MP_WITH_CDK], [
  mp_save_LIBS="$LIBS"
  CDK_LIB=""
  AC_CACHE_CHECK([for CDK library], mp_cv_cdk,
    [LIBS="$mp_save_libs -lcdk -lncurses"
      AC_TRY_LINK(
        [#include <cdk.h>],
        [CDKSCREEN *s = initCDKScreen(stdscr); ],
        mp_cv_cdk=yes, mp_cv_cdk=no)])
  if test "$mp_cv_cdk" = yes
  then
    AC_DEFINE(HAVE_CDK, 1, [Define if you have CDK])
    CDK_LIB="-lcdk"
    mp_cv_cdk=yes
  fi
  LIBS="$LIBS -lcdk"
])dnl
