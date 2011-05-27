## ---
##
## $Id: configure.ac,v 1.13 2010/03/26 04:52:08 hartwork Exp $
##
## CppTest - A C++ Unit Testing Framework
## Copyright (c) 2003 Niklas Lundell
##
## ---
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the
## Free Software Foundation, Inc., 59 Temple Place - Suite 330,
## Boston, MA 02111-1307, USA.
##
## ---

##### Boilerplate

AC_PREREQ(2.53)

AC_INIT(CppTest, 1.1.1, nilu@users.sourceforge.net, cpptest)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(autogen.sh)

AM_INIT_AUTOMAKE([1.8 gnu dist-zip])
AM_CONFIG_HEADER(config/config.h)

# LibTool versioning
#
# +1 : ? : +1 => new interface that does not break old one
# +1 : ? :  0 => new interface that breaks old one
#  ? : ? :  0 => no new interfaces, but breaks apps
#  ? :+1 :  ? => internal changes only, nothing breaks but might work better
#
# current:revision:age
#
LT_VERSION=0:7:0
AC_SUBST(LT_VERSION, $LT_VERSION)

##### Alternatives

# Generate documentation 
# Note: AutoMake conditional will be set when 'doxygen' is checked for.
#
AC_ARG_ENABLE(
	doc,
	AC_HELP_STRING(
		[--enable-doc],
		[Generate documentation (default=no)]),
	[ case "$enableval" in
		yes)	enable_doc=yes ;;
		no)	enable_doc=no  ;;
		*)	AC_MSG_ERROR([bad value '$enableval' for --enable-doc]) ;;
	  esac
	],
	[enable_doc=no]
)
AM_CONDITIONAL(DOC, test "$enable_doc" = "yes")
AC_MSG_CHECKING(whether to generate documentation)
AC_MSG_RESULT($enable_doc)

# Enable C++ warnings
#
AC_ARG_ENABLE(
	warnings,
	AC_HELP_STRING(
		[--enable-warnings],
		[Turn on additional compiler warnings (g++ only) (default=no)]),
	[ case "$enableval" in
		yes)	enable_warnings=yes ;;
		no)	enable_warnings=no  ;;
		*)	AC_MSG_ERROR([bad value '$enableval' for --enable-warnings]) ;;
	  esac
	],
	[enable_warnings=no]
)
AM_CONDITIONAL(WARNINGS, test "$enable_warnings" = "yes")
AC_MSG_CHECKING(whether to turn on C++ compilation warnings)
AC_MSG_RESULT($enable_warnings)

##### Programs

AC_PROG_CC
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL

if test "$enable_doc" = "yes"; then
	AC_CHECK_PROG(x_doxygen, doxygen, yes, no)
	if test "$x_doxygen" = "no"; then
		AC_MSG_WARN(['doxygen' not found. You will not be able to generate code documentation])
		enable_doc=no
	fi
fi

##### Libraries

##### Headers

AC_CHECK_HEADERS(sys/time.h)

##### Datatypes

##### Functions

AC_CHECK_FUNCS([gettimeofday])

AC_MSG_CHECKING(for round)
AC_TRY_COMPILE(
	[ #include <math.h> ],
	[ double d = round(1.0); ],
	[ round=yes ],[ round=no ])
AC_MSG_RESULT($round)
if test "$round" = "yes"; then
	AC_DEFINE([HAVE_ROUND],,[Defined if the function exists])
fi

##### Output

AC_SUBST(prefix)

AC_CONFIG_FILES([config/stamp-h], [echo timestamp > config/stamp-h])
AC_CONFIG_FILES([doc/Doxyfile INSTALL.quick libcpptest.pc README])
AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile test/Makefile])
AC_OUTPUT

