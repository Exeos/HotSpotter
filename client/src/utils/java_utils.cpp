#include "java_utils.hpp"

#include "../globals.hpp"

namespace hot_spotter::utils::java {
    std::vector<std::string> parseJavaModifiers(jint modifiers) {
        std::vector<std::string> result;
        // Access modifiers (mutually exclusive)
        if (modifiers & 0x0001) result.push_back("public");
        else if (modifiers & 0x0002) result.push_back("private");
        else if (modifiers & 0x0004) result.push_back("protected");

        // Non-access modifiers
        if (modifiers & 0x0008) result.push_back("static");
        if (modifiers & 0x0010) result.push_back("final");
        if (modifiers & 0x0020) result.push_back("synchronized");
        if (modifiers & 0x0040) result.push_back("volatile");
        if (modifiers & 0x0080) result.push_back("transient");
        if (modifiers & 0x0100) result.push_back("native");
        if (modifiers & 0x0200) result.push_back("interface");
        if (modifiers & 0x0400) result.push_back("abstract");
        if (modifiers & 0x0800) result.push_back("strict");
        if (modifiers & 0x1000) result.push_back("synthetic");
        if (modifiers & 0x2000) result.push_back("annotation");
        if (modifiers & 0x4000) result.push_back("enum");
        if (modifiers & 0x8000) result.push_back("mandated");

        return result;
    }

    std::string getJvmtiErrorName(jvmtiError error) {
        char *errorName;
        if (jvmtiError err = jvmTi->GetErrorName(error, &errorName); err != JVMTI_ERROR_NONE) {
            // failed to get error name, return error code as string
            return std::to_string(err);
        }

        std::string result(errorName);
        jvmTi->Deallocate(reinterpret_cast<unsigned char *>(errorName));

        return result;
    }
}
