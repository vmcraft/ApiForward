#include <Windows.h>
#include <psapi.h>
#include "apifwrd.h"


#pragma comment(lib, "user32.lib")
#pragma comment(lib, "psapi.lib")

#pragma data_seg("SHARED")
#pragma data_seg()

#pragma comment(linker, "/section:SHARED,RWS")

extern char *_without_hook[];


bool GetProcessNameByProcessId( DWORD processID, char* szProcessName, DWORD ProcessNameSize );



int APIENTRY HelperHookProcForSetWindowsHookEx(int code, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(NULL, code, wParam, lParam);
}


bool IsHookProcess(DWORD pid)
{
    char processName[MAX_PATH];
    
    if (!GetProcessNameByProcessId(pid, processName, sizeof(processName))) {
        return false;
    }

    for(int i=0 ; _without_hook[i]!=NULL; ++i) {
        if (_strnicmp(_without_hook[i], processName, sizeof(processName))!=0) {
            return true;
        }
    }

    return false;
}


bool GetProcessNameByProcessId( DWORD processID, char* szProcessName, DWORD ProcessNameSize )
{
    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    if (hProcess == NULL) return false;


    // Get the process name.

    HMODULE hMod;
    DWORD cbNeeded;

    if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
            &cbNeeded) )
    {
        if (GetModuleBaseNameA( hProcess, hMod, szProcessName, ProcessNameSize)==0){
            return false;
        }
    }

    // Release the handle to the process.
    CloseHandle( hProcess );

    return true;
}
