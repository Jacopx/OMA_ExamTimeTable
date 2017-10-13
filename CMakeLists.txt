cmake_minimum_required(VERSION 3.8)
project(OMA_ExamTimeTable)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c fileManager.c fileManager.h)
add_executable(OMA_ExamTimeTable ${SOURCE_FILES} fileManager.c fileManager.h)