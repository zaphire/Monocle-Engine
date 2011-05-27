## ---
##
## $Id: Makefile.am,v 1.6 2008/07/13 18:28:10 hartwork Exp $
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

WARN_FLAGS		=
if WARNINGS
WARN_FLAGS		+= -W -Wall
endif

AM_CXXFLAGS		= $(WARN_FLAGS) 

lib_LTLIBRARIES		= libcpptest.la

libcpptest_la_LDFLAGS	= -version-info $(LT_VERSION)

libcpptest_la_SOURCES	= \
	collectoroutput.cpp \
	compileroutput.cpp \
	htmloutput.cpp \
	missing.cpp \
	source.cpp \
	suite.cpp \
	textoutput.cpp \
	time.cpp \
	utils.cpp
	
libcpptest_la_SOURCES += \
	missing.h \
	utils.h

include_HEADERS = \
	cpptest-assert.h \
	cpptest-collectoroutput.h \
	cpptest-compileroutput.h \
	cpptest-htmloutput.h \
	cpptest-output.h \
	cpptest-source.h \
	cpptest-suite.h \
	cpptest-textoutput.h \
	cpptest-time.h \
	cpptest.h

MOSTLYCLEANFILES =

CLEANFILES = 

DISTCLEANFILES = \
	Makefile
	
MAINTAINERCLEANFILES = \
	Makefile.in

