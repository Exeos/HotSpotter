#include "../attacher.hpp"
#include <windows.h>

#include "../../../logger/logger.hpp"

namespace jvm::attacher {

    bool attach(JavaVM*& vm, JNIEnv*& env, jvmtiEnv*& jvmtiEnv) {
        HMODULE jvm_handle = LoadLibraryW(L"jvm.dll");
        if (!jvm_handle) {
            logger::Log("Failed to obtain handle to jvm.dll");
            return false;
        }


        using t_JNI_GetCreatedJavaVMs = jint(*)(JavaVM**, jsize, jsize*);
        FARPROC proc = GetProcAddress(jvm_handle, "JNI_GetCreatedJavaVMs");
        if (!proc) {
            logger::Log("Failed to find symbol: JNI_GetCreatedJavaVMs");
            FreeLibrary(jvm_handle);
            return false;
        }
        auto GetCreatedJavaVMs = reinterpret_cast<t_JNI_GetCreatedJavaVMs>(proc);

        jint error = GetCreatedJavaVMs(&vm, 1, nullptr);
        if (error != JNI_OK) {
            logger::Log("Failed to obtain jvm");
            FreeLibrary(jvm_handle);
            return false;
        }

        error = vm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
        if (error != JNI_OK) {
            logger::Log("Failed to attach thread to jvm");
            FreeLibrary(jvm_handle);
            return false;
        }

        error = vm->GetEnv(reinterpret_cast<void**>(&jvmtiEnv), JVMTI_VERSION_1_1);
        if (error != JNI_OK || !jvmtiEnv) {
            logger::Log("Failed to obtain jvmTi");
            FreeLibrary(jvm_handle);
            return false;
        }

        // Success
        FreeLibrary(jvm_handle);
        return true;
    }
}
