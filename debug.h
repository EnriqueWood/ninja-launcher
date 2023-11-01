#ifndef NINJALAUNCHER_DEBUG_H
#define NINJALAUNCHER_DEBUG_H
#pragma once
#include <stdio.h>
#include <stdbool.h>

extern bool debug;

void debugPrint(const char *format, ...);
#endif //NINJALAUNCHER_DEBUG_H
