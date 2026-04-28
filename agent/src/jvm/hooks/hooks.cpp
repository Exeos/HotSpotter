#include "hooks.hpp"

#include <cstring>
#include <string>
#include <mutex>

#include "jvmti.h"
#include "../../agent.hpp"

namespace jvm::hooks {
    void JNICALL ClassFileLoadHook(jvmtiEnv *jvmti, JNIEnv *jni,
                                   jclass class_being_redefined, jobject loader,
                                   const char *name, jobject protection_domain,
                                   jint class_data_len,
                                   const unsigned char *class_data,
                                   jint *new_class_data_len,
                                   unsigned char **new_class_data) {
    }

    bool initHooks() {
        jvmtiEventCallbacks event_callbacks = {};

        event_callbacks.ClassFileLoadHook = ClassFileLoadHook;
        jvmtiError setCallbacksError =
                agent::jvmTi->SetEventCallbacks(&event_callbacks, sizeof(event_callbacks));
        jvmtiError setNotificationError = agent::jvmTi->SetEventNotificationMode(
            JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, nullptr);

        return setCallbacksError == JVMTI_ERROR_NONE &&
               setNotificationError == JVMTI_ERROR_NONE;
    }

    bool removeHooks() {
        jvmtiEventCallbacks event_callbacks = {};
        jvmtiError setCallbacksError =
                agent::jvmTi->SetEventCallbacks(&event_callbacks, sizeof(event_callbacks));
        jvmtiError setNotificationError = agent::jvmTi->SetEventNotificationMode(
            JVMTI_DISABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, nullptr);

        return setCallbacksError == JVMTI_ERROR_NONE &&
               setNotificationError == JVMTI_ERROR_NONE;
    }
} // namespace hot_spotter::hooks
