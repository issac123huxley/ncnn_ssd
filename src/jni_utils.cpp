#include "jni_utils.h"
#include <sstream>
#include <unistd.h>

namespace jniutils {


std::string convertJStrToString(JNIEnv* env, jstring lString) {
  const char* lStringTmp;
  std::string str;

  lStringTmp = env->GetStringUTFChars(lString, NULL);
  if (lStringTmp == NULL)
    return NULL;

  str = lStringTmp;

  env->ReleaseStringUTFChars(lString, lStringTmp);

  return str;
}

}
