#include "../include/commands.h"
#include "../include/colors.h"
#include "../include/monitor.h"

// Definition of the internal commands array
Command internals_commands[] = {
    {"cd", command_cd},
    {"echo", command_echo},
    {"clr", (void (*)(char*))command_clear},
    {"quit", (void (*)(char*))command_quit},
    {"start_monitor", start_monitor},
    {"stop_monitor", stop_monitor},
    {"status_monitor", status_monitor},
};

/**
 * @brief Executes an external command by creating a child process and using execvp.
 */
void external_command(char* command)
{

    // Create a child process
    pid_t pid = fork();

    switch (pid)
    {
    // There was an error creating the child process
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;
    // The child process was created correctly
    case 0: {
        // Separate the command and its arguments
        char* args[256];
        int i = 0;
        args[i] = strtok(command, " ");
        while (args[i] != NULL && i < 255)
        {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Execute the program
        if (execvp(args[0], args) == -1)
        {
            printf(COLOR_RED "Comando no encontrado: %s" COLOR_RESET "\n", args[0]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        break;
    }

    // Parent process
    default:
        set_foreground_pid(pid);
        // Wait for the child process to finish
        waitpid(pid, NULL, 0);
        set_foreground_pid(0);
        break;
    }
}

/**
 * @brief Changes the current directory to the specified one, updating PWD and OLDPWD environment variables.
 *
 * @param arg The directory to change to. If NULL or empty, changes to HOME. If "-", changes to OLDPWD.
 */
void command_cd(char* arg)
{
    char* directory;
    char cwd[1024];
    char* oldpwd = getenv("PWD");

    if (arg == NULL || strcmp(arg, "") == 0)
    {
        directory = getenv("HOME");
        if (directory == NULL)
        {
            fprintf(stderr, "cd: HOME not defined\n");
            return;
        }
    }
    else if (strcmp(arg, "-") == 0)
    {
        directory = getenv("OLDPWD");
        if (directory == NULL)
        {
            fprintf(stderr, "cd: OLDPWD not defined\n");
            return;
        }
        printf("%s\n", directory);
    }
    else
    {
        directory = arg;
    }

    if (chdir(directory) != 0)
    {
        perror("cd");
        return;
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        setenv("OLDPWD", oldpwd, 1);
        setenv("PWD", cwd, 1);
    }
    else
    {
        perror("getcwd");
    }
}

/**
 * @brief Echoes the input string, expanding environment variables prefixed with $.
 *
 * @param arg The string to echo, which may contain environment variable references.
 */
void command_echo(char* arg)
{
    if (arg == NULL)
    {
        printf("\n");
        return;
    }

    if (arg[0] == '$')
    {
        char* env_var = getenv(arg + 1);
        if (env_var != NULL)
        {
            printf("%s\n", env_var);
        }
        else
        {
            printf("Environment variable not found: %s\n", arg + 1);
        }
    }
    else
    {
        printf("%s\n", arg);
    }
}

/**
 * @brief Clears the terminal screen by moving the cursor to the top left and clearing the screen.
 */
void command_clear()
{
    printf("\033[H\033[J");
}

/**
 * @brief Exits the shell.
 */
void command_quit()
{
    exit(0);
}