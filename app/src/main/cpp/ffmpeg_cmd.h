#ifndef FFMPEG_FFMPEG_CMD_H
#define FFMPEG_FFMPEG_CMD_H

#include <jni.h>

int Java_com_leikang_ffmpeg_MainActivity_exec(JNIEnv *env, jclass obj, jobjectArray commands);

int Java_com_leikang_ffmpeg_MainActivity_playVideo(JNIEnv *env, jclass obj, jstring url, jobject surface);

int checkVideoSize(int width, int height);


#endif //FFMPEG_FFMPEG_CMD_H
