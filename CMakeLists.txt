cmake_minimum_required(VERSION 3.10)
project(so-i-24-chp2-FedericaMayorga01)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

enable_testing()

include_directories(include)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("-- Building Release mode with code coverage enabled")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators")

find_path(CJSON_INCLUDE_DIR NAMES cJSON.h PATHS /usr/include /usr/local/include)
find_library(CJSON_LIBRARY NAMES cjson PATHS /usr/lib /usr/local/lib)

if(NOT CJSON_INCLUDE_DIR OR NOT CJSON_LIBRARY)
  message(FATAL_ERROR "Could not find cJSON library and headers")
endif()

include_directories(${CJSON_INCLUDE_DIR})
target_link_libraries(tu_target ${CJSON_LIBRARY})

add_executable(myshell
    src/main.c
    src/commands.c
    src/executions.c
    src/monitor.c
    src/shell.c
)

add_library(shell_lib STATIC
    src/commands.c
    src/executions.c
    src/monitor.c
    src/shell.c
    include/commands.h
    include/executions.h
    include/monitor.h
    include/shell.h
)

target_link_libraries(myshell PRIVATE shell_lib cjson::cjson unity::unity)

add_subdirectory(tests)