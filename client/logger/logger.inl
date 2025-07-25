#pragma once

#include <cstdio>
#include <vector>

#include "logger.hpp"

namespace Logger {
    template<typename... Args>
    void LogFormat(const char* format, Args... args) {
        // Calculate buffer size needed
        int size_needed = snprintf(nullptr, 0, format, args...) + 1;
        if (size_needed <= 0) {
            return;
        }

        // Allocate buffer and format the string
        std::vector<char> buffer(size_needed);
        snprintf(buffer.data(), buffer.size(), format, args...);

        // Pass to regular log function
        Log(std::string_view(buffer.data(), size_needed - 1));
    }
}
