AC_PREREQ(2.67)
AC_INIT(turing, 1.0)
AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign silent-rules subdir-objects])

AC_CONFIG_MACRO_DIR([m4])

AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_TRY_DOXYGEN
BOOST_REQUIRE([1.36.0])
BOOST_FOREACH

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([DEPS], [largenet2 >= 0.3])
 
# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
AC_C_INLINE

# Sanity check
AC_CONFIG_SRCDIR([src/turing.cpp])

# Output files
AC_CONFIG_FILES([Makefile])

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT

