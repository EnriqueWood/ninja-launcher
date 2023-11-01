#ifndef NINJALAUNCHER_CONFIGURATIONLOADER_H

typedef struct {
    char** executablesToLaunch;
    char** executablesToHide;
    char** forbiddenClassNames;
} Configuration;

Configuration* loadConfiguration();
void freeConfigurationMemory(Configuration *configuration);

#define NINJALAUNCHER_CONFIGURATIONLOADER_H

#endif //NINJALAUNCHER_CONFIGURATIONLOADER_H