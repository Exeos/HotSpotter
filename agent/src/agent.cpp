#include "agent.hpp"

#include "jvm/attacher/attacher.hpp"
#include "jvm/capabilities/capabilities.hpp"
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
    }

    void destroy() {
        logger::CloseConsole();
    }
}
