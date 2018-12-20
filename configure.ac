AC_INIT([lylog], [0.1], [julius@seporaitis.net])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])

MP_WITH_CURSES
HAVE_CURS=$mp_cv_curses
if test x$HAVE_CURS != xyes; then
  AC_MSG_ERROR([lylog requires the curses or ncurses library. Please install before compiling lylog.])
fi

MP_WITH_NEWT
HAVE_NEWT=$mp_cv_newt
if test x$HAVE_NEWT != xyes; then
  AC_MSG_ERROR([lylog requires the Newt library. Please install before compiling lylog.])
fi

AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT
