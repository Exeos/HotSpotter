#include "agent.hpp"

#include "jvm/attacher/attacher.hpp"
#include "jvm/capabilities/capabilities.hpp"
#include "jvm/hooks/hooks.hpp"
#include "logger/logger.hpp"

namespace agent {

    void init() {
        if (!logger::InitConsole()) {
            return;
        }
        logger::Log("Initializing");

        if (!jvm::attacher::attach(vm, jniEnv, jvmTi)) {
            logger::Log("Failed to attach to the JVM");
            destroy();
            return;
        }

        if (!jvm::capabilities::setCapabilities()) {
            logger::Log("Failed to set Capabilities. This might lead to problems");
        }

        if (!jvm::hooks::initHooks()) {
            logger::Log("Failed to init hooks");
        }
    }

    void destroy() {
        logger::CloseConsole();
    }
}
