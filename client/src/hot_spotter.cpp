#include "hot_spotter.hpp"

#include "attacher/Attacher.hpp"
#include "logger/logger.hpp"

#include <map>
#include <ranges>

#include "capabilities/capabilities.hpp"
#include "class_dumper/class_dumper.hpp"
#include "globals.hpp"
#include "gui/gui.hpp"
#include "hooks/hooks.hpp"
#include "jni.h"
#include "jvmti.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace hot_spotter {
  JavaVM *jvm = nullptr;
  JNIEnv *jniEnv = nullptr;
  jvmtiEnv *jvmTi = nullptr;
  class_map_t classes = {};

  void init() {
    if (!logger::InitConsole()) {
      return;
    }
    logger::Log("Initializing");

    Attacher *attacher = createAttacher();
    if (!attacher->attach(jvm, jniEnv, jvmTi)) {
      logger::Log("Failed to attach to jvm.");
      tidy(); // Clean up before exiting
      return;
    }
    delete attacher;

    if (!capabilities::setCapabilities()) {
      logger::Log("Failed to get or set Capabilities");
    }

    if (!hooks::initHooks()) {
      logger::Log("Failed to init hooks");
    }

    if (!class_dumper::dump()) {
      logger::Log("Failed to setup class dump");
    }

    logger::Log("Initialized, starting gui");

    startGui();

    logger::Log("Exiting. TEST");

    tidy();
  }

  void startGui() {
    if (gui::init()) {
      gui::render();
      gui::cleanup();
    }
  }

  void tidy() {
    if (!hooks::removeHooks()) {
      logger::Log("Failed to remove hooks");
    }

    for (const auto &[jclazz, clazzData]: classes | std::views::values) {
      // delete manually allocated class file data memory
      jniEnv->DeleteGlobalRef(jclazz);
      delete[] clazzData.second;
    }

    logger::CloseConsole();
  }
} // namespace hot_spotter
