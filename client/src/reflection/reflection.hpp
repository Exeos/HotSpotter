#pragma once
#include "jni.h"

namespace hot_spotter::reflection {
    jstring getMethodName(jclass clazz, jmethodID methodId);
}