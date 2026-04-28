#pragma once
#include "jni.h"
#include "jvmti.h"

namespace jvm::attacher {
    bool attach(JavaVM*& vm, JNIEnv*& env, jvmtiEnv*& jvmtiEnv);
}
