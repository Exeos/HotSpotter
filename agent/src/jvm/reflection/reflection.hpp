#pragma once
#include "jni.h"

namespace jvm::reflection {
    jstring getMethodName(jclass clazz, jmethodID methodId);
}