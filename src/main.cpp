// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2017 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <time.h>
#include <vector>
#include <jni.h>
#include "jni_convertArrayToMat.h"
#include "jni_utils.h"
#include "net.h"

// define JNI Interface

struct Object{
    cv::Rect rec;
    int class_id;
    float prob;
};

const char* class_names[] = {"background",
                            "aeroplane", "bicycle", "bird", "boat",
                            "bottle", "bus", "car", "cat", "chair",
                            "cow", "diningtable", "dog", "horse",
                            "motorbike", "person", "pottedplant",
                            "sheep", "sofa", "train", "tvmonitor"};
ncnn::Net mobilenet;
static int detect_mobilenet(cv::Mat& raw_img, float show_threshold, std::vector<Object> & objects)
{
    
    /*
     * model is  converted from https://github.com/chuanqi305/MobileNet-SSD
     * and can be downloaded from https://drive.google.com/open?id=0ByaKLD9QaPtucWk0Y0dha1VVY0U
     */
    int img_h = raw_img.size().height;
    int img_w = raw_img.size().width;

    int input_size = 300;
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(raw_img.data, ncnn::Mat::PIXEL_BGR, raw_img.cols, raw_img.rows, input_size, input_size);

    const float mean_vals[3] = {127.5f, 127.5f, 127.5f};
    const float norm_vals[3] = {1.0/127.5,1.0/127.5,1.0/127.5};
    in.substract_mean_normalize(mean_vals, norm_vals);

    ncnn::Mat out;

    ncnn::Extractor ex = mobilenet.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(4);

    printf("%d %d %d\n", out.w, out.h, out.c);

    for (int iw=0;iw<out.h;iw++)
    {
        Object object;
        const float *values = out.row(iw);
        object.class_id = values[0];
        object.prob = values[1];
        object.rec.x = values[2] * img_w;
        object.rec.y = values[3] * img_h;
        object.rec.width = values[4] * img_w - object.rec.x;
        object.rec.height = values[5] * img_h - object.rec.y;
        objects.push_back(object);
    }

    return 0;
}

// extern "C"
// JNIEXPORT void JNICALL Java_com_thinkjoy_androidssd_NativeInterface_init(JNIEnv *env, jobject obj) {
//     mobilenet.load_param("/sdcard/mobilenet_ssd_voc_ncnn.param");
//     mobilenet.load_model("/sdcard/mobilenet_ssd_voc_ncnn.bin");
// }
extern "C"
JNIEXPORT void JNICALL Java_com_thinkjoy_androidssd_NativeInterface_init(JNIEnv *env, jobject obj, jstring bin_path, jstring param_path) {
    // mobilenet.load_param("/sdcard/ncnn.proto");
    // mobilenet.load_model("/sdcard/ncnn.bin");
    // mobilenet.load_param("/sdcard/mobilenet_ssd_voc_ncnn.param");
    // mobilenet.load_model("/sdcard/mobilenet_ssd_voc_ncnn.bin");
    std::string path1 = jniutils::convertJStrToString(env, param_path);
    std::string path2 = jniutils::convertJStrToString(env, bin_path);
    mobilenet.load_param(path1.c_str());
    mobilenet.load_model(path2.c_str());
}
extern "C"
JNIEXPORT void JNICALL Java_com_thinkjoy_androidssd_NativeInterface_detect(JNIEnv *env, jobject obj, jbyteArray imageArray, int rotation, int imageFormat, int imageWidth, int imageHeight, jobject object_list) {
    std::vector<Object> objects;
    jbyte *pBuf = NULL;
    pBuf = (jbyte *)env->GetByteArrayElements(imageArray, 0);
    cv::Mat raw_img = cv::Mat(imageHeight + imageHeight / 2, imageWidth, CV_8UC1, (unsigned char *)pBuf);
    cv::cvtColor(raw_img, raw_img, CV_YUV2BGR_NV21);
    env->ReleaseByteArrayElements(imageArray, pBuf, 0);
    int img_h = raw_img.size().height;
    int img_w = raw_img.size().width;
    int input_size = 300;
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(raw_img.data, ncnn::Mat::PIXEL_BGR, raw_img.cols, raw_img.rows, 300, 300);

    const float mean_vals[3] = {127.5f, 127.5f, 127.5f};
    const float norm_vals[3] = {1.0/127.5,1.0/127.5,1.0/127.5};
    in.substract_mean_normalize(mean_vals, norm_vals);

    ncnn::Mat out;

    ncnn::Extractor ex = mobilenet.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(6);
    ex.input("data", in);
    ex.extract("detection_out",out);


    jclass class_ArrayList = env->FindClass("java/util/ArrayList");
    jmethodID method_add_ArrayList = env->GetMethodID(class_ArrayList, "add", "(Ljava/lang/Object;)Z");
    jmethodID method_get_ArrayList = env->GetMethodID(class_ArrayList, "get", "(I)Ljava/lang/Object;");
    jmethodID method_size_ArrayList = env->GetMethodID(class_ArrayList, "size", "()I");

    jclass class_Object = env->FindClass("com/thinkjoy/androidssd/Object");
    jmethodID method_construct_Object = env->GetMethodID(class_Object, "<init>", "(IIIIIF)V");

    int left, top, bottom, right, class_id;
    float prob;
    for (int iw=0;iw<out.h;iw++)
    {
        const float *values = out.row(iw);
        class_id = values[0];
        prob = values[1];
        left = std::min(std::max((int)(values[2] * img_w), 0), img_w);
        top = std::min(std::max((int)(values[3] * img_h), 0), img_h);
        right = std::min(std::max((int)(values[4] * img_w), 0), img_w);
        bottom = std::min(std::max((int)(values[5] * img_h), 0), img_h);
        jobject object = env->NewObject(class_Object, method_construct_Object, left, top, right, bottom, class_id, prob);
        env->CallBooleanMethod(object_list, method_add_ArrayList, object);        
    }    
}
