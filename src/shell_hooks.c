#include "../include/shell_hooks.h"

/* Global variables for extensions */
ExtensionCommand* extension_commands = NULL;
int extension_command_count = 0;

/* Default (weak) implementations, can be overridden */
void __attribute__((weak)) register_extension_commands(void)
{
    /* By default, no extension commands */
    extension_commands = NULL;
    extension_command_count = 0;
}

int __attribute__((weak)) handle_extension_command(const char* command, char** args)
{
    /* By default, does not handle any extension command */
    return 0; // Command not found
}

void __attribute__((weak)) show_extension_help(void)
{
    /* By default, does not show extension help */
}