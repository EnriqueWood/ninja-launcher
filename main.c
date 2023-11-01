#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BinariesLauncher/launcher.h"
#include "Configuration/configurationManager.h"
#include "debug.h"

typedef struct {
    HWND hwnd;
    char windowTitle[256];
    char className[256];
    DWORD processId;
    char exePath[MAX_PATH];
    int width;
    int height;
} WindowInfo;

WindowInfo windowList[1024]; // Asumiendo que no habrá más de 1024 ventanas abiertas
int windowCount = 0;

Configuration *config;

void handleAppWindow(WindowInfo windowInfo);

char *windowInfoToString(WindowInfo windowInfo) {
    int size = snprintf(NULL, 0, "Handle: %-10p Title: %-50s ClassName: %-30s PID: %-10lu Path: %-70s Rect: %dx%d\n",
                        (void *) windowInfo.hwnd,
                        windowInfo.windowTitle,
                        windowInfo.className,
                        windowInfo.processId,
                        windowInfo.exePath,
                        windowInfo.width,
                        windowInfo.height);

    // Reservar memoria para el string (incluyendo el carácter nulo final)
    char *result = (char *) malloc(size + 1);

    // Comprobar si la memoria se reservó correctamente
    if (result != NULL) {
        // Almacenar la información en el string
        snprintf(result, size + 1, "Handle: %-10p Title: %-50s ClassName: %-30s PID: %-10lu Path: %-70s Rect: %dx%d\n",
                 (void *) windowInfo.hwnd,
                 windowInfo.windowTitle,
                 windowInfo.className,
                 windowInfo.processId,
                 windowInfo.exePath,
                 windowInfo.width,
                 windowInfo.height);
    }

    return result;
}

void PrintWindowsInfo(const WindowInfo *windowList, int windowCount) {
    if (windowList == NULL || windowCount <= 0) {
        debugPrint("No windows information available to display.\n");
        return;
    }

    debugPrint("Window Information:\n");
    debugPrint("===============================================================================\n");

    for (int i = 0; i < windowCount; ++i) {
        debugPrint("%s", windowInfoToString(windowList[i]));
    }

    debugPrint("===============================================================================\n");
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd) {
    // Obtener el ID del proceso asociado con la ventana
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    // Obtener un handle al proceso
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle != NULL) {
        // Obtener la ruta del binario de ejecución
        if (GetModuleFileNameEx(processHandle, NULL, windowList[windowCount].exePath,
                                sizeof(windowList[windowCount].exePath)) == 0) {
            // Si no se pudo obtener la ruta, almacenar una cadena vacía
            windowList[windowCount].exePath[0] = '\0';
        }
        CloseHandle(processHandle);
    } else {
        // Si no se pudo abrir el proceso, almacenar una cadena vacía
        windowList[windowCount].exePath[0] = '\0';
    }

    // Almacenar el ID del proceso
    windowList[windowCount].processId = processId;

    // Obtener el título de la ventana
    GetWindowText(hwnd, windowList[windowCount].windowTitle, sizeof(windowList[windowCount].windowTitle));

    // Obtener la clase de la ventana
    GetClassName(hwnd, windowList[windowCount].className, sizeof(windowList[windowCount].className));

    // Almacenar el handle de la ventana
    windowList[windowCount].hwnd = hwnd;

    // Obtener las dimensiones de la ventana
    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        windowList[windowCount].width = rect.right - rect.left;
        windowList[windowCount].height = rect.bottom - rect.top;
    }

    windowCount++;
    return TRUE; // Continuar enumerando
}


bool isInText(const char *fullText, const char *stringToSearch) {
    return strstr(fullText, stringToSearch) != NULL;
}

int shouldMinimize(WindowInfo windowInfo, const char *processName) {
    // Verificar si el className de windowInfo está en la lista de forbiddenClassNames
    for (int i = 0; config->forbiddenClassNames[i] != NULL && strlen(config->forbiddenClassNames[i]) > 0; i++) {
        if (strstr(windowInfo.className, config->forbiddenClassNames[i]) != NULL) {
            return 0;
        }
    }

    // Verificar si el proceso debe ser incluido
    return isInText(windowInfo.exePath, processName);
}

int main() {
    config = loadConfiguration();
    if (!config) {
        fprintf(stderr, "Failed to load configuration\n");
        return 1;
    }
    launchApps(config->executablesToLaunch);
    // Buscar todas las ventanas y almacenar información sobre ellas
    EnumWindows((WNDENUMPROC) EnumWindowsProc, 0);

   // PrintWindowsInfo(windowList, windowCount);

    DWORD startTime = GetTickCount();
    while (GetTickCount() - startTime < 60000) { // 60 segundos
        windowCount = 0;
        EnumWindows((WNDENUMPROC) EnumWindowsProc, 0);

        // Revisar si el nombre del ejecutable contiene "steamwebhelper.exe"
        for (int i = 0; i < windowCount; ++i) {
            handleAppWindow(windowList[i]);
        }
        Sleep(100); // Esperar 100 ms antes de verificar de nuevo para no saturar el CPU
    }

    freeConfigurationMemory(config);

    return 0;
}

void handleAppWindow(WindowInfo windowInfo) {
    debugPrint("Checking executable '%s'\n", windowInfo.exePath);
    for (int i = 0; config->executablesToHide[i] != NULL; i++) {
        if (shouldMinimize(windowInfo, config->executablesToHide[i])) {
            debugPrint("Executable '%s' should be minimized as it is not in %s\n", windowInfo.exePath,
                   config->executablesToHide[i]);
            SetWindowPos(windowInfo.hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            ShowWindow(windowInfo.hwnd, SW_MINIMIZE);
        } else {
            debugPrint("Executable '%s' should not be minimized\n", windowInfo.exePath);
        }
    }
}
