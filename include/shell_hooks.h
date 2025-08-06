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
 *
 * @var ExtensionCommand::command
 *   The string representing the command name (maximum 63 characters plus null terminator).
 *
 * @var ExtensionCommand::handler
 *   Pointer to the function that handles the command. The function takes an array of
 *   argument strings (char **args) as input.
 *
 * @var ExtensionCommand::description
 *   A brief description of the command (maximum 255 characters plus null terminator).
 */
typedef struct {
    char command[64];
    void (*handler)(char **args);
    char description[256];
} ExtensionCommand;

/**
 * @brief Registers additional extension commands to the shell.
 *
 * This function should be called to add custom or extension commands
 * to the shell environment, enabling extended functionality.
 */
extern void register_extension_commands(void);

/**
 * @brief Handles the execution of extension commands in the shell.
 *
 * This function processes and executes custom extension commands provided to the shell.
 *
 * @param command The name of the extension command to execute.
 * @param args    An array of arguments to pass to the command. The array must be NULL-terminated.
 * @return An integer status code: 0 on success, or a negative value on error.
 */
extern int handle_extension_command(const char *command, char **args);

/**
 * @brief Displays help information for shell extensions.
 *
 * This function provides the user with guidance or instructions
 * related to available shell extensions or extension commands.
 */
extern void show_extension_help(void);

/**
 * @brief Pointer to the array of registered extension commands.
 *
 * This external variable provides access to the list of extension commands
 * available to the shell. Each element in the array is a pointer to an
 * ExtensionCommand structure, which defines the properties and behavior
 * of a specific shell extension command.
 */
extern ExtensionCommand* extension_commands;

/**
 * @brief Global variable that stores the number of extension commands available in the shell.
 */
extern int extension_command_count;

/**
 * @brief Weak function to register extension commands.
 *
 * This function can be overridden by the user to register custom extension commands.
 * It is marked as weak to allow for user-defined implementations.
 */
void __attribute__((weak)) register_extension_commands(void);

/**
 * @brief Weak function to handle extension commands.
 *
 * This function can be overridden by the user to handle custom extension commands.
 * It is marked as weak to allow for user-defined implementations.
 *
 * @param command The name of the command to handle.
 * @param args    An array of arguments for the command.
 * @return An integer status code: 0 on success, or a negative value on error.
 */
int __attribute__((weak)) handle_extension_command(const char *command, char **args);

/**
 * @brief Weak function to show help for shell extensions.
 *
 * This function can be overridden by the user to provide custom help information
 * for shell extensions. It is marked as weak to allow for user-defined implementations.
 */
void __attribute__((weak)) show_extension_help(void);

#endif