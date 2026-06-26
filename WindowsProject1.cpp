// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject1.h"
#include <string>
#include <tlhelp32.h> // To create snapshot of processes

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Process(HWND, UINT, WPARAM, LPARAM);
void                Hax(const std::wstring&);
DWORD               GetProcessIdByName(const std::wstring&);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    OutputDebugString(L"Entry point called\n");

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        // If a keyboard shortcut (the list is in the .rc file Accelerator Table) is pressed
        // this function returns True, so we skip the if conditional statement.
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg); // Translate user-pressed keys (scan code --> virtual keys into their actual text representation)
            DispatchMessage(&msg);  // The OS sends the msg to the corresponding window's callback function for processing
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//  Note: This registers the BLUEPRINT of the windows class (it does not instantiate an object here!) to the OS
//  So when you make a window later through CreateWindow and pass the name of the blueprint to use, here szWindowClass,
//  it will then instantiate an object with properties defaulted to the below.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   // notice how szTitle and other parameters are not part of the Windows class blueprint above?
   // that is because the blueprint is meant for all windows sharing the same properties, but titles like szTitle
   // are customizable values specific to each individual instance so semantically it makes more sense to pass it
   // as a parameter rather than defined in the blueprint
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//  https://learn.microsoft.com/en-us/windows/win32/winmsg/about-messages-and-message-queues#system-defined-messages
//  https://learn.microsoft.com/en-us/windows/win32/menurc/menu-notifications
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    wchar_t buffer[256];

    // Format the string exactly like printf, but into our buffer array
    //swprintf_s(buffer, 256, L"Inside WndProc Callback. Message ID: %d\n", message);
    //OutputDebugString(buffer);

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_PROCESSNAME:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_PROCESSBOX), hWnd, Process);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for process box.
INT_PTR CALLBACK Process(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            OutputDebugString(L"you clicked on cancel\n");
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDOK)
        {
            // Accept the user string
            HWND hControl = GetDlgItem(hDlg, IDC_INPUT);
            int length = GetWindowTextLengthW(hControl);
            if (length == 0) {
                return (INT_PTR)TRUE;
            }

            // Create a dynamic string and resize it to hold the text + null terminator
            std::wstring usertext;
            usertext.resize(length + 1);

            // Call GetWindowTextW using the internal buffer pointer
            // data() gives write access to the underlying array in C++11 and newer
            int charactersCopied = GetWindowTextW(hControl, usertext.data(), static_cast<int>(usertext.size()));

            // Trim the trailing null terminator so std::wstring functions correctly
            usertext.resize(charactersCopied);

            // 'usertext' now contains the exact string from the window!
            OutputDebugString(usertext.c_str());
            Hax(usertext.c_str());



            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_INPUT)
        {
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Returns the process ID in the current snapshot, given the process name
// to search for
DWORD GetProcessIdByName(const std::wstring& processName) {
    DWORD pid = -1;
    DWORD flags = TH32CS_SNAPPROCESS;
    DWORD th32ProcessId = 0;
    HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(flags, th32ProcessId);
    PROCESSENTRY32 pe32;
    if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
        OutputDebugString(L"Failed to obtain snapshot handle\n");
        return -1;
    }

    // Enumerate the process name 
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnapshot, &pe32)) {
        return -1;
    }

    // Print metadata for target process if found
    do 
    {
        if (processName == pe32.szExeFile)
        {
            pid = pe32.th32ProcessID;
        }
    } while (Process32Next(hProcessSnapshot, &pe32));


    return pid;
}

// Creates a kernel handle to another local process, allocates virtual memory,
// write to the allocated space, and execute the written code
void Hax(const std::wstring& processName) {
    // Open process to retrieve its kernel handle
    DWORD accessRights = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_TERMINATE;
    BOOL childInheritHandle = false;
    DWORD processId = GetProcessIdByName(processName);

    if (processId == -1) {
        OutputDebugString(L"Failed to obtain target process id\n");
        return;
    }
    OutputDebugString((L"Target process discovered, Process ID: " + std::to_wstring(processId) + L"\n").c_str());

    // Obtain a handle to the target process
    HANDLE htargetProcess = OpenProcess(accessRights, childInheritHandle, processId);
    if (!htargetProcess)
    {
        OutputDebugString(L"Failed to obtain target process handle\n");
        return;
    }
    OutputDebugString(L"Obtained handle to target process\n");

    // Allocate virtual memory to the target process
    const wchar_t test_str[] = L"doll";

    // wcslen() counts the characters in a string (excluding null terminator)
    SIZE_T dwSize = (wcslen(test_str) + 1) * sizeof(wchar_t);

    // MEM_RESERVE reserves a range of a process's VAS, MEM_COMMIT guarantees the process is allocated a given
    // amount of physical storage space like 4KB. Note: It does not guarantee fixed physical addresses (since page frames get moved around
    // all the time), it guarantees physical storage space.


    LPVOID pAllocatedMemory = VirtualAllocEx(
        htargetProcess,
        NULL,
        dwSize,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );
    if (!pAllocatedMemory)
    {
        OutputDebugString(L"Failed to allocate virtual memory for target\n");
        return;
    }
    OutputDebugString(L"Allocated pages for target\n");
    
    // Print the base address of page
    wchar_t buffer[128];
    std::swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%p", (void*)pAllocatedMemory);
    OutputDebugString((std::wstring(buffer) + L"\n").c_str());

    // Write to the address
    BOOL writeSuccess = WriteProcessMemory(htargetProcess, pAllocatedMemory, test_str, sizeof(test_str), NULL);
    if (writeSuccess == 0)
    {
        OutputDebugString(L"Failed to write test string to target process page\n");
    }
    OutputDebugString(L"Wrote test string to target process page\n");

    // Read the string from the address
    wchar_t buf[128] = { 0 };
    BOOL readSuccess = ReadProcessMemory(htargetProcess, pAllocatedMemory, buf, sizeof(test_str), NULL);
    if (readSuccess == 0)
    {
        OutputDebugString(L"Failed to read test string from target process\n");
    }
    OutputDebugString((L"Target string fetched: " + std::wstring(buf) + L"\n").c_str());


}