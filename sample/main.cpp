
#include <Windows.h>
#include "stdio.h"
#include "userdef_server.h"

#pragma comment(lib, "user32.lib")

bool inject_dll(HMODULE &hmodule, HHOOK &hhook, int target_threadid);
void eject_dll(HMODULE &hmodule, HHOOK &hhook);



int main(int argc, char **argv) {

    if (argc==2 && _stricmp(argv[1], "-installhook")){
        int test_threadid = 27580;
        HMODULE hmodule = NULL;
        HHOOK hhook = NULL;

        if (!inject_dll(hmodule, hhook, test_threadid)){
            printf("Injection failed.\n");
            return 1;
        }

        printf("Press any key to stop.");
        getchar();

        eject_dll(hmodule, hhook);
        return 0;
    }

    start_server(9090);
    return 0;
}

//
// This sample demonstrate how to inject dll by SetWindowsHookEx.
// You can inject apifwrd.dll to destination process simply by AppInit_DLLs registry key or
// implement your own method like CreateRemoteThread, ...
//

bool inject_dll(HMODULE &hmodule, HHOOK &hhook, int target_threadid)
{
    // Already hooked.
    if (hhook!=NULL) return true;

    // Load library
    if (hmodule==NULL) {
        hmodule = LoadLibraryW(L"apifwrd.dll");
        if (hmodule==NULL) return false;
    }

    // apifwrd.dll is ready to support SetWindowsHookEx() way.
    HOOKPROC addr = (HOOKPROC) GetProcAddress(hmodule, "HelperHookProcForSetWindowsHookEx");
    if (addr==NULL) {
        FreeLibrary(hmodule);
        return false;
    }

    // Install hook
    hhook = SetWindowsHookEx(WH_KEYBOARD, addr, hmodule, target_threadid);
    if (hhook==NULL) {
        FreeLibrary(hmodule);
        return false;
    }

    return true;
}

void eject_dll(HMODULE &hmodule, HHOOK &hhook)
{
    if (hhook) {
        UnhookWindowsHookEx(hhook);
        hhook = NULL;
    }

    if (hmodule) {
        FreeLibrary(hmodule);
        hmodule = NULL;
    }

    return;
}