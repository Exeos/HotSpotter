#include <iostream>
#include <windows.h>
#include <thread>
#include "../hot_spotter.hpp"

DWORD WINAPI MainThread(HMODULE instance) {
    hot_spotter::init();
    FreeLibraryAndExitThread(instance, 0);

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        if (auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), hinstDLL, 0, nullptr)) {
            CloseHandle(handle);
        }
    }

    return true;
}


