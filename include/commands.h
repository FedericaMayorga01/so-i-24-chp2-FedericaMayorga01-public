#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Forward declaration for set_foreground_pid (from executions.h)
void set_foreground_pid(pid_t pid);

/**
 * @brief executes an external command
 * if the command is not found in the list of internal commands
 * it is interpreted as a program invocation. This will be
 * executed in a child process
 * @param program to execute
 */
void external_command(char* command);

/**
 * @brief Implementation of the cd command
 * Changes the current working directory
 * @param new directory
 * NULL = changes to the HOME directory
 * "-" = changes to the previous directory
 * path = changes to the "path" directory
 */
void command_cd(char* arg);

/**
 * @brief Implementation of the echo command
 * Prints a message to the console
 * @param message to print
 */
void command_echo(char* arg);

/**
 * @brief Implementation of the clr command
 * Clears the console
 */
void command_clear();

/**
 * @brief Implementation of the quit command
 * Exits the program
 */
void command_quit();

/**
 * @brief Structure to map commands with their implementations
 */
typedef struct
{
    /** @brief Nombre del comando */
    char* name;

    /** @brief Puntero a función que ejecuta el comando */
    void (*func)(char* arg);
} Command;

/**
 * @brief List of internal commands
 */
extern Command internals_commands[];

#endif // COMMANDS_H