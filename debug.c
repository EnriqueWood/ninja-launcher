#include "debug.h"
#include <stdarg.h>

bool debug = false;

void debugPrint(const char *format, ...) {
    if (!debug) return;

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
