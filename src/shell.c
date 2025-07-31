#include "../include/shell.h"

void prompt(void);
void choose_execution(char* command);

/*
** Program initialization and input handling
*/
int init_shell(int argc, char* argv[])
{
    // Welcome message!!
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║              SURVIVAL TERMINAL v1.0            ║\n");
    printf("║                                                ║\n");
    printf("║        Refuge operating system initiated       ║\n");
    printf("║           May luck be on your side!            ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("System resources loaded... [OK]\n");
    printf("Monitoring connection established... [OK]\n\n");

    setup_signals();

    if (argc > 1)
    { // A batch file was sent
        // Read commands from the file
        FILE* file = fopen(argv[1], "r");
        if (file == NULL)
        {
            fprintf(stderr, "ERROR: Could not access command file '%s'\n", argv[1]);
            perror("Error details");
            exit(EXIT_FAILURE);
        }

        char command[256];
        int line_number = 1;

        printf("Executing commands from batch file: %s\n\n", argv[1]);

        // The loop executes all commands in the file, fgets handles moving to the next line in each loop
        while (fgets(command, sizeof(command), file) != NULL)
        {
            // Validation to prevent buffer overflow
            if (strlen(command) == sizeof(command) - 1 && command[sizeof(command) - 2] != '\n')
            {
                fprintf(stderr, "WARNING: Command on line %d is too long, truncated\n", line_number);
                // Clear the rest of the line from the buffer
                int c;
                while ((c = fgetc(file)) != '\n' && c != EOF)
                    ;
            }

            // Show which command is being executed
            printf("[Line %d] Executing: %s", line_number, command);

            // Select what type of execution
            choose_execution(command);

            line_number++;
        }

        if (ferror(file))
        {
            fprintf(stderr, "ERROR: Problem reading the command file\n");
            perror("Error details");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        fclose(file);
        printf("\nBatch execution completed. Shelter systems ready.\n");
    }
    else
    {
        // If executed without arguments
        char command[256];
        printf("Interactive mode activated. Type 'quit' to exit the system.\n\n");

        while (1)
        {
            prompt();

            if (fgets(command, sizeof(command), stdin) == NULL)
            {
                // Improved handling of reading errors
                if (feof(stdin))
                {
                    printf("\nEOF signal detected. Closing survival terminal...\n");
                    break;
                }
                else if (ferror(stdin))
                {
                    fprintf(stderr, "\nERROR: Failed to read commands from terminal\n");
                    perror("Error details");
                    clearerr(stdin); // Clear the error state
                    continue;
                }
            }

            // Validation to prevent buffer overflow
            if (strlen(command) == sizeof(command) - 1 && command[sizeof(command) - 2] != '\n')
            {
                fprintf(stderr, "WARNING: Command too long, truncated to %zu characters\n", sizeof(command) - 1);
                // Clear the rest of the line from the buffer
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            }

            choose_execution(command);
        }
    }
    return 0;
}

/*
 * Function that prints the prompt
 * for each new command line
 */
void prompt()
{
    char* user = getenv("USER");
    if (user == NULL)
    {
        user = "survivor";
    }

    char hostname[1024];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        fprintf(stderr, "WARNING: Could not get the shelter name\n");
        strncpy(hostname, "unknown-refuge", sizeof(hostname) - 1);
        hostname[sizeof(hostname) - 1] = '\0';
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        fprintf(stderr, "WARNING: Could not determine the current location\n");
        strncpy(cwd, "/unknown-location", sizeof(cwd) - 1);
        cwd[sizeof(cwd) - 1] = '\0';
    }

    // Print the prompt with the user, hostname, and current working directory
    printf("🏚️ %s@%s:%s$ ", user, hostname, cwd);
    fflush(stdout);
}

/*
 * Function that chooses the type of execution
 * based on the command syntax
 */
void choose_execution(char* command)
{
    // Validate that the command is not empty
    if (command == NULL || strlen(command) == 0)
    {
        return;
    }
    // Remove the newline character at the end of the command
    command[strcspn(command, "\n")] = 0;

    // Validate that there is still content after removing the newline character
    if (strlen(command) == 0)
    {
        return; // Empty command, do nothing
    }

    // Verify if the command ends with '&' to execute in the background
    if (command[strlen(command) - 1] == '&')
    {
        execute_command_secondplane(command);
    }
    else if (strchr(command, '|') != NULL) // Verify if there is a pipe in the code
    {
        execute_piped_commands(command);
    }
    else if (strchr(command, '<') != NULL || strchr(command, '>') != NULL) // Verify if there is a < or > symbol
    {
        execute_command_redirection(command);
    }
    else
    { // If there are no special characters, execute the command normally
        execute_command(command);
    }
}