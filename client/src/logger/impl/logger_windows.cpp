//
// Created by Nick The Goat on 8/4/2025.
//
//
// Created by Nick The Goat on 8/4/2025.
//

#include "../logger.hpp"
#include "../../platform/windows_utils.h"
#include <iostream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

static std::mutex logger_mutex;
static HANDLE g_consoleHandle = nullptr;
static FILE* g_consoleStream = nullptr;

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);

namespace Logger {

    static std::string get_timestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(now);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&tt));
        return std::string(buf);
    }

    //Returns the last Win32 error, in string format. Returns an empty string if there is no error.
    std::string GetLastErrorAsString()
    {
        //Get the error message ID, if any.
        DWORD errorMessageID = ::GetLastError();
        if(errorMessageID == 0) {
            return std::string(); //No error message has been recorded
        }

        LPSTR messageBuffer = nullptr;

        //Ask Win32 to give us the string version of that message ID.
        //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        //Copy the error message into a std::string.
        std::string message(messageBuffer, size);

        //Free the Win32's string's buffer.
        LocalFree(messageBuffer);

        return message;
    }

    std::string GetCurrentConsoleTitle() {
        char title[MAX_PATH];
        DWORD length = GetConsoleTitleA(title, MAX_PATH);
        if (length > 0) {
            return std::string(title, length);
        }
        return "";
    }

    bool ConsoleExists() {
        return GetConsoleWindow() != NULL;
    }

    BOOL WINAPI ConsoleHandler(DWORD dwCtrlType) {
        switch (dwCtrlType) {
            case CTRL_CLOSE_EVENT:
                CloseConsole();
                FreeLibraryAndExitThread(Windows_Utils::GetModuleHandleA(), TRUE);
                return TRUE;
            case CTRL_C_EVENT:
            case CTRL_BREAK_EVENT:
                return TRUE;
            default:
                return FALSE;
        }
    }

    bool InitConsole(const std::string& title) {
        if (ConsoleExists()) return true;
        bool attached_to_existing = false;

        // Try to attach to parent console first
        if (AttachConsole(ATTACH_PARENT_PROCESS)) {
            attached_to_existing = true;
        } else if (AllocConsole()) {
            // If attach failed, allocate a new console
            attached_to_existing = false;
        } else {
            std::string errormsg = GetLastErrorAsString();
            MessageBoxA(nullptr, errormsg.c_str(), "InitConsole Error", MB_OK | MB_ICONERROR);
            return false;
        }
        SetConsoleTitleA((title + " | Windows Port made by DevOfDeath").c_str());
        // Set console title (only if we allocated a new console)
        if (!attached_to_existing) {
            // Redirect stdout, stdin, stderr to console (only for new console)
            g_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            freopen_s(&g_consoleStream, "CONOUT$", "w", stdout);
            freopen_s(&g_consoleStream, "CONOUT$", "w", stderr);
            freopen_s(&g_consoleStream, "CONIN$", "r", stdin);
            SetConsoleCtrlHandler(ConsoleHandler, TRUE);

            HWND hwnd = GetConsoleWindow();
            if (hwnd) ShowWindow(hwnd, SW_SHOW);
        }

        // Get console handle
        g_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        // Enable ANSI escape sequences for colored output (Windows 10+)
        DWORD mode = 0;
        GetConsoleMode(g_consoleHandle, &mode);
        SetConsoleMode(g_consoleHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

        return true;
    }

    void Log(const std::string_view& message) {
        std::lock_guard<std::mutex> lock(logger_mutex);
        std::cout << "[" << get_timestamp() << "] " << message << std::endl;
    }

    std::string GetInput() {
        std::lock_guard<std::mutex> lock(logger_mutex);
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    void CloseConsole() {
        if (g_consoleStream) {
            fclose(g_consoleStream);
            g_consoleStream = nullptr;
        }

        // Close redirected C runtime I/O streams
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        // Reset CRT file descriptors to invalid
        /*_close(_fileno(stdin));
        _close(_fileno(stdout));
        _close(_fileno(stderr));*/

        // Clear internal buffers
        std::ios::sync_with_stdio(false);

        // Unset console handles
        SetStdHandle(STD_INPUT_HANDLE, nullptr);
        SetStdHandle(STD_OUTPUT_HANDLE, nullptr);
        SetStdHandle(STD_ERROR_HANDLE, nullptr);

        // Remove handler and free console
        FreeConsole();
        SetConsoleCtrlHandler(ConsoleHandler, FALSE);

        g_consoleHandle = nullptr;
    }
}