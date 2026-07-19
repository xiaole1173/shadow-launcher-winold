#include <windows.h>
#include <cstdio>
#include "include/cef_app.h"

int main() {
    // Test 1: Can we load CEF API at all?
    printf("CEF minimal test — loading...\n");
    
    CefMainArgs args(GetModuleHandle(nullptr));
    
    // Subprocess? Exit early.
    int code = CefExecuteProcess(args, nullptr, nullptr);
    if (code >= 0) {
        printf("CEF subprocess exit: %d\n", code);
        return code;
    }
    
    // Test 2: CefInitialize with minimal settings
    CefSettings sets;
    sets.size = sizeof(CefSettings);
    sets.no_sandbox = true;
    sets.multi_threaded_message_loop = false;
    sets.log_severity = LOGSEVERITY_DISABLE;  // no log to avoid path issues
    
    // Get our directory for resources
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    wchar_t* lastSlash = wcsrchr(path, L'\\');
    if (lastSlash) *lastSlash = 0;
    char asciiPath[MAX_PATH];
    WideCharToMultiByte(CP_ACP, 0, path, -1, asciiPath, MAX_PATH, nullptr, nullptr);
    
    CefString(&sets.resources_dir_path).FromASCII(asciiPath);
    CefString(&sets.browser_subprocess_path).FromASCII(asciiPath);  // won't be used without browser
    
    printf("Calling CefInitialize...\n");
    bool ok = CefInitialize(args, sets, nullptr, nullptr);
    printf("CefInitialize = %s\n", ok ? "TRUE" : "FALSE");
    
    if (ok) {
        printf("SUCCESS! CEF initialized.\n");
        CefShutdown();
    } else {
        printf("FAILED — CEF init returned false.\n");
    }
    
    return ok ? 0 : 1;
}
