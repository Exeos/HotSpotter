#pragma once
#include "jni.h"
#include "jvmti.h"

namespace hot_spotter {
    extern JavaVM* jvm;
    extern JNIEnv* jniEnv;
    extern jvmtiEnv* jvmTi;

    void init();
    bool showWindow();
    void tidy();
}
