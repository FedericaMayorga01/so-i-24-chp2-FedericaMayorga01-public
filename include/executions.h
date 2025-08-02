#ifndef EXECUTIONS_H
#define EXECUTIONS_H

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "commands.h"

/**
 * @brief Function that executes a command in the foreground
 * Separates the arguments and defines whether the command is internal or external
 */
void execute_command(char* command);

/**
 * @brief Function that executes a command in the background
 * When an & is detected, the command is executed in the background in a child process.
 * Its output is redirected to a temporary file, and the parent prints the job ID and
 * process ID while it is running. Once the execution is finished, the file is printed.
 */
void execute_command_secondplane(char* command);

/**
 * @brief Function that executes chained commands
 * Separates the commands at each | and creates a pipe for each pair of commands
 * Redirects the output of the first command to the pipe and the input of the second command to the pipe
 */
void execute_piped_commands(char* command);

/**
 * @brief Function that executes a command with IO redirection
 * Redirects stdin from a file to a command and from stdout to a file
 * Works for internal and external commands
 */
void execute_command_redirection(char* command);

/**
 * @brief Function that restores standard input and output
 * Restores standard input and output after redirecting them
 * @param original_stdin, original_stdout, original_stderr are the original files
 * input_file, output_file are the ones they were redirected to for that execution
 */

void restore_io(int original_stdin, int original_stdout, int original_stderr);

/**
 * @brief Function that handles signals
 * If an interrupt signal is received, the signal is sent to the foreground process
 * @param signo The number of the signal being handled.
 */
void signal_handler(int signo);

/**
 * @brief Function that assigns the handler to each signal
 */
void setup_signals(void);

/**
 * @brief Function that assigns the pid of the foreground process
 * @param pid The pid of the foreground process
 */
void set_foreground_pid(pid_t pid);

#endif // EXECUTIONS_H