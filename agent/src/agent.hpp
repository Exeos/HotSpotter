#pragma once
#include "jni.h"
#include "jvmti.h"

namespace agent {

    extern JavaVM* vm;
    extern JNIEnv* jniEnv;
    extern jvmtiEnv* jvmTi;

    void init();
    void destroy();
}
