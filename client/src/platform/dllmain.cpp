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

    std::thread(hot_spotter::init).detach();
//    hot_spotter::init();
    /*MessageBoxA(nullptr, "PLEASE GOD I'VE BEEN STUCK AT THIS SHIT FOR ETERNITY", "HELP", MB_HELP);
    if (!Logger::InitConsole()) {
        FreeLibraryAndExitThread(instance, TRUE);
    }
    printf("Yooo wsgggg from console world!\n");
    Logger::Log("Logging wit da Logga!");

    int i{};
    while (ProgramState::isRunning()) {
//        Logger::Log(std::to_string(i));
        MessageBoxA(nullptr, std::to_string(i).c_str(), "counter", MB_OK | MB_ICONEXCLAMATION);
        i++;
        Sleep(100);
    }
    MessageBoxA(nullptr, "thread", "thread", MB_OK | MB_ICONEXCLAMATION);
    FreeLibraryAndExitThread(instance, 0);*/
    while (ProgramState::isRunning()) {

    }
    ProgramState::markTerminated();
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


