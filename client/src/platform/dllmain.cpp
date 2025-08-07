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
#include "../utils/ProgramState.h"
#include "../platform/windows_utils.h"

using std::cout;

#include "../hot_spotter.hpp"

DWORD WINAPI MainThread(HMODULE instance) {
    hot_spotter::init();
    FreeLibraryAndExitThread(instance, 0);
    return TRUE;
}

DWORD APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(instance);
            CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), instance, 0, nullptr);
            break;
        }
        case DLL_PROCESS_DETACH: {
//            MessageBoxA(nullptr, "detach", "detach", MB_OK | MB_ICONEXCLAMATION);
//            Logger::CloseConsole();
            break;
        }
    }
    return TRUE;
}


