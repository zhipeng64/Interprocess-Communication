#pragma once
#include <windows.h>
#include <string>

// Prints the address in hex that an input pointer holds
void PrintPointerAddress(void* ptr)
{
    wchar_t buffer[128];
    std::swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%p", ptr);
    OutputDebugString((std::wstring(buffer) + L"\n").c_str());
}