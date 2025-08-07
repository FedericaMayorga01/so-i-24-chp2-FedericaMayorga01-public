#include "../include/executions.h"
#include "../include/colors.h"
#include "../include/shell_hooks.h"

// Global variable for the job id
static int job_id = 1;
static int size_internal_commands = 7;
pid_t foreground_pid = 0;

// Function declarations
void execute_command(char* command);
void execute_command_secondplane(char* command);
void execute_piped_commands(char* command);

/**
 * @brief Executes a command in the foreground.
 *
 * Separates the arguments and determines if the command is internal or external.
 *
 * @param command The full command string.
 */
void execute_command(char* command)
{
    char* command_name = strtok(command, " ");
    if (command_name == NULL)
    {
        return; // Empty command
    }

    char* arg = strtok(NULL, " ");

    // Check if the command is an internal command
    for (int i = 0; i < size_internal_commands; i++)
    {
        if (strcmp(command_name, internals_commands[i].name) == 0)
        {
            internals_commands[i].func(arg);
            return;
        }
    }

    external_command(command);

    printf("Command not found: %s\n", command_name);
    return;
}

/**
 * @brief Executes a command in the background.
 *
 * Detects an '&' and runs the command in a child process. Its output is
 * redirected to a temporary file, and the parent prints the job ID and PID.
 * After the child finishes, the parent prints the contents of the file.
 *
 * @param command The command string to be executed.
 */
void execute_command_secondplane(char* command)
{
    command[strlen(command) - 1] = '\0';

    pid_t pid = fork();

    if (pid == 0)
    { // Child process
        int original_stdin = dup(STDIN_FILENO);
        int original_stdout = dup(STDOUT_FILENO);
        int original_stderr = dup(STDERR_FILENO);

        if (freopen("/tmp/command_output.txt", "w", stdout) == NULL)
        {
            perror("freopen failed");
        }
        if (freopen("/tmp/command_output.txt", "w", stderr) == NULL)
        {
            perror("freopen failed");
        }

        execute_command(command);

        restore_io(original_stdin, original_stdout, original_stderr);

        close(original_stdin);
        close(original_stdout);
        close(original_stderr);

        exit(0);
    }
    else if (pid > 0)
    { // Parent process
        set_foreground_pid(pid);

        printf(COLOR_YELLOW "[%d] %d" COLOR_RESET "\n", job_id++, pid);

        int status;
        waitpid(pid, &status, 0);

        FILE* file = fopen("/tmp/command_output.txt", "r");
        if (file != NULL)
        {
            char line[256];
            while (fgets(line, sizeof(line), file) != NULL)
            {
                printf("%s", line);
            }
            fclose(file);
        }
    }
    else
    { // If pid is -1
        perror("fork");
    }
    set_foreground_pid(0);
}

/**
 * @brief Executes chained commands using pipes.
 *
 * Separates commands by the '|' character, creates a pipe for each pair,
 * and redirects the output of one command to the input of the next.
 *
 * @param command The full command string with pipes.
 */
void execute_piped_commands(char* command)
{
    char* commands[20];
    int num_commands = 0;
    char* command_n = strtok(command, "|");
    while (command_n != NULL)
    {
        commands[num_commands++] = command_n;
        command_n = strtok(NULL, "|");
    }

    int* filedes = malloc(2 * (num_commands - 1) * sizeof(int));
    for (int j = 0; j < num_commands - 1; j++)
    {
        if (pipe(filedes + j * 2) < 0)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int j = 0;
    for (int i = 0; i < num_commands; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            if (i < num_commands - 1)
            {
                if (dup2(filedes[j + 1], STDOUT_FILENO) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            if (j != 0)
            {
                if (dup2(filedes[j - 2], STDIN_FILENO) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            for (int k = 0; k < 2 * (num_commands - 1); k++)
            {
                close(filedes[k]);
            }

            char* arguments[10];
            char* cmd_token = strtok(commands[i], " ");
            int arg_count = 0;
            while (cmd_token != NULL)
            {
                arguments[arg_count++] = cmd_token;
                cmd_token = strtok(NULL, " ");
            }
            arguments[arg_count] = NULL;

            execvp(arguments[0], arguments);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        j += 2;
    }

    for (int i = 0; i < 2 * (num_commands - 1); i++)
    {
        close(filedes[i]);
    }
    for (int i = 0; i < num_commands; i++)
    {
        wait(NULL);
    }
}

/**
 * @brief Executes a command with I/O redirection.
 *
 * Redirects stdin from a file and/or stdout/stderr to a file.
 *
 * @param command The command string with redirection symbols.
 */
void execute_command_redirection(char* command)
{
    char* input_file = NULL;
    char* output_file = NULL;
    char* program = NULL;
    char* args[100];
    int arg_count = 0;

    char* token = strtok(command, " ");
    while (token != NULL)
    {
        if (strcmp(token, "<") == 0)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                input_file = token;
            }
        }
        else if (strcmp(token, ">") == 0)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                output_file = token;
            }
        }
        else
        {
            if (program == NULL)
            {
                program = token;
            }
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    int original_stderr = dup(STDERR_FILENO);

    if (input_file != NULL)
    {
        int input_fd = open(input_file, O_RDONLY);
        if (input_fd < 0)
        {
            perror("open input file");
            exit(EXIT_FAILURE);
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    if (output_file != NULL)
    {
        int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0)
        {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        dup2(output_fd, STDOUT_FILENO);
        dup2(output_fd, STDERR_FILENO);
        close(output_fd);
    }

    execute_command(program);
    restore_io(original_stdin, original_stdout, original_stderr);
    close(original_stdin);
    close(original_stdout);
    close(original_stderr);
}

/**
 * @brief Restores standard I/O to their original file descriptors.
 *
 * @param original_stdin The original file descriptor for standard input.
 * @param original_stdout The original file descriptor for standard output.
 * @param original_stderr The original file descriptor for standard error.
 */
void restore_io(int original_stdin, int original_stdout, int original_stderr)
{
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    dup2(original_stderr, STDERR_FILENO);
}

/**
 * @brief Signal handler function.
 *
 * If an interrupt signal is received, it sends the signal to the foreground process.
 *
 * @param signo The signal number being handled.
 */
void signal_handler(int signo)
{
    if (foreground_pid > 0)
    {
        kill(foreground_pid, signo);
    }
}

/**
 * @brief Sets up signal handlers for the shell.
 */
void setup_signals()
{
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGQUIT, signal_handler);
}

/**
 * @brief Sets the PID of the foreground process.
 *
 * @param pid The PID of the new foreground process.
 */
void set_foreground_pid(pid_t pid)
{
    foreground_pid = pid;
}