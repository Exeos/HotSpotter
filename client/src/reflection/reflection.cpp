#include "jni.h"
#include "../globals.hpp"

namespace hot_spotter::reflection {

    jstring getMethodName(jclass clazz, jmethodID methodId) {
        jobject reflectedMethod = jniEnv->ToReflectedMethod(clazz, methodId, JNI_FALSE);

        jclass methodClass = jniEnv->GetObjectClass(reflectedMethod);
        jmethodID getName = jniEnv->GetMethodID(methodClass, "getName", "()Ljava/lang/String;");

        return static_cast<jstring>(jniEnv->CallObjectMethod(reflectedMethod, getName));
    }
}
