#!/bin/bash

NDK=/home/leikang/Android/android-ndk-r12b
SYSROOT=$NDK/platforms/android-14/arch-arm
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64

build_one()
{
./configure \
--enable-static \
--disable-shared \
--enable-pic \
--disable-asm \
--disable-cli \
--host=arm-linux-androideabi \
--prefix=$PREFIX \
--cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
--sysroot=$SYSROOT \

make clean
make
make install
}

PREFIX=$(pwd)/android
build_one

