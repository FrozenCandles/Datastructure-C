cmake_minimum_required(VERSION 3.20)
project(DataStructure_C C)

set(CMAKE_C_STANDARD 11)

file(GLOB
        "Src/*.*"
        "Utils/*.*"
)

aux_source_directory(Src SOURCES)

include_directories(
        "Inc"
        "Utils"
)


add_executable(DataStructure_C ${SOURCES})
