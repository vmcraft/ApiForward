#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>


#pragma comment(lib, "user32.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
        if (AllocConsole()){
            freopen("CONOUT$", "w", stdout);
        }
#endif
        printf("apifwrd.dll loaded.\n");
        break;

	case DLL_THREAD_ATTACH:
        break;

	case DLL_THREAD_DETACH:
        break;

	case DLL_PROCESS_DETACH:
        printf("apifwrd.dll unloaded.\n");
#ifdef _DEBUG
		FreeConsole();
#endif
        break;
	}
	return TRUE;
}



int APIENTRY CALLBACK HelperHookProcForSetWindowsHookEx(int code, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(NULL, code, wParam, lParam);
}
