#include "thriftlink_client.h"
#include "minhooklink.h"

// We will not hook the following processes forcely.
char *_without_hook[] = {
                    "sample.exe",
                    NULL
};

bool _dont_hook_if_connection_failed = true;


// Define original function pointer
int (WINAPI *fpShellAboutW)(HWND, LPCWSTR, LPCWSTR, HICON) = NULL;

// Define detour function
int WINAPI DetourShellAboutW(HWND hWnd, LPCWSTR szApp, LPCWSTR szOtherStuff, HICON hIcon)
{
    printf("DetourShellAboutW()\n");
    if (!ensure_connection()) return 0;
    
    if (szApp==NULL) {
        return _api->ShellAboutW(std::string(""));
    }
    else {
        return _api->ShellAboutW(std::string((char*)szApp, wcslen(szApp)*sizeof(WCHAR)+sizeof(WCHAR)));
    }
}

// Define hook items
HOOK_ITEM _hook_items[] = {
    {L"shell32", "ShellAboutW", (LPVOID)&DetourShellAboutW, (LPVOID*)&fpShellAboutW},
    {NULL, NULL, NULL, NULL}
};


