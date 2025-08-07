//
// Created by Nick The Goat on 8/4/2025.
//

#include "../Attacher.hpp"
#include "../../logger/logger.hpp"
#include <windows.h>

class WindowsAttacher final : public Attacher {
public:
    bool attach(JavaVM*& vm, JNIEnv*& env, jvmtiEnv*& jvmtiEnv) override {
        HMODULE jvm_handle = LoadLibraryW(L"jvm.dll");
        if (!jvm_handle) {
            Logger::Log("Failed to obtain handle to jvm.dll");
            return false;
        }


        using t_JNI_GetCreatedJavaVMs = jint(*)(JavaVM**, jsize, jsize*);
        FARPROC proc = GetProcAddress(jvm_handle, "JNI_GetCreatedJavaVMs");
        if (!proc) {
            Logger::Log("Failed to find symbol: JNI_GetCreatedJavaVMs");
            FreeLibrary(jvm_handle);
            return false;
        }
        auto GetCreatedJavaVMs = reinterpret_cast<t_JNI_GetCreatedJavaVMs>(proc);


        jint error = GetCreatedJavaVMs(&vm, 1, nullptr);
        if (error != JNI_OK) {
            Logger::Log("Failed to obtain jvm");
            FreeLibrary(jvm_handle);
            return false;
        }


        error = vm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
        if (error != JNI_OK) {
            Logger::Log("Failed to attach thread to jvm");
            FreeLibrary(jvm_handle);
            return false;
        }

        error = vm->GetEnv(reinterpret_cast<void**>(&jvmtiEnv), JVMTI_VERSION_1_1);
        if (error != JNI_OK || !jvmtiEnv) {
            Logger::Log("Failed to obtain jvmTi");
            FreeLibrary(jvm_handle);
            return false;
        }

        // Success
        FreeLibrary(jvm_handle);
        return true;
    }
};

Attacher* createAttacher() {
    return new WindowsAttacher();
}