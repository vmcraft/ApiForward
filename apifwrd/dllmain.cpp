#include "thriftlink_client.h"
#include "minhooklink.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "apifwrd.h"

bool IsHookProcess(DWORD pid);


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

        if (IsHookProcess(GetCurrentProcessId())) {
            if (!thrift_connect("127.0.0.1", 3900)) {
                printf("thrift_connect() failed.\n");
                return FALSE;
            }

            printf("thrift_connect() successful.\n");

            if (!start_hook()) {
                printf("start_hook() failed.\n");
                return FALSE;
            }

            printf("start_hook() successful.\n");
        }

        break;

	case DLL_THREAD_ATTACH:
        break;

	case DLL_THREAD_DETACH:
        break;

	case DLL_PROCESS_DETACH:
        if (IsHookProcess(GetCurrentProcessId())) {
            stop_hook();
            printf("stop_hook()\n");
            thrift_close();
            printf("thrift_close()\n");
        }
        printf("apifwrd.dll unloaded.\n");
#ifdef _DEBUG
        // Don't free to check messages to analyze after unloaded.
//		FreeConsole();
#endif
        break;
	}
	return TRUE;
}



