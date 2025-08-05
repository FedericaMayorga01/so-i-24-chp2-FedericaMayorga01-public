#ifndef SHELL_H
#define SHELL_H

#include "../include/colors.h"
#include "../include/commands.h"
#include "../include/executions.h"
#include "../include/monitor.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Initializes the shell and handles input.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return int Program exit code.
 */
int init_shell(int argc, char* argv[]);

/**
 * @brief Prints the prompt for each new command line.
 */
void prompt(void);

/**
 * @brief Chooses the type of execution based on the command syntax.
 *
 * @param command The command to execute.
 */
void choose_execution(char* command);

#endif // SHELL_H