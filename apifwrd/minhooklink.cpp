#include <Windows.h>
#include <stdio.h>
#include "minhooklink.h"


#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

extern HOOK_ITEM _hook_items[];


BOOL start_hook()
{
    // Initialize MinHook.
    if (MH_Initialize() != MH_OK)
    {
        printf("MH_Initialize() failed.\n");
        return false;
    }
   
    for (int i=0 ; _hook_items[i].procName!=NULL ; ++i) {
        if (MH_CreateHookApi(
            _hook_items[i].moduleName, _hook_items[i].procName, _hook_items[i].pDetour, _hook_items[i].pOriginal) != MH_OK)
        {
            printf("MH_CreateHookApiEx('%s') failed.\n", _hook_items[i].procName);
            return false;
        }
    }

    bool result = true;
    for (int i=0 ; _hook_items[i].procName!=NULL ; ++i) {
        HMODULE hModule;
        LPVOID  pTarget;

        hModule = GetModuleHandleW(_hook_items[i].moduleName);
        if (hModule == NULL) {
            result = false;
            break;
        }

        pTarget = GetProcAddress(hModule, _hook_items[i].procName);
        if (pTarget == NULL) {
            result = false;
            break;
        }

        if (MH_EnableHook(pTarget) != MH_OK) {
            result = false;
            break;
        }
    }

    return result;
}

void stop_hook()
{
    for (int i=0 ; _hook_items[i].procName!=NULL ; ++i) {
        HMODULE hModule;
        LPVOID  pTarget;

        hModule = GetModuleHandleW(_hook_items[i].moduleName);
        if (hModule == NULL) continue;

        pTarget = GetProcAddress(hModule, _hook_items[i].procName);
        if (pTarget == NULL) continue;

        if (MH_DisableHook(pTarget) != MH_OK) {
            printf("MH_DisableHook(%s) failed\n", _hook_items[i].procName);
            continue;
        }
    }

    // Uninitialize MinHook.
    MH_Uninitialize();
}
