#include "hot_spotter.hpp"

#include <cstdint>

#include "attacher/Attacher.hpp"
#include "logger/logger.hpp"
#include "utils/ProgramState.h"

#include "imgui.h"
#include <string>
#include <map>

#include "globals.hpp"
#include "jni.h"
#include "jvmti.h"
#include "capabilities/capabilities.hpp"
#include "class_dumper/class_dumper.hpp"
#include "gui/MainWindow.hpp"
#include "hooks/hooks.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace hot_spotter {

    JavaVM* jvm = nullptr;
    JNIEnv* jniEnv = nullptr;
    jvmtiEnv* jvmTi = nullptr;
    class_map_t classes = {};

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

        if (!class_dumper::dump()) {
            Logger::Log("Failed to setup class dump");
        }

        Logger::Log("Initialized, starting gui");
        startGui();

        Logger::Log("Exiting, cleaning up");
        tidy();
        Logger::Log("Bye bye :)");
        ProgramState::terminate();
    }

    void startGui() {
        auto* mainWindow = new gui::MainWindow();
        if (mainWindow->init()) {
            mainWindow->render();
            mainWindow->close();
        }
        delete mainWindow;
    }


    void tidy() {
        if (!hooks::removeHooks()) {
            Logger::Log("Failed to remove hooks");
        }

        for (auto entry : classes) {
            // delete manually allocated class file data memory
            delete[] entry.second.second.second;
        }

        Logger::CloseConsole();
    }
}
