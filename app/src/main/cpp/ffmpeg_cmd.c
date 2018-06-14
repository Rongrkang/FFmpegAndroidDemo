#include <jni.h>
#include "ffmpeg.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "android_log.h"
#include "libavutil/imgutils.h"
#include "ffmpeg_cmd.h"


JNIEXPORT jint

JNICALL
Java_com_leikang_ffmpeg_MainActivity_exec(
        JNIEnv *env, jclass obj, jobjectArray commands) {

    int argc = (*env)->GetArrayLength(env, commands);
    char *argv[argc];

    int i;
    for (i = 0; i < argc; i++) {
        jstring js = (jstring) (*env)->GetObjectArrayElement(env, commands, i);
        argv[i] = (char *) (*env)->GetStringUTFChars(env, js, JNI_FALSE);
    }

    return main(argc, argv);
}


JNIEXPORT jint

JNICALL
Java_com_leikang_ffmpeg_MainActivity_playVideo(
        JNIEnv *env, jclass obj, jstring url, jobject surface) {
    XLOGD("start playvideo... url");

    const char *file_name = (*env)->GetStringUTFChars(env, url, JNI_FALSE);
    av_register_all();
    AVFormatContext *pContext = avformat_alloc_context();
    if (avformat_open_input(&pContext, file_name, NULL, NULL) != 0) {
        XLOGE("Couldn't open file:%s\n", file_name);
        return -1;// Couldn't open file
    }

    if (avformat_find_stream_info(pContext, NULL) < 0) {
        XLOGE("Couldn't find stream information.");
        return -1;
    }

    int videoStream = -1;
    // Find the first video stream
    for (int i = 0; i < pContext->nb_streams; i++) {
        if (pContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }

    if (videoStream == -1) {
        XLOGE("Didn't find a video stream.");
        return -1; // Didn't find a video stream
    }

    // Get a pointer to the codec context for the video stream
    AVCodecContext *pCodecContext = pContext->streams[videoStream]->codec;
    // Find the decoder for the video stream
    AVCodec *pCodec = avcodec_find_decoder(pCodecContext->codec_id);
    if (pCodec == NULL) {
        XLOGE("Codec not found.");
        return -1;// Codec not found
    }

    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0) {
        XLOGE("Could not open codec.");
        return -1; // Could not open codec
    }

    // 获取native window
    ANativeWindow *pWindow = ANativeWindow_fromSurface(env, surface);

    // 获取视频宽高
    int width = pCodecContext->width;
    int height = pCodecContext->height;

    if (checkVideoSize(width, height) == 0) {
        XLOGE("Could not get video width,height.");
        return -1;
    }

    // 设置native window的buffer大小,可自动拉伸
    ANativeWindow_setBuffersGeometry(pWindow, width, height, WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer window_buffer;
    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0) {
        XLOGE("Could not open codec.");
        return -1; // Could not open codec
    }

    // Allocate video frame
    AVFrame *pFrame = av_frame_alloc();
    // 用于渲染
    AVFrame *pFrameRGBA = av_frame_alloc();
    if (pFrame == NULL || pFrameRGBA == NULL) {
        XLOGE("Could not allocate video frame.");
        return -1;
    }
    // Determine required buffer size and allocate buffer
    // buffer中数据就是用于渲染的,且格式为RGBA
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecContext->width,
                                            pCodecContext->height, 1);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                         pCodecContext->width, pCodecContext->height, 1);

    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    struct SwsContext *sws_ctx = sws_getContext(pCodecContext->width, pCodecContext->height,
                                                pCodecContext->pix_fmt,
                                                pCodecContext->width, pCodecContext->height,
                                                AV_PIX_FMT_RGBA,
                                                SWS_BILINEAR, NULL,
                                                NULL, NULL);


    int frameFinished;
    AVPacket packet;
    while (av_read_frame(pContext, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream) {
            // Decode video frame
            avcodec_decode_video2(pCodecContext, pFrame, &frameFinished, &packet);
            // 并不是decode一次就可解码出一帧
            if (frameFinished) {
                // lock native window buffer
                ANativeWindow_lock(pWindow, &window_buffer, 0);
                // 格式转换
                sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
                          pFrame->linesize, 0, pCodecContext->height,
                          pFrameRGBA->data, pFrameRGBA->linesize);
                // 获取stride
                uint8_t *dst = window_buffer.bits;
                int dstStride = window_buffer.stride * 4;
                uint8_t *src = (uint8_t *) (pFrameRGBA->data[0]);
                int srcStride = pFrameRGBA->linesize[0];
                // 由于window的stride和帧的stride不同,因此需要逐行复制
                int h;
                for (h = 0; h < height; h++) {
                    memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
                }
                ANativeWindow_unlockAndPost(pWindow);
            }
        }
        av_packet_unref(&packet);
    }
    av_free(buffer);
    av_free(pFrameRGBA);
    // Free the YUV frame
    av_free(pFrame);
    // Close the codecs
    avcodec_close(pCodecContext);
    // Close the video file
    avformat_close_input(&pContext);

    return 0;
}


int checkVideoSize(int width, int height) {
    if (width == 0 || height == 0) {
        return 0;
    }
    return 1;
}