#pragma once
#include "jni.h"
#include "jvmti.h"
#include <map>
#include <string>

typedef std::map<std::string, std::pair<jclass, std::pair<jint, const unsigned char*>>> class_map_t;

namespace hot_spotter {
    extern JavaVM* jvm;
    extern JNIEnv* jniEnv;
    extern jvmtiEnv* jvmTi;
    extern class_map_t classes;
}