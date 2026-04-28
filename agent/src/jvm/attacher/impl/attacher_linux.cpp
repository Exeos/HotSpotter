#include <dlfcn.h>
#include "../attacher.hpp"
#include "../../../logger/logger.hpp"

namespace jvm::attacher {

    bool attach(JavaVM*& vm, JNIEnv*& env, jvmtiEnv*& jvmtiEnv) {
        void* jvm = dlopen("libjvm.so", RTLD_NOW | RTLD_GLOBAL);
        if (!jvm) {
            logger::Log("Failed to obtain handle to libjvm.so");
            return false;
        }

        // Define function pointer type for JNI_GetCreatedJavaVMs
        using t_JNI_GetCreatedJavaVMs = jint(*)(JavaVM**, jsize, jsize*);
        void* s_JNI_GetCreatedJavaVMs = dlsym(jvm, "JNI_GetCreatedJavaVMs");
        if (!s_JNI_GetCreatedJavaVMs) {
            logger::Log("Failed to find symbol: JNI_GetCreatedJavaVMs");
            dlclose(jvm);
            return false;
        }
        auto GetCreatedJavaVMs = reinterpret_cast<t_JNI_GetCreatedJavaVMs>(s_JNI_GetCreatedJavaVMs);

        // Call JNI_GetCreatedJavaVMs
        jint error = GetCreatedJavaVMs(&vm, 1, nullptr);
        if (error != JNI_OK) {
            logger::Log("Failed to obtain jvm");
            dlclose(jvm);
            return false;
        }

        // Attach current thread to JVM
        error = vm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
        if (error != JNI_OK) {
            logger::Log("Failed to attach thread to jvm");
            dlclose(jvm);
            return false;
        }

        // Get JVMTI environment
        error = vm->GetEnv(reinterpret_cast<void**>(&jvmtiEnv), JVMTI_VERSION_1_1);
        if (error != JNI_OK || !jvmtiEnv) {
            logger::Log("Failed to obtain jvmTi");
            dlclose(jvm);
            return false;
        }

        // Success
        dlclose(jvm);
        return true;
    }
}
