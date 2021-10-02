#include "pch.h"
#include "windows.h"
#include <stdio.h>

#define _WIN32_WINNT 0x0400

#include<fstream>
#include<windows.h>

HHOOK hKeyboardHook;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) int KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
    DWORD SHIFT_key = 0;
    DWORD CTRL_key = 0;
    DWORD ALT_key = 0;


    if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
    {
        KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
        DWORD dwMsg = 1;
        dwMsg += hooked_key.scanCode << 16;
        dwMsg += hooked_key.flags << 24;
        char lpszKeyName[1024] = { 0 };

        int i = GetKeyNameText(dwMsg, LPWSTR(lpszKeyName + 1), 0xFF) + 1;

        int key = hooked_key.vkCode;

        SHIFT_key = GetAsyncKeyState(VK_SHIFT);
        CTRL_key = GetAsyncKeyState(VK_CONTROL);
        ALT_key = GetAsyncKeyState(VK_MENU);

        printf("Keycode = %c\n", key);

        if (key >= 'A' && key <= 'Z')
        {

            if (GetAsyncKeyState(VK_SHIFT) >= 0) key += 32;

            /*********************************************
            ***   Hotkey scope                         ***
            ***   do stuff here                        ***
            **********************************************/

            if (CTRL_key != 0 && key == 'y')
            {
                MessageBox(NULL, LPCWSTR("CTRL-y was pressed\nLaunch your app here"), LPCWSTR("H O T K E Y"), MB_OK);
                CTRL_key = 0;

                //do stuff here




            }

            if (CTRL_key != 0 && key == 'q')
            {
                MessageBox(NULL, LPCWSTR("Shutting down"), LPCWSTR("H O T K E Y"), MB_OK);
                PostQuitMessage(0);
            }




            SHIFT_key = 0;
            CTRL_key = 0;
            ALT_key = 0;

        }

    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}