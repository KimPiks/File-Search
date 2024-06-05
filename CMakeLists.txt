cmake_minimum_required(VERSION 3.22)
project(File_Search C)

set(CMAKE_C_STANDARD 17)

add_executable(File_Search src/main.c
        src/Options.h
        src/Options.c
        src/Directory.h
        src/Directory.c)
