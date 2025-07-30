#include "hot_spotter.hpp"

#include <cstdint>

#include "attacher/Attacher.hpp"
#include "logger/logger.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <cstdio>
#include <GLFW/glfw3.h>

#include "capabilities/capabilities.hpp"
#include "gui/MainWindow.hpp"
#include "hooks/hooks.hpp"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace hot_spotter {

    JavaVM* jvm = nullptr;
    JNIEnv* jniEnv = nullptr;
    jvmtiEnv* jvmTi = nullptr;

    void init() {
        if (!Logger::InitConsole()) {
            return;
        }
        Logger::Log("Initializing");

        Attacher* attacher = createAttacher();
        if (!attacher->attach(jvm, jniEnv, jvmTi)) {
            Logger::Log("Failed to attach to jvm.");
            return;
        }
        delete attacher;

        if (!capabilities::setCapabilities()) {
            Logger::Log("Failed to get or set Capabilities");
        }

        if (!hooks::initHooks()) {
            Logger::Log("Failed to init hooks");
        }

        Logger::Log("Initialized");

        Logger::LogFormat("jvm_handle: %p", reinterpret_cast<intptr_t>(jvm));
        Logger::LogFormat("jni_env: %p", reinterpret_cast<intptr_t>(jniEnv));
        Logger::LogFormat("jvm_ti: %p", reinterpret_cast<intptr_t>(jvmTi));

        Logger::Log("Collecting classes and using Retransform to pass them to ClassLoad hook");

        jclass* classes;
        jint classCount;
        if (jvmtiError error = jvmTi->GetLoadedClasses(&classCount, &classes); error != JVMTI_ERROR_NONE) {
            Logger::Log("Failed to get loaded classes");
        }

        std::vector<jclass> modifiable;
        for (int i = 0; i < classCount; ++i) {
            jboolean isModifiable = false;
            if (jvmTi->IsModifiableClass(classes[i], &isModifiable) == JVMTI_ERROR_NONE && isModifiable) {
                modifiable.push_back(classes[i]);
            }
        }

        if (!modifiable.empty()) {
            if (jvmTi->RetransformClasses(modifiable.size(), modifiable.data()) != JVMTI_ERROR_NONE) {
                Logger::Log("RetransformClasses failed");
            }
        }

        auto* mainWindow = new gui::MainWindow();
        mainWindow->init();
        mainWindow->render();
        mainWindow->close();

        delete mainWindow;
    }

    void tidy() {
        Logger::CloseConsole();
    }
}
