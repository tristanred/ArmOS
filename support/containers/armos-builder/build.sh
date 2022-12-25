#!/bin/sh

cd /buildsrc

mkdir build-binutils
cd build-binutils

# Build and installs binutils
../binutils-2.39/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd /buildsrc

mkdir build-gcc
cd build-gcc

# Build and install gcc
../gcc-12.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
