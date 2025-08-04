//
// Created by Nick The Goat on 8/3/2025.
//

#include <iostream>
#include <windows.h>
#include <thread>
#include <fstream>
#include <cstdio>
#include <io.h>
#include "windows_utils.h"
#include "../logger/logger.hpp"

using std::cout;

void CleanupConsole();

#include "../hot_spotter.hpp"

HMODULE g_hModule = nullptr;

DWORD WINAPI MainThread(LPVOID lpParam) {
    HMODULE hModule = (HMODULE)lpParam;
//    std::thread(hot_spotter::init).detach();
//    hot_spotter::init();
    MessageBoxA(nullptr, "PLEASE GOD I'VE BEEN STUCK AT THIS SHIT FOR ETERNITY", "HELP", MB_HELP);
    if (!Logger::InitConsole()) {
        FreeLibraryAndExitThread(hModule, TRUE);
    }
    printf("Yooo wsgggg from console world!\n");
    Logger::Log("Logging wit da Logga!");
    Logger::CloseConsole();

    FreeLibraryAndExitThread(hModule, TRUE);
    return TRUE;
}

DWORD APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH: {
            g_hModule = hModule;
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
            break;
        }
        case DLL_PROCESS_DETACH: {
            Logger::CloseConsole();
            break;
        }
    }
    return TRUE;
}

namespace Windows_Utils {
    HMODULE GetModuleHandle() {
        return g_hModule;
    }
}

