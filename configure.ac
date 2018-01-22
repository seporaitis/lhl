AC_INIT([hog], [0.1], [julius@seporaitis.net])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])

MP_WITH_CURSES
HAVE_CURS=$mp_cv_curses
if test x$HAVE_CURS != xyes; then
  AC_MSG_ERROR([hog requires the curses or ncurses library. Please install before compiling hog.])
fi

MP_WITH_CDK
HAVE_CDK=$mp_cv_cdk
if test x$HAVE_CDK != xyes; then
  AC_MSG_ERROR([hog requires the Curses Development Kit library. Please install before compiling hog.])
fi

AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT
