#ifndef CONVERT_ARRAY_TO_MAT_H
#define CONVERT_ARRAY_TO_MAT_H

#include <android/bitmap.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <jni.h>


#define IMAGEFORMAT_NV21 1
#define IMAGEFORMAT_YUYV 2
#define IMAGEFORMAT_RGB 3
#define IMAGEFORMAT_RGBA 4


namespace jniutils {

void convertArrayToMat(JNIEnv* env, jbyteArray yuv, jint format, jint rotation, jint width, jint height, cv::Mat & rgbMat);

}  // end jniutils


#endif /* CONVERT_ARRAY_TO_MAT_H */
