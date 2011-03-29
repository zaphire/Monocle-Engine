#!/bin/sh

echo "Do you want to build tests? (y/n) : "
read ANSWER

case "$ANSWER" in
	y*|Y*) CMAKEOPTS="${CMAKEOPTS} -DBUILD_TESTS:BOOL=ON" ;;
	*)     CMAKEOPTS="${CMAKEOPTS} -DBUILD_TESTS:BOOL=OFF" ;;
esac

cmake ${CMAKEOPTS} .

make

