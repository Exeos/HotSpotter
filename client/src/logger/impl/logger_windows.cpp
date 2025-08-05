//
// Created by Nick The Goat on 8/4/2025.
//
//
// Created by Nick The Goat on 8/4/2025.
//

#include "../logger.hpp"
#include "../../platform/windows_utils.h"
#include "../../utils/ProgramState.h"
#include <iostream>
#include <mutex>
#include <chrono>
#include <ctime>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

static std::mutex logger_mutex;

static bool g_ConsoleAllocated = false;
static HANDLE g_consoleHandle = nullptr;

static FILE* g_StdOut{nullptr};
static FILE* g_StdIn{nullptr};
static FILE* g_StdErr{nullptr};

// Original file descriptors
static int g_OriginalStdOutFd = -1;
static int g_OriginalStdErrFd = -1;
static int g_OriginalStdInFd = -1;

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);

namespace Logger {

    static std::string get_timestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(now);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&tt));
        return std::string(buf);
    }



    bool ConsoleExists() {
        return GetConsoleWindow() != NULL;
    }

    BOOL WINAPI ConsoleHandler(DWORD dwCtrlType) {
        switch (dwCtrlType) {
            case CTRL_BREAK_EVENT:
            case CTRL_C_EVENT:
            case CTRL_CLOSE_EVENT:
                if (ProgramState::isRunning()) {
                    ProgramState::terminate();
                    // Wait for graceful shutdown before allowing console to close
                    if (ProgramState::waitForTermination(std::chrono::milliseconds(3000))) {
                        return TRUE; // Allow console to close after successful termination
                    } else {
                        // Force termination if timeout exceeded
                        ProgramState::markTerminated();
                        return TRUE;
                    }
                }
                return TRUE; // Already terminated, allow close
            default:
                return FALSE;
        }
    }

    bool InitConsole(const std::string& title) {
        if (ConsoleExists()) return true;
        // Save original file descriptors
        g_OriginalStdOutFd = _dup(_fileno(stdout));
        g_OriginalStdErrFd = _dup(_fileno(stderr));
        g_OriginalStdInFd = _dup(_fileno(stdin));

        if (AttachConsole(ATTACH_PARENT_PROCESS)) {
            g_ConsoleAllocated = false;
        } else if (AllocConsole()) {
            g_ConsoleAllocated = true;
        } else {
            std::string errormsg = Windows_Utils::GetLastErrorAsString();
            MessageBoxA(nullptr, errormsg.c_str(), "InitConsole Error", MB_OK | MB_ICONERROR);
            if (g_OriginalStdOutFd != -1) _dup2(g_OriginalStdOutFd, _fileno(stdout));
            if (g_OriginalStdErrFd != -1) _dup2(g_OriginalStdErrFd, _fileno(stderr));
            if (g_OriginalStdInFd != -1) _dup2(g_OriginalStdInFd, _fileno(stdin));
            return false;
        }
        SetConsoleTitleA((title + " | Windows Port made by DevOfDeath").c_str());
        if (g_ConsoleAllocated) {
            freopen_s(&g_StdOut, "CONOUT$", "w", stdout);
            freopen_s(&g_StdErr, "CONOUT$", "w", stderr);
            freopen_s(&g_StdIn, "CONIN$", "r", stdin);
            SetConsoleCtrlHandler(ConsoleHandler, TRUE);

            g_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

            if (g_consoleHandle != INVALID_HANDLE_VALUE) {
                DWORD consoleMode;
                if (GetConsoleMode(g_consoleHandle, &consoleMode)) {
                    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                    SetConsoleMode(g_consoleHandle, consoleMode);
                }
            }
        }
        HWND hwnd = GetConsoleWindow();
        if (hwnd) ShowWindow(hwnd, SW_SHOW);
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
        if (!ConsoleExists()) return;
        if (g_ConsoleAllocated) {
            SetConsoleCtrlHandler(ConsoleHandler, FALSE);
            if (g_StdOut) {
                fclose(g_StdOut);
                g_StdOut = nullptr;
            }
            if (g_StdErr) {
                fclose(g_StdErr);
                g_StdErr = nullptr;
            }
            if (g_StdIn) {
                fclose(g_StdIn);
                g_StdIn = nullptr;
            }
            FreeConsole();
            g_consoleHandle = nullptr;
            g_ConsoleAllocated = false;

            // Restore original streams
            if (g_OriginalStdOutFd != -1) {
                _dup2(g_OriginalStdOutFd, _fileno(stdout));
                _close(g_OriginalStdOutFd);
                g_OriginalStdOutFd = -1;
            }
            if (g_OriginalStdErrFd != -1) {
                _dup2(g_OriginalStdErrFd, _fileno(stderr));
                _close(g_OriginalStdErrFd);
                g_OriginalStdErrFd = -1;
            }
            if (g_OriginalStdInFd != -1) {
                _dup2(g_OriginalStdInFd, _fileno(stdin));
                _close(g_OriginalStdInFd);
                g_OriginalStdInFd = -1;
            }
        }
    }
}