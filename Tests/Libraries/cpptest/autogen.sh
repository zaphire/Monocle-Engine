#!/bin/sh
#
# $Id: autogen.sh,v 1.6 2008/07/15 20:33:31 hartwork Exp $
#
# CppTest - A C++ Unit Testing Framework
# Copyright (c) 2003 Niklas Lundell
#
# ---
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# ---

# --- Initialization of libtool

echo "Creating library tools..."
LIBTOOLIZE=libtoolize
if glibtoolize --version &>/dev/null ; then
	LIBTOOLIZE=glibtoolize
fi
${LIBTOOLIZE} --automake --copy --force

# --- Creation of 'aclocal.m4'

# The autotools are not backwards compatible with previous releases. 
# Therefore, it often outputs non-important warnings that will be 
# ignore and suppressed.
#
echo "Creating macros..."
aclocal $aclocal_flags 2>&1 \
	| grep -v 'warning: underquoted definition of' \
	| grep -v 'Extending.aclocal' \
	| grep -v 'Extending%20aclocal' 

# --- Initialization of automake

echo "Creating Makefile templates..."
touch README
automake --gnu --add-missing --copy

# --- Initialization of autoconf

echo "Creating 'configure'..."
autoconf

# --- Finished...

echo -e "\nRun: ./configure [OPTIONS]; make; make install\n"

