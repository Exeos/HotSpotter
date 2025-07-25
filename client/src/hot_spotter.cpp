#include "hot_spotter.hpp"

#include <cstdint>

#include "../attacher/Attacher.hpp"
#include "../logger/logger.hpp"

namespace hot_spotter {

    void init() {
        if (!Logger::InitConsole()) {
            goto exit;
        }
        Logger::Log("Allocated Console");

        if (Attacher* attacher = createAttacher(); !attacher->attach(jvm, jniEnv, jvmTi)) {
            Logger::Log("Failed to attach to jvm.");
            goto exit;
        }

        Logger::LogFormat("jvm_handle: %p", reinterpret_cast<intptr_t>(jvm));
        Logger::LogFormat("jni_env: %p", reinterpret_cast<intptr_t>(jniEnv));
        Logger::LogFormat("jvm_ti: %p", reinterpret_cast<intptr_t>(jvmTi));

        exit:
        tidy();
    }

    void tidy() {
        Logger::CloseConsole();
    }
}
