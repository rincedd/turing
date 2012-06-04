AC_PREREQ(2.61)
AC_INIT(turing, 1.0)
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign silent-rules subdir-objects])
LT_PREREQ([2.2])
LT_INIT([disable-static])

AC_CONFIG_MACRO_DIR([m4])

AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_TRY_DOXYGEN
BOOST_REQUIRE([1.43.0])
BOOST_FOREACH
BOOST_PROGRAM_OPTIONS

# configure option --enable-debug [=no]
AC_ARG_ENABLE([debug],
			   [AS_HELP_STRING([--enable-debug], [Enable debug mode (default=no)])],
			   [debug="$withval"],
			   [debug=no])

if test "x$debug" = xno; then
  AM_CONDITIONAL([DEBUGMODE], [test "x$debug" = "xyes"])
fi

# configure option --with-builtin-odeint [=yes]
AC_ARG_WITH([builtin-odeint], 
			[AS_HELP_STRING([--with-builtin-odeint], [Use builtin odeint library (default=yes)])], 
			[myodeint="$withval"], 
			[myodeint=yes])

if test "x$myodeint" = xno; then
  BOOST_FIND_HEADER([boost/numeric/odeint.hpp], [odeint_fallback=yes; myodeint=yes])
fi

AM_CONDITIONAL([USE_BUILTIN_ODEINT], [test "x$myodeint" = "xyes"])

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([DEPS], [largenet2 >= 0.3])
 
# Sanity check
AC_CONFIG_SRCDIR([src/turing.cpp])

# Output files
AC_CONFIG_FILES([Makefile])

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT

if test "x$odeint_fallback" = "xyes"; then
  echo "================================"
  echo "Could not find odeint library, "
  echo "falling back on builtin version."
  echo "================================"
fi
