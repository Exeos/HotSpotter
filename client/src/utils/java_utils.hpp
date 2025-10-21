#pragma once

#include <string>
#include <vector>

#include "jni_md.h"
#include "jvmti.h"

namespace hot_spotter::utils::java {
    std::vector<std::string> parseJavaModifiers(jint modifiers);

    std::string getJvmtiErrorName(jvmtiError error);
}
