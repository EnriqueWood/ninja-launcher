#include "debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#ifdef _WIN32

#include <windows.h>
#include <shlobj.h>

#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

bool debug = false;
bool logToFile = false;
char logFilePath[MAX_PATH] = {0};

void getDateTimeString(char *buffer, size_t bufferSize, const char *format) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(buffer, bufferSize, format, timeinfo);
}

void initializeLogFilePath() {
    if (logFilePath[0] == '\0') {
        char dateTime[20];
        getDateTimeString(dateTime, sizeof(dateTime), "%Y-%m-%d_%H-%M-%S");

        char logFolderPath[MAX_PATH];
#ifdef _WIN32
        SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, logFolderPath);
#else
        struct passwd *pw = getpwuid(getuid());
        const char *homedir = pw->pw_dir;
        sprintf(logFolderPath, "%s/Documents", homedir);
#endif

        sprintf(logFilePath, "%s/NinjaLauncher/log", logFolderPath);
        mkdir(logFilePath);

        sprintf(logFilePath, "%s/debug_%s.log", logFilePath, dateTime);
    }
}

void debugPrint(const char *format, ...) {
    if (!debug && !logToFile) return;
    initializeLogFilePath();

    va_list args;
    va_start(args, format);

    char dateTime[20];
    getDateTimeString(dateTime, sizeof(dateTime), "%H:%M:%S");

    if (logToFile) {
        FILE *logFile = fopen(logFilePath, "a");
        if (logFile != NULL) {
            char *logLine = malloc((strlen(dateTime) + strlen(format)) * sizeof(char *));

            sprintf(logLine, "%s - %s", dateTime, format);
            vfprintf(logFile, logLine, args);
            fclose(logFile);
        }
    } else if (debug) {
        printf("%s", dateTime);
        vprintf(format, args);
    }
    va_end(args);
}
