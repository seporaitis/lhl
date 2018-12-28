AC_INIT([lhl], [0.0.1], [seporaitis@users.noreply.github.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])

MP_WITH_CURSES
HAVE_CURS=$mp_cv_curses
if test x$HAVE_CURS != xyes; then
  AC_MSG_ERROR([Linux Ham Log requires ncurses library to compile.])
fi

AX_LIB_SQLITE3(3.20)
if test "x$success" != "xyes"; then
  AC_MSG_ERROR([Linux Ham Log requires SQLite3 to compile.])
fi

AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT
