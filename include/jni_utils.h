#ifndef JNI_UTILS_H
#define JNI_UTILS_H
#include <jni.h>
#include <string>

namespace jniutils {

std::string convertJStrToString(JNIEnv* env, jstring lString);

}  // end jniutils

#endif /* JNI_UTILS_H */
