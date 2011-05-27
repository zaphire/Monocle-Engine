## ---
##
## $Id: Makefile.am,v 1.5 2008/07/13 18:28:11 hartwork Exp $
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

noinst_PROGRAMS		= mytest

mytest_LDFLAGS		= -static 
mytest_LDADD		= ../src/libcpptest.la 
mytest_SOURCES		= mytest.cpp

MOSTLYCLEANFILES =

CLEANFILES = 

DISTCLEANFILES = \
	Makefile
	
MAINTAINERCLEANFILES = \
	Makefile.in

