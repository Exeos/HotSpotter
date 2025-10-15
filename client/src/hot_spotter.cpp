#include "hot_spotter.hpp"

#include <cstdint>

#include "attacher/Attacher.hpp"
#include "logger/logger.hpp"

#include "imgui.h"
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
        if (!hot_spotter::logger::InitConsole()) {
            return;
        }
        hot_spotter::logger::Log("Initializing");

        Attacher* attacher = createAttacher();
        if (!attacher->attach(jvm, jniEnv, jvmTi)) {
            hot_spotter::logger::Log("Failed to attach to jvm.");
            tidy(); // Clean up before exiting
            return;
        }
        delete attacher;

        if (!capabilities::setCapabilities()) {
            hot_spotter::logger::Log("Failed to get or set Capabilities");
        }

        if (!hooks::initHooks()) {
            hot_spotter::logger::Log("Failed to init hooks");
        }

        if (!class_dumper::dump()) {
            hot_spotter::logger::Log("Failed to setup class dump");
        }

        hot_spotter::logger::Log("Initialized, starting gui");

        startGui();

        tidy();
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
            hot_spotter::logger::Log("Failed to remove hooks");
        }

        for (auto entry : classes) {
            // delete manually allocated class file data memory
            delete[] entry.second.second.second;
        }

        hot_spotter::logger::CloseConsole();
    }
}
