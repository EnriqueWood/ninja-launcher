#include <windows.h>
#include "../debug.h"

int launchApps(char **paths) {
    char binaryPath[MAX_PATH];
    char workDir[MAX_PATH];

    for (int i = 0; paths[i] != NULL; ++i) {
        debugPrint("Launching process %s...\n", paths[i]);
        char *pipePos = strstr(paths[i], "|");

        if (pipePos != NULL) {
            strncpy(binaryPath, paths[i], pipePos - paths[i]);
            binaryPath[pipePos - paths[i]] = '\0';
            strcpy(workDir, pipePos + 1);
            debugPrint("Work directory for the binary '%s' will be '%s'...\n", binaryPath, workDir);
            SetCurrentDirectory(TEXT(workDir));
        } else {
            strcpy(binaryPath, paths[i]);
        }

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