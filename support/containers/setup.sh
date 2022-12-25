#!/bin/sh

yes | unminimize && apt update

apt install curl git vim -y

mkdir /buildsrc
cd /buildsrc

curl https://ftp.gnu.org/gnu/binutils/binutils-2.39.tar.gz > binutils-2.39.tar.xz
curl https://ftp.gnu.org/gnu/gcc/gcc-12.2.0/gcc-12.2.0.tar.gz > gcc-12.2.0.tar.gz

tar -xzf binutils-2.39.tar.xz
tar -xzf gcc-12.2.0.tar.gz

# Install build dependencies
apt install build-essential	bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo -y