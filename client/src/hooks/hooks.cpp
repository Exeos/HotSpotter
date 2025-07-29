#include "hooks.hpp"

#include "../hot_spotter.hpp"
#include "../logger/logger.hpp"

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
    Logger::LogFormat("Intercepted class load: %s", name);
}


namespace hot_spotter::hooks {
    jvmtiEventCallbacks event_callbacks = {};

    bool initHooks() {
        event_callbacks.ClassFileLoadHook = ClassFileLoadHook;
        jvmtiError setCallbacksError = jvmTi->SetEventCallbacks(&event_callbacks, sizeof(event_callbacks));
        jvmtiError setNotificationError = jvmTi->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);

        return setCallbacksError == JVMTI_ERROR_NONE && setNotificationError == JVMTI_ERROR_NONE;
    }
}
