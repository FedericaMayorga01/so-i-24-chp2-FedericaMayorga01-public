cmake_minimum_required(VERSION 3.10)
project(so-i-24-chp2-FedericaMayorga01)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

enable_testing()

include_directories(include)
include_directories(lab1/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators")

set(CJSON_INCLUDE_DIR "/usr/include/cjson")
set(CJSON_LIBRARY "/usr/lib/x86_64-linux-gnu/libcjson.so")

if(NOT EXISTS "${CJSON_INCLUDE_DIR}/cJSON.h")
  message(FATAL_ERROR "Could not find the cJSON.h header in ${CJSON_INCLUDE_DIR}")
endif()

if(NOT EXISTS "${CJSON_LIBRARY}")
  message(FATAL_ERROR "Could not find the cJSON library in ${CJSON_LIBRARY}")
endif()

include_directories(${CJSON_INCLUDE_DIR})

set(LAB1_SOURCES
    lab1/src/metrics.c
)

add_library(survShell_lib STATIC
    src/commands.c
    src/executions.c
    src/monitor.c
    src/shell.c
    src/shell_hooks.c
    include/commands.h
    include/executions.h
    include/monitor.h
    include/shell.h
    include/shell_hooks.h
    include/colors.h
    ${LAB1_SOURCES} 
)

add_executable(survivorShell
    src/main.c
)

target_link_libraries(survivorShell PRIVATE survShell_lib ${CJSON_LIBRARY})

add_library(unity STATIC lib/unity/unity.c)
target_include_directories(unity PUBLIC lib/unity)

add_executable(unit_test_shell tests/test_commands.c)
target_link_libraries(unit_test_shell unity survShell_lib ${CJSON_LIBRARY})
add_test(NAME unit_test_shell COMMAND unit_test_shell)
