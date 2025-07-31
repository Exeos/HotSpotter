#include "hooks.hpp"

#include <cstring>

#include "../globals.hpp"
#include "../logger/logger.hpp"

namespace hot_spotter::hooks {
    jvmtiEventCallbacks event_callbacks = {};

    void JNICALL ClassFileLoadHook(
        jvmtiEnv* jvmti,
        JNIEnv* jni,
        jclass class_being_redefined,
        jobject loader,
        const char* name,
        jobject protection_domain,
        jint class_data_len,
        const unsigned char* class_data,
        jint* new_class_data_len,
        unsigned char** new_class_data
    ) {
        // allocate memory and copy class data into it. NEEDS MEMORY CLEANUP
        unsigned char* copyDest = new unsigned char[class_data_len];
        memcpy(copyDest, class_data, class_data_len);

        classes[std::string(name)] = std::make_pair(static_cast<jclass>(jni->NewGlobalRef(class_being_redefined)), std::make_pair(class_data_len, copyDest));
        Logger::LogFormat("Intercepted class load: %s", name);
    }

    bool initHooks() {
        event_callbacks.ClassFileLoadHook = ClassFileLoadHook;
        jvmtiError setCallbacksError = jvmTi->SetEventCallbacks(&event_callbacks, sizeof(event_callbacks));
        jvmtiError setNotificationError = jvmTi->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);

        return setCallbacksError == JVMTI_ERROR_NONE && setNotificationError == JVMTI_ERROR_NONE;
    }

    bool removeHooks() {
        event_callbacks = {};
        jvmtiError setCallbacksError = jvmTi->SetEventCallbacks(&event_callbacks, sizeof(event_callbacks));
        jvmtiError setNotificationError = jvmTi->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);

        return setCallbacksError == JVMTI_ERROR_NONE && setNotificationError == JVMTI_ERROR_NONE;
    }
}
