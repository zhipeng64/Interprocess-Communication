// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>

DWORD WINAPI Payload(LPVOID lpParameter)
{
    MessageBox(NULL, L"dll injected!", L"Hi", MB_OKCANCEL);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    // Windows loader uses a loader lock that prevents any thread from interacting with the current dll 
    // Makes a new thread (must wait for loader lock to be released), return from main thread to release loader lock.
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hThread = CreateThread(NULL, 0, Payload, NULL, 0, NULL);
        return TRUE;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

