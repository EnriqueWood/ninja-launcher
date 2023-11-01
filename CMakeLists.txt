cmake_minimum_required(VERSION 3.26)
project(NinjaLauncher C)

set(CMAKE_C_STANDARD 11)

add_executable(NinjaLauncher BinariesLauncher/launcher.c main.c
        BinariesLauncher/launcher.h
        Configuration/configurationManager.c
        Configuration/configurationManager.h
        debug.c
        debug.h)
