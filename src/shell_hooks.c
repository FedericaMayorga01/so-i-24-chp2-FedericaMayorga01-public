#include "../include/shell_hooks.h"

/**
 * @brief Global pointer to array of extension commands
 *
 * This variable holds a pointer to an array of ExtensionCommand structures
 * that define the available extension commands. It is initialized to NULL
 * by default and should be set by the register_extension_commands() function.
 */
ExtensionCommand* extension_commands = NULL;

/**
 * @brief Global count of registered extension commands
 *
 * This variable tracks the number of extension commands currently registered
 * in the extension_commands array. It is used for bounds checking when
 * iterating through available commands.
 */
int extension_command_count = 0;

/**
 * @brief Default implementation for registering extension commands (weak symbol)
 *
 * This is the default implementation that does nothing. It can be overridden
 * by providing a strong implementation in another compilation unit.
 * The weak attribute allows this function to be replaced without causing
 * linking conflicts.
 *
 * By default, this function sets extension_commands to NULL and
 * extension_command_count to 0, indicating no extensions are available.
 */
void __attribute__((weak)) register_extension_commands(void)
{
    /* By default, no extension commands */
    extension_commands = NULL;
    extension_command_count = 0;
}

/**
 * @brief Default implementation for handling extension commands (weak symbol)
 *
 * This is the default implementation that handles no extension commands.
 * It can be overridden by providing a strong implementation in another
 * compilation unit.
 *
 * @param command The name of the command to handle
 * @param args Array of command arguments (NULL-terminated)
 * @return 0 indicating the command was not found/handled
 *
 * @retval 0 Command not found or not handled
 * @retval 1 Command found and executed successfully (in override implementations)
 */
int __attribute__((weak)) handle_extension_command(const char* command, char** args)
{
    /* By default, does not handle any extension command */
    return 0; // Command not found
}

/**
 * @brief Default implementation for showing extension help (weak symbol)
 *
 * This is the default implementation that shows no help information.
 * It can be overridden by providing a strong implementation in another
 * compilation unit that displays help for registered extension commands.
 *
 * The default implementation does nothing, as there are no extensions
 * to document by default.
 */
void __attribute__((weak)) show_extension_help(void)
{
    /* By default, does not show extension help */
}