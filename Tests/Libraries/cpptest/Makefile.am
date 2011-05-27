## ---
##
## $Id: Makefile.am,v 1.10 2008/07/15 20:33:31 hartwork Exp $
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

SUBDIRS = src test
if DOC
SUBDIRS += doc
endif

####### Distribution

EXTRA_DIST = \
	autogen.sh \
	BUGS \
	config/stamp-h.in \
	INSTALL.quick \
	libcpptest.pc \
	README \
	win/winconfig.h \
	win/VisualStudio.NET/VisualStudio.NET.sln \
	win/VisualStudio.NET/cpptest/cpptest.vcproj \
	win/VisualStudio.NET/test/test.vcproj

####### Clean

MOSTLYCLEANFILES =

CLEANFILES =

DISTCLEANFILES = \
	config/config.h \
	config/stamp-h? \
	INSTALL.quick \
	libcpptest.pc \
	README \
	Makefile
	
MAINTAINERCLEANFILES = \
	aclocal.m4 \
	autom4te-*.cache/* \
	config.h \
	configure \
	config/config.guess \
	config/config.sub \
	config/depcomp \
	config/install-sh \
	config/ltmain.sh \
	config/missing \
	config/mkinstalldirs \
	Makefile.in

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = libcpptest.pc

