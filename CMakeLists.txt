cmake_minimum_required(VERSION 3.26)
project(NinjaLauncher C)

set(CMAKE_C_STANDARD 11)

add_executable(NinjaLauncher BinariesLancher/launcher.c main.c
        BinariesLancher/launcher.h
        definitions.h
        Configuration/configurationManager.c
        Configuration/configurationManager.h
        debug.c
        debug.h)
