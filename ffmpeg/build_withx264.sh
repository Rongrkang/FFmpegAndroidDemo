#!/bin/bash

# 设置NDK路径
NDK=/home/leikang/Android/android-ndk-r12b

# 设置编译针对的平台，可以根据实际需求进行设置
# 当前设置为最低支持android-14版本，arm架构
SYSROOT=$NDK/platforms/android-14/arch-arm

# 设置编译工具链，4.9为版本号
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64

# 不指定相对路径可不可以，我在实际编译的时候用的是绝对路径
# LIB_DIR=/home/leikang/Android/ffmpeg/x264/android
LIB_DIR=../x264/android

function build_one
{
./configure \
--enable-cross-compile \
--enable-asm \
--enable-neon \
--enable-shared \
--disable-static \
--enable-small \
--enable-libx264 \
--enable-gpl \
--enable-yasm \
--disable-doc \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-avdevice \
--disable-symver \
--disable-parsers \
--enable-parser=aac \
--enable-parser=ac3 \
--enable-parser=h264 \
--disable-demuxers \
--enable-demuxer=image2 \
--enable-demuxer=h264 \
--enable-demuxer=aac \
--enable-demuxer=avi \
--enable-demuxer=mpc \
--enable-demuxer=mov \
--enable-encoders \
--disable-decoders \
--enable-decoder=aac \
--enable-decoder=aac_latm \
--enable-decoder=h264 \
--enable-decoder=mpeg4 \
--enable-decoder=mjpeg \
--enable-decoder=png \
--prefix=$PREFIX \
--cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
--target-os=android \
--arch=arm \
--cpu=armv7-a \
--sysroot=$SYSROOT \
--extra-cflags="-I$LIB_DIR/include -fPIC -DANDROID -mfpu=neon -mfloat-abi=softfp -I$SYSROOT/usr/include" \
--extra-ldflags="-L$LIB_DIR/lib -lx264 $ADDI_LDFLAGS" \


make clean
make
make install
}

# 设置编译后文件的输出目录
CPU=arm
PREFIX=$(pwd)/android/$CPU
ADDI_CFLAGS="-marm" 
build_one






