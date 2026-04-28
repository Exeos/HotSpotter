#include "jni.h"
#include "../../agent.hpp"

namespace jvm::reflection {

    jstring getMethodName(jclass clazz, jmethodID methodId) {
        jobject reflectedMethod = agent::jniEnv->ToReflectedMethod(clazz, methodId, JNI_FALSE);

        jclass methodClass = agent::jniEnv->GetObjectClass(reflectedMethod);
        jmethodID getName = agent::jniEnv->GetMethodID(methodClass, "getName", "()Ljava/lang/String;");

        return static_cast<jstring>(agent::jniEnv->CallObjectMethod(reflectedMethod, getName));
    }
}
