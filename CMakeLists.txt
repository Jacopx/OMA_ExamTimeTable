cmake_minimum_required(VERSION 3.8)
project(OMA_ExamTimeTable)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c fileManager.c fileManager.h edge.h edge.c ADTgraph.c ADTgraph.h utils.c utils.h ST.h ST.c TabuList.c TabuList.h ConflictStructure.c ConflictStructure.h rng.h local.c local.h annealing.c annealing.h verbose.h greedySlots.c greedySlots.h)
add_executable(OMA_ExamTimeTable ${SOURCE_FILES} fileManager.c fileManager.h edge.h edge.c ADTgraph.c ADTgraph.h utils.c utils.h ST.h ST.c TabuList.c TabuList.h ConflictStructure.c ConflictStructure.h rng.h local.c local.h annealing.c annealing.h greedySlots.c greedySlots.h)