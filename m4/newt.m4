#
# SYNOPSIS
#
#   MP_WITH_NEWT
#
# DESCRIPTION
#
#   Detect Newt Library
#
#   Defines HAVE_NEWT if Newt is found and appends library to LIBS
#   automatically.
#
# LICENCE
#
#   Copyright (c) 2018 Julius Seporaitis <julius@seporaitis.net>
#

AC_DEFUN([MP_WITH_NEWT], [
  mp_save_LIBS="$LIBS"
  NEWT_LIB=""
  AC_CACHE_CHECK([for Newt library], mp_cv_newt,
    [LIBS="$mp_save_libs -lnewt"
      AC_TRY_LINK(
        [#include <newt.h>],
        [int r = newtInit(); newtFinished(); ],
        mp_cv_newt=yes, mp_cv_newt=no)])
  if test "$mp_cv_newt" = yes
  then
    AC_DEFINE(HAVE_NEWT, 1, [Define if you have NEWT])
    NEWT_LIB="-lnewt"
    mp_cv_newt=yes
  fi
  LIBS="$LIBS -lnewt"
])dnl
