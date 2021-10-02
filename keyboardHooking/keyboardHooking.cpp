#define _WIN32_WINNT 0x0400
#pragma comment( lib, "user32.lib" )

#include <iostream>
#include <windows.h>
#include <stdio.h>

HHOOK hKeyboardHook;

__declspec(dllexport) LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam)
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

        int i = GetKeyNameText(dwMsg, LPWSTR((lpszKeyName + 1)), 0xFF) + 1;

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

void MessageLoop()
{
    MSG message;
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

DWORD WINAPI my_HotKey(LPVOID lpParm)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    if (!hInstance) hInstance = LoadLibrary(LPCWSTR(lpParm));
    if (!hInstance) return 1;

    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, NULL);
    MessageLoop();
    UnhookWindowsHookEx(hKeyboardHook);
    return 0;
}

int main(int argc, char** argv)
{
    HANDLE hThread;
    DWORD dwThread;

    printf("CTRL-y  for  H O T K E Y  \n");
    printf("CTRL-q  to quit  \n");

    hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)my_HotKey, (LPVOID)argv[0], NULL, &dwThread);

    /* uncomment to hide console window */
 //ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);

    if (hThread) return WaitForSingleObject(hThread, INFINITE);
    else return 1;

}