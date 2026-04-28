#pragma once

#include <string>
#include <string_view>

// pasted from copilot
namespace logger {
    bool InitConsole(const std::string& title = "HotSpotter");
    void CloseConsole();
    void Log(const std::string_view& message);
    template<typename... Args> void LogFormat(const char* format, Args... args);
}

// Include template implementation
#include "logger.inl"
