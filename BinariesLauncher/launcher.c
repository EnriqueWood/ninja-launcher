#include <windows.h>
#include "../debug.h"

int launchApps(char **paths) {
    for (int i = 0; paths[i] != NULL; ++i) {
        debugPrint("Launching process %s...\n", paths[i]);

        HINSTANCE hResult = ShellExecute(
                NULL,                   // No parent window
                "open",            // Operation to perform
                paths[i],               // Application path
                NULL,             // No parameters
                NULL,              // Default directory
                SW_SHOWMINNOACTIVE // Start minimized without activating the window
        );

        if ((INT_PTR) hResult <= 32) {
            debugPrint("ShellExecute failed (%ld): executable path was: '%s'.\n", (LONG_PTR) hResult, paths[i]);
        } else {
            debugPrint("Process '%s' launched successfully.\n", paths[i]);
        }
    }

    return 0;
}