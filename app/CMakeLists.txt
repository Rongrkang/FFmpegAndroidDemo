# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

# 设置Cmake版本
cmake_minimum_required(VERSION 3.6)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# 设置cpp目录路径
set(CPP_DIR ${CMAKE_SOURCE_DIR}/src/main/cpp)

# 设置jniLibs目录路径
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs)

# 添加库
add_library( # Sets the name of the library.
             ffmpeg

             # Sets the library as a shared library.
             # 动态库，生成so文件
             SHARED

             # Provides a relative path to your source file(s).
             # 源码
             ${CPP_DIR}/cmdutils.c
             ${CPP_DIR}/ffmpeg.c
             ${CPP_DIR}/ffmpeg_filter.c
             ${CPP_DIR}/ffmpeg_opt.c
             ${CPP_DIR}/ffmpeg_hw.c
             ${CPP_DIR}/ffmpeg_cmd.c )

# 用于各种类型声音、图像编解码
add_library( # 库名称
             avcodec

             # 动态库，生成so文件
             SHARED

             # 表示该库是引用的不是生成的
             IMPORTED )

# 引用库文件
set_target_properties( # 库名称
                       avcodec

                       # 库的路径
                       PROPERTIES IMPORTED_LOCATION
                       ${LIBS_DIR}/armeabi-v7a/libavcodec.so )

# 提供了各种音视频过滤器
add_library( avfilter
             SHARED
             IMPORTED )

set_target_properties( avfilter
                       PROPERTIES IMPORTED_LOCATION
                       ${LIBS_DIR}/armeabi-v7a/libavfilter.so )

# 用于各种音视频封装格式的生成和解析，读取音视频帧等功能
add_library( avformat
             SHARED
             IMPORTED )

set_target_properties( avformat
                       PROPERTIES IMPORTED_LOCATION
                       ${LIBS_DIR}/armeabi-v7a/libavformat.so )

# 包含一些公共的工具函数
add_library( avutil
             SHARED
             IMPORTED )

set_target_properties( avutil
                       PROPERTIES IMPORTED_LOCATION
                       ${LIBS_DIR}/armeabi-v7a/libavutil.so )

# 用于后期效果处理
add_library( libpostproc
             SHARED
             IMPORTED )


set_target_properties( libpostproc
                       PROPERTIES IMPORTED_LOCATION
                       ${LIBS_DIR}/armeabi-v7a/libpostproc.so )


# 用于音频重采样，采样格式转换和混合
add_library( swresample
             SHARED
             IMPORTED )

set_target_properties( swresample
                       PROPERTIES IMPORTED_LOCATION
                       ${LIBS_DIR}/armeabi-v7a/libswresample.so )

# 用于视频场景比例缩放、色彩映射转换
add_library( swscale
             SHARED
             IMPORTED )

set_target_properties( swscale
                       PROPERTIES IMPORTED_LOCATION
                       ${LIBS_DIR}/armeabi-v7a/libswscale.so )


# 据说是最好的视频有损编码器
# add_library( x264
#              SHARED
#              IMPORTED )

# set_target_properties( x264
#                        PROPERTIES IMPORTED_LOCATION
#                        ${LIBS_DIR}/armeabi-v7a/libx264.so )


# 引用源码
include_directories( ../ffmpeg/
                     ${CPP_DIR}/include/ )


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# 关联库
target_link_libraries( ffmpeg
                       avcodec
                       avformat
                       avutil
                       avfilter
                       libpostproc
                       swresample
                       swscale
                       android
                       log )
