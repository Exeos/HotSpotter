#pragma once

#include <string>
#include <vector>

#include "jni_md.h"

namespace hot_spotter::utils::java {
    std::vector<std::string> parseJavaModifiers(jint modifiers);
}
