#include "../logger.hpp"
#include <windows.h>
#include <chrono>
#include <ctime>
#include <string>
#include <string_view>
#include <iostream>
#include <mutex>
#include <cstdio>
#include <fcntl.h>

namespace hot_spotter::logger {

    static std::mutex logger_mutex;

    static std::string get_timestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};

        localtime_s(&tm, &tt);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
        return std::string(buf);
    }

    bool InitConsole(const std::string& title) {
        if (!GetConsoleWindow()) {
            if (!AllocConsole()) {
                return false;
            }
        }

        FILE* f = nullptr;
        if (freopen_s(&f, "CONOUT$", "w", stdout) != 0) return false;
        if (freopen_s(&f, "CONOUT$", "w", stderr) != 0) return false;
        if (freopen_s(&f, "CONIN$",  "r", stdin) != 0) return false;

        // Ensure C++ streams use the C FILE* handles
        std::ios::sync_with_stdio(true);
        std::cout.clear(); std::cerr.clear(); std::cin.clear();
        SetConsoleTitleA(title.c_str());

        return true;
    }

    void CloseConsole() {
        std::lock_guard<std::mutex> lk(logger_mutex);
        fflush(stdout);
        fflush(stderr);

        FreeConsole();
    }

    std::string GetInput() {
        std::lock_guard<std::mutex> lk(logger_mutex);
        std::string line;
        if (!std::getline(std::cin, line)) {
            line.clear();
        }
        return line;
    }

    void Log(const std::string_view& message) {
        std::lock_guard<std::mutex> lk(logger_mutex);
        std::cout << "[" << get_timestamp() << "] " << message << std::endl;
    }
}