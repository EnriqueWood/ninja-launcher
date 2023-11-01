#include <stdio.h>
#include <windows.h>
#include "../debug.h"

int launchApps(char **paths) {
    for (int i = 0; paths[i] != NULL; ++i) {
        debugPrint("Creating process %s...\n", paths[i]);
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        // Inicializa las estructuras de STARTUPINFO y PROCESS_INFORMATION.
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcess(NULL,   // Nombre del módulo
                           paths[i],        // Línea de comandos
                           NULL,           // Atributos de seguridad de proceso
                           NULL,           // Atributos de seguridad de hilo
                           FALSE,          // Heredar manijas
                           0,              // Flags de creación
                           NULL,           // Usar entorno del padre
                           NULL,           // Usar directorio del padre
                           &si,            // Puntero a STARTUPINFO
                           &pi)            // Puntero a PROCESS_INFORMATION
                ) {
            debugPrint("CreateProcess failed (%lu): executable path was: '%s'.\n", GetLastError(), paths[i]);
        } else {
            debugPrint("Process '%s' created successfully (PID: %lu).\n", paths[i], pi.dwProcessId);
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return 0;
}
