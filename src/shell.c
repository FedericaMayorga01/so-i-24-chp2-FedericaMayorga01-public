#include "../include/shell.h"
#include "../include/colors.h"

void prompt(void);
void choose_execution(char* command);

/**
 * @brief Initializes the shell and handles command input.
 *
 * This function serves as the main entry point for the shell. It prints a
 * welcome message, sets up signal handlers, and then either reads commands
 * from a specified batch file or enters an interactive loop to handle
 * user input from stdin.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return Returns 0 upon successful execution.
 */
int init_shell(int argc, char* argv[])
{
    // Welcome message!!
    printf(COLOR_CYAN COLOR_BOLD);
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║              SURVIVAL TERMINAL v1.0            ║\n");
    printf("║                                                ║\n");
    printf("║        Refuge operating system initiated       ║\n");
    printf("║           May luck be on your side!            ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf(COLOR_RESET);
    printf("System resources loaded... [OK]\n");
    printf("Monitoring connection established... [OK]\n\n");

    setup_signals();

    if (argc > 1)
    {
        FILE* file = fopen(argv[1], "r");
        if (file == NULL)
        {
            printf(COLOR_RED "Error opening file: %s" COLOR_RESET, argv[1]);
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        char command[256];
        while (fgets(command, sizeof(command), file) != NULL)
        {
            choose_execution(command);
        }
        fclose(file);
    }
    else
    {
        char command[256];
        while (1)
        {
            prompt();
            if (fgets(command, sizeof(command), stdin) != NULL)
            {
                choose_execution(command);
            }
        }
    }
    return 0;
}

/**
 * @brief Prints the command prompt.
 *
 * This function displays a command prompt showing the current user,
 * hostname, and working directory, similar to a standard shell.
 */
void prompt()
{
    char* user = getenv("USER");
    if (user == NULL)
    {
        user = "unknown";
    }

    char hostname[1024];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    printf(COLOR_GREEN "%s" COLOR_RESET "@" COLOR_BLUE "%s" COLOR_RESET ":" COLOR_YELLOW "%s" COLOR_RESET "$ ", user,
           hostname, cwd);
}

/**
 * @brief Chooses the type of command execution.
 *
 * It inspects the command string for special characters to determine
 * if it should be executed in the background ('&'), piped ('|'),
 * with I/O redirection ('<' or '>'), or as a standard command.
 *
 * @param command The command string to analyze and execute.
 */
void choose_execution(char* command)
{
    command[strcspn(command, "\n")] = 0;

    if (command[strlen(command) - 1] == '&')
    {
        execute_command_secondplane(command);
    }
    else if (strchr(command, '|') != NULL)
    {
        execute_piped_commands(command);
    }
    else if (strchr(command, '<') != NULL || strchr(command, '>') != NULL)
    {
        execute_command_redirection(command);
    }
    else
    {
        execute_command(command);
    }
}
