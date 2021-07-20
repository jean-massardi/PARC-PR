cmake_minimum_required(VERSION 3.5.1)
project(PARC)

include_directories("./src/solver")
include_directories("./Plan_Library")
include_directories("./include")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE solver_HEADERS "./src/solver/*.h")
file(GLOB_RECURSE solver_SOURCES "./src/solver/*.cpp")
FILE(GLOB_RECURSE PLHEADERS  "./Plan_Library/*.h")
FILE(GLOB_RECURSE PLSOURCES  "./Plan_Library/*.cpp")

list(APPEND HEADERS ${solver_HEADERS} ${PLHEADERS})
list(APPEND SOURCES ${solver_SOURCES} ${PLSOURCES})

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${HEADERS} ${SOURCES})


