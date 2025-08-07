#ifndef SHELL_HOOKS_H
#define SHELL_HOOKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure representing a shell extension command.
 *
 * This structure defines a command that can be added to the shell as an extension.
 * It contains the command string, a pointer to the handler function that implements
 * the command's behavior, and a description of the command.
 */
typedef struct
{
    char command[64];             /**< Command name string (max 63 chars + null terminator) */
    void (*handler)(char** args); /**< Function pointer to command handler */
    char description[256];        /**< Command description string (max 255 chars + null terminator) */
} ExtensionCommand;

/**
 * @brief Pointer to the array of registered extension commands.
 *
 * This external variable provides access to the list of extension commands
 * available to the shell.
 */
extern ExtensionCommand* extension_commands;

/**
 * @brief Number of registered extension commands.
 *
 * This variable tracks the count of currently registered extension commands
 * for iteration and bounds checking purposes.
 */
extern int extension_command_count;

/**
 * @brief Registers additional extension commands to the shell.
 *
 * This function should be called to add custom or extension commands
 * to the shell environment, enabling extended functionality.
 */
void register_extension_commands(void);

/**
 * @brief Handles the execution of extension commands in the shell.
 *
 * This function processes and executes custom extension commands provided to the shell.
 *
 * @param command The name of the extension command to execute.
 * @param args    An array of arguments to pass to the command. The array must be NULL-terminated.
 * @return An integer status code indicating the result of the command execution.
 * @retval 0 Command not found or not handled
 * @retval 1 Command found and executed successfully
 */
int handle_extension_command(const char* command, char** args);

/**
 * @brief Displays help information for shell extensions.
 *
 * This function provides the user with guidance or instructions
 * related to available shell extensions or extension commands.
 */
void show_extension_help(void);

#endif // SHELL_HOOKS_H