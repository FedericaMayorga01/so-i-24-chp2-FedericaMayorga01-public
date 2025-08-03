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

set(CJSON_INCLUDE_DIR "/usr/include/cjson")
set(CJSON_LIBRARY "/usr/lib/x86_64-linux-gnu/libcjson.so")

if(NOT EXISTS "${CJSON_INCLUDE_DIR}/cJSON.h")
  message(FATAL_ERROR "No se encontró el header cJSON.h en ${CJSON_INCLUDE_DIR}")
endif()

if(NOT EXISTS "${CJSON_LIBRARY}")
  message(FATAL_ERROR "No se encontró la librería cJSON en ${CJSON_LIBRARY}")
endif()

include_directories(${CJSON_INCLUDE_DIR})

add_library(survShell_lib STATIC
    src/commands.c
    src/executions.c
    src/monitor.c
    src/shell.c
    include/commands.h
    include/executions.h
    include/monitor.h
    include/shell.h
    include/colors.h
)

add_executable(survivorShell
    src/main.c
    src/commands.c
    src/executions.c
    src/monitor.c
    src/shell.c
)

add_subdirectory(tests)

target_link_libraries(survivorShell PRIVATE survShell_lib ${CJSON_LIBRARY})

add_library(unity STATIC lib/unity/unity.c)
target_include_directories(unity PUBLIC lib/unity)

add_executable(unit_test_shell tests/test_commands.c)
target_link_libraries(unit_test_shell unity survShell_lib)
add_test(NAME unit_test_shell COMMAND unit_test_shell)