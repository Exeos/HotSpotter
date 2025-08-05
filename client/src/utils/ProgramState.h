//
// Created by Nick The Goat on 8/4/2025.
//

#pragma once
#include <atomic>
#include <chrono>
#include <thread>

#ifndef HOTSPOTTER_PROGRAMSTATE_H
#define HOTSPOTTER_PROGRAMSTATE_H

namespace ProgramState {
    inline std::atomic<bool>& runningFlag() {
        static std::atomic<bool> flag{true};
        return flag;
    }

    inline std::atomic<bool>& terminatedFlag() {
        static std::atomic<bool> flag{false};
        return flag;
    }

    inline void terminate() {
        runningFlag().store(false, std::memory_order_release);
    }

    inline void markTerminated() {
        terminatedFlag().store(true, std::memory_order_release);
    }

    inline bool isRunning() {
        return runningFlag().load(std::memory_order_acquire);
    }

    inline bool hasTerminated() {
        return terminatedFlag().load(std::memory_order_acquire);
    }

    inline bool waitForTermination(std::chrono::milliseconds timeout = std::chrono::milliseconds(5000)) {
        auto start = std::chrono::steady_clock::now();
        while (!hasTerminated() &&
               std::chrono::steady_clock::now() - start < timeout) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return hasTerminated();
    }
}

#endif //HOTSPOTTER_PROGRAMSTATE_H
