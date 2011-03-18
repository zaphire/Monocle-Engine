#!/bin/sh

mkdir -p Build
pushd Build

echo "Do you want to build tests? (y/n) : "
read ANSWER

if [[ "${ANSWER}" == y* || "${ANSWER}" == Y* ]]; then
	CMAKEOPTS="${CMAKEOPTS} -DBUILD_TESTS:BOOL=ON"
else
	CMAKEOPTS="${CMAKEOPTS} -DBUILD_TESTS:BOOL=OFF"
fi

cmake ${CMAKEOPTS} ..

popd
make -C Build/ all

