#pragma once
#include <windows.h>

// Options when requesting the kernel to open a process
struct OpenProcessOptions
{
	DWORD accessRights = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_TERMINATE;
	BOOL inheritChildHandle = false;
};

// Options when allocating virtual memory of a process
struct VirtualMemoryProcessOptions
{
	DWORD flAllocationTypes = MEM_RESERVE | MEM_COMMIT;
	DWORD flProtect = PAGE_READWRITE;
};