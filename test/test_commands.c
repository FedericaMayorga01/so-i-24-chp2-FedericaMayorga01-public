#include "../include/commands.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Mock functions to avoid dependencies during testing
void set_foreground_pid(pid_t pid)
{
    // Mock implementation
    (void)pid;
}

void setUp(void)
{
    // Setup before each test
}

void tearDown(void)
{
    // Cleanup after each test
}

void test_command_echo_simple_string(void)
{
    // Redirect stdout to capture output
    FILE* original_stdout = stdout;
    char buffer[256];
    FILE* test_stdout = fmemopen(buffer, sizeof(buffer), "w");
    stdout = test_stdout;

    command_echo("Hello World");

    fclose(test_stdout);
    stdout = original_stdout;

    TEST_ASSERT_TRUE(strstr(buffer, "Hello World") != NULL);
}

void test_command_echo_null_argument(void)
{
    // Redirect stdout to capture output
    FILE* original_stdout = stdout;
    char buffer[256];
    FILE* test_stdout = fmemopen(buffer, sizeof(buffer), "w");
    stdout = test_stdout;

    command_echo(NULL);

    fclose(test_stdout);
    stdout = original_stdout;

    // Should just print a newline
    TEST_ASSERT_EQUAL_CHAR('\n', buffer[0]);
}

void test_command_echo_environment_variable(void)
{
    // Set a test environment variable
    setenv("TEST_VAR", "test_value", 1);

    // Redirect stdout to capture output
    FILE* original_stdout = stdout;
    char buffer[256];
    FILE* test_stdout = fmemopen(buffer, sizeof(buffer), "w");
    stdout = test_stdout;

    command_echo("$TEST_VAR");

    fclose(test_stdout);
    stdout = original_stdout;

    TEST_ASSERT_TRUE(strstr(buffer, "test_value") != NULL);

    // Clean up
    unsetenv("TEST_VAR");
}

void test_command_cd_to_home(void)
{
    char original_dir[1024];
    getcwd(original_dir, sizeof(original_dir));

    // Test cd to home
    command_cd(NULL);

    char current_dir[1024];
    getcwd(current_dir, sizeof(current_dir));

    char* home = getenv("HOME");
    if (home != NULL)
    {
        TEST_ASSERT_EQUAL_STRING(home, current_dir);
    }

    // Restore original directory
    chdir(original_dir);
}

void test_command_cd_with_path(void)
{
    char original_dir[1024];
    getcwd(original_dir, sizeof(original_dir));

    // Try to change to /tmp (should exist on most systems)
    command_cd("/tmp");

    char current_dir[1024];
    getcwd(current_dir, sizeof(current_dir));

    TEST_ASSERT_EQUAL_STRING("/tmp", current_dir);

    // Restore original directory
    chdir(original_dir);
}

void test_internals_commands_array_not_empty(void)
{
    // Test that we have some internal commands defined
    TEST_ASSERT_NOT_NULL(internals_commands[0].name);
    TEST_ASSERT_NOT_NULL(internals_commands[0].func);
}

void test_find_internal_command(void)
{
    // Test that we can find the echo command
    int found = 0;
    for (int i = 0; i < 7; i++)
    { // We know there are 7 internal commands
        if (strcmp(internals_commands[i].name, "echo") == 0)
        {
            found = 1;
            TEST_ASSERT_EQUAL_PTR(command_echo, internals_commands[i].func);
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_command_echo_simple_string);
    RUN_TEST(test_command_echo_null_argument);
    RUN_TEST(test_command_echo_environment_variable);
    RUN_TEST(test_command_cd_to_home);
    RUN_TEST(test_command_cd_with_path);
    RUN_TEST(test_internals_commands_array_not_empty);
    RUN_TEST(test_find_internal_command);

    return UNITY_END();
}