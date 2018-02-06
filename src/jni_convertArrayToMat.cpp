
#include "jni_convertArrayToMat.h"
#include <jni.h>

namespace jniutils
{

using namespace cv;

void convertArrayToMat(JNIEnv *env, jbyteArray yuv, jint format, jint rotation, jint width, jint height, cv::Mat &image)
{

    jbyte *pBuf = NULL;
    switch (format)
    {
    case IMAGEFORMAT_NV21:
        pBuf = (jbyte *)env->GetByteArrayElements(yuv, 0);
        image = Mat(height + height / 2, width, CV_8UC1, (unsigned char *)pBuf);
        cvtColor(image, image, CV_YUV2BGR_NV21);
        env->ReleaseByteArrayElements(yuv, pBuf, 0);
        break;
    case IMAGEFORMAT_YUYV:
        break;
    case IMAGEFORMAT_RGB:
        break;
    case IMAGEFORMAT_RGBA:
        break;
    }

    switch (rotation)
    {
    case 0:
        break;
    case 90:
        cv::transpose(image, image);
        cv::flip(image, image, 1);
        break;
    case 180:
        cv::flip(image, image, -1);
        break;
    case 270:
        cv::transpose(image, image);
        cv::flip(image, image, 0);
        break;
    }
}
}
