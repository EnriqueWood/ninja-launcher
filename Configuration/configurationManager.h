#ifndef NINJALAUNCHER_CONFIGURATIONLOADER_H

typedef struct {
    char **executablesToLaunch;
    char **executablesToHide;
    char **doNotMinimizeTheseClassNames;
} Configuration;

Configuration *loadConfiguration();

void freeConfigurationMemory(Configuration *configuration);

#define NINJALAUNCHER_CONFIGURATIONLOADER_H

#endif //NINJALAUNCHER_CONFIGURATIONLOADER_H