#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <shlobj.h>
#include <direct.h>
#include "../Configuration/configurationManager.h"
#include "../debug.h"

#define CONFIG_FILE "NinjaLauncher\\config.txt"
#define CONFIG_EXAMPLE "\
[ExecutablesToLaunch]\n\
C:\\Program Files (x86)\\Steam\\Steam.exe\n\
notepad\n\
\n\
[ExecutablesToHide]\n\
steamwebhelper.exe\n\
\n\
[ForbiddenClassNames]\n\
Chrome_WidgetWin_0\n\
Chrome_SystemMessageWindow\n\
crashpad_SessionEndWatcher\n\
IME\n\
Base_PowerMessageWindow\n"


Configuration *createConfiguration() {
    Configuration *config = calloc(1, sizeof(Configuration));
    if (config == NULL) {
        perror("Error allocating memory for config");
        return NULL;
    }
    config->executablesToLaunch = malloc(500 * sizeof(char*));
    config->executablesToHide = malloc(500 * sizeof(char*));
    config->forbiddenClassNames = malloc(500 * sizeof(char*));
    return config;
}

void printConfiguration(const Configuration *config) {
   debugPrint("Executables to Launch:\n");
    for (int i = 0; config->executablesToLaunch[i] != NULL; ++i) {
       debugPrint("%s\n", config->executablesToLaunch[i]);
    }

   debugPrint("\nExecutables to Hide:\n");
    for (int i = 0; config->executablesToHide[i] != NULL; ++i) {
       debugPrint("%s\n", config->executablesToHide[i]);
    }

   debugPrint("\nForbidden Class Names:\n");
    for (int i = 0; config->forbiddenClassNames[i] != NULL; ++i) {
       debugPrint("%s\n", config->forbiddenClassNames[i]);
    }
}

char *getDocumentsPath() {
    static char docPath[MAX_PATH];
    if (SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, docPath) != S_OK) {
        fprintf(stderr, "Error getting documents folder path\n");
        return NULL;
    }
    return docPath;
}

void createExampleConfigFile(const char *configPath) {
    char dirPath[MAX_PATH];
    strcpy(dirPath, configPath);
    char *lastSlash = strrchr(dirPath, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0';
        _mkdir(dirPath);
    }

    FILE *file = fopen(configPath, "w");
    if (file == NULL) {
        perror("Error creating example configuration file");
        return;
    }
    fprintf(file, CONFIG_EXAMPLE);
    fclose(file);
}

void freeConfigurationMemory(Configuration *configuration) {
    if(configuration != NULL) {
        free(configuration->executablesToLaunch);
        free(configuration->executablesToHide);
        free(configuration->forbiddenClassNames);
        free(configuration);
    }
}

Configuration *loadConfiguration() {
   debugPrint("Init reading config file\n");
    Configuration *config = createConfiguration();
    if (config == NULL) return NULL;

    char configPath[MAX_PATH];
    snprintf(configPath, MAX_PATH, "%s\\%s", getDocumentsPath(), CONFIG_FILE);

    if (access(configPath, F_OK) != -1) {
       debugPrint("The file exists!\n");
    } else {
       debugPrint("The file does not exist, a template will be created!\n");
        createExampleConfigFile(configPath);
    }

    FILE *file = fopen(configPath, "r");
    if (file == NULL) {
        perror("Error opening file");
        freeConfigurationMemory(config);
        return NULL;
    }

    char line[MAX_PATH];
    int section = 0;
    int executablesToLaunchCount = 0;
    int executablesToHideCount = 0;
    int forbiddenClassNamesCount = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

       debugPrint("Reading line %s\n", line);

        if (strncmp(line, "[ExecutablesToLaunch]", 21) == 0) {
            section = 1;
           debugPrint("Now in section %d\n", section);
        } else if (strncmp(line, "[ExecutablesToHide]", 19) == 0) {
            section = 2;
           debugPrint("Now in section %d\n", section);
        } else if (strncmp(line, "[ForbiddenClassNames]", 21) == 0) {
            section = 3;
           debugPrint("Now in section %d\n", section);
        } else if (line[0] != '[' && line[0] != '\0') {
            if (section == 1) {
                config->executablesToLaunch[executablesToLaunchCount] = malloc(len * sizeof(char));
                strcpy(config->executablesToLaunch[executablesToLaunchCount], line);
                executablesToLaunchCount++;
               debugPrint("Line %s inserted in section %d\n", line, section);
            } else if (section == 2) {
                config->executablesToHide[executablesToHideCount] = malloc(len * sizeof(char));
                strcpy(config->executablesToHide[executablesToHideCount], line);
                executablesToHideCount++;
               debugPrint("Line %s inserted in section %d\n", line, section);
            } else if (section == 3) {
                config->forbiddenClassNames[forbiddenClassNamesCount] = malloc(len * sizeof(char));
                strcpy(config->forbiddenClassNames[forbiddenClassNamesCount], line);
                forbiddenClassNamesCount++;
               debugPrint("Line %s inserted in section %d\n", line, section);
            }
        }
    }
    config->executablesToLaunch[executablesToLaunchCount] = NULL;
    config->executablesToHide[executablesToHideCount] = NULL;
    config->forbiddenClassNames[forbiddenClassNamesCount] = NULL;
    fclose(file);

    printConfiguration(config);
   debugPrint("Finished reading config file\n");
    return config;
}