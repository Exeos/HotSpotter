//
// Created by Nick The Goat on 8/4/2025.
//
#pragma once
#ifndef HOTSPOTTER_WINDOWS_UTILS_H
#define HOTSPOTTER_WINDOWS_UTILS_H
#include <windows.h>
#include <string>


namespace Windows_Utils {
    inline std::string GetLastErrorAsString()
    {
        DWORD errorMessageID = ::GetLastError();
        if(errorMessageID == 0) {
            return std::string();
        }

        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&messageBuffer, 0, NULL);

        if (size == 0 || messageBuffer == nullptr) {
            return "Unknown error occurred";
        }

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);

        // Remove trailing newline characters if present
        while (!message.empty() && (message.back() == '\r' || message.back() == '\n')) {
            message.pop_back();
        }

        return message;
    }
    inline std::string GetCurrentConsoleTitle()
    {
        char title[MAX_PATH];
        DWORD length = GetConsoleTitleA(title, MAX_PATH);
        if (length > 0) {
            return std::string(title, length);
        }
        return "";
    }
}

#endif //HOTSPOTTER_WINDOWS_UTILS_H
