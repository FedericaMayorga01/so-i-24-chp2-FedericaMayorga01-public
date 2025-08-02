cmake_minimum_required(VERSION 3.10)
project(so-i-24-chp2-FedericaMayorga01)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add include directories
include_directories(/usr/local/include)
include_directories(include)
include_directories(tests)

# Add library directories
link_directories(/usr/local/lib)

# Compiler flags for debugging and coverage
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_C_FLAGS_COVERAGE "-g -O0 --coverage -fprofile-arcs -ftest-coverage")

# Option to build tests
option(BUILD_TESTS "Build the tests" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

# Add coverage flags if enabled
if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# Add your source files
file(GLOB SOURCES "src/*.c")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against cJSON and other required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    cjson
    pthread
    m
)

# Download Unity if building tests
if(BUILD_TESTS)
    # Check if Unity is available
    find_path(UNITY_INCLUDE_DIR unity.h PATHS /usr/local/include /usr/include)
    find_library(UNITY_LIBRARY unity PATHS /usr/local/lib /usr/lib)
    
    if(NOT UNITY_INCLUDE_DIR OR NOT UNITY_LIBRARY)
        # Download Unity from GitHub
        include(FetchContent)
        FetchContent_Declare(
            unity
            GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
            GIT_TAG v2.5.2
        )
        FetchContent_MakeAvailable(unity)
        
        # Build Unity as static library
        add_library(unity_lib STATIC
            ${unity_SOURCE_DIR}/src/unity.c
        )
        target_include_directories(unity_lib PUBLIC ${unity_SOURCE_DIR}/src)
        
        set(UNITY_INCLUDE_DIR ${unity_SOURCE_DIR}/src)
        set(UNITY_LIBRARY unity_lib)
    endif()
    
    # Include Unity headers
    include_directories(${UNITY_INCLUDE_DIR})
    
    # Test executables
    set(TEST_SOURCES
        tests/test_commands.c
        #tests/test_shell.c
        src/monitor.c
    )
    
    # test_commands necesita monitor.c
    add_executable(test_commands
        tests/test_commands.c
        src/monitor.c
    )
    target_link_libraries(test_commands PRIVATE
        ${UNITY_LIBRARY}
        cjson
        pthread
        m
    )
    target_include_directories(test_commands PRIVATE ${UNITY_INCLUDE_DIR})
    add_test(NAME test_commands COMMAND test_commands)

#    # test_shell no depende de monitor.c
#    add_executable(test_shell
#        tests/test_shell.c
#    )
#    target_link_libraries(test_shell PRIVATE
#        ${UNITY_LIBRARY}
#        cjson
#        pthread
#        m
#    )
#    target_include_directories(test_shell PRIVATE ${UNITY_INCLUDE_DIR})
#    add_test(NAME test_shell COMMAND test_shell)
    
    # Enable testing
    enable_testing()
    
    # Custom target to run all tests
    add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --verbose
        DEPENDS ${TEST_SOURCES}
        COMMENT "Running all tests"
    )
    
    # Coverage target
    if(ENABLE_COVERAGE)
        find_program(GCOV_PATH gcov)
        find_program(LCOV_PATH lcov)
        find_program(GENHTML_PATH genhtml)
        
        if(GCOV_PATH AND LCOV_PATH AND GENHTML_PATH)
            # Setup coverage target
            add_custom_target(coverage
                COMMAND ${LCOV_PATH} --directory . --zerocounters
                COMMAND ${CMAKE_CTEST_COMMAND} --verbose
                COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info
                COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' '*/tests/*' '*/unity/*' --ignore-errors unused --output-file coverage.info.cleaned
                COMMAND ${GENHTML_PATH} -o coverage coverage.info.cleaned
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                DEPENDS run_tests
                COMMENT "Generating code coverage report"
            )
            
            # Clean coverage target
            add_custom_target(coverage_clean
                COMMAND ${LCOV_PATH} --directory . --zerocounters
                COMMAND find . -name "*.gcda" -delete
                COMMAND find . -name "*.gcno" -delete
                COMMAND rm -rf coverage coverage.info coverage.info.cleaned
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Cleaning coverage data"
            )
        else()
            message(WARNING "Coverage tools not found. Install lcov and gcov for coverage reporting.")
        endif()
    endif()
endif()

# Install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES config.json DESTINATION etc)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C flags: ${CMAKE_C_FLAGS}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Enable coverage: ${ENABLE_COVERAGE}")

if(BUILD_TESTS)
    message(STATUS "Unity include dir: ${UNITY_INCLUDE_DIR}")
    message(STATUS "Unity library: ${UNITY_LIBRARY}")
endif()

message(STATUS "Linking with cjson, pthread, and math libraries")