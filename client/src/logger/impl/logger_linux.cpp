#include "../logger.hpp"
#include <iostream>
#include <mutex>
#include <chrono>
#include <ctime>

static std::mutex logger_mutex;

namespace hot_spotter::logger {

    static std::string get_timestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(now);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&tt));
        return std::string(buf);
    }

    bool InitConsole(const std::string& title) {
        // use default output because creating a new console and redirecting output is way too much effort on linux
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
        // Not required
    }
}