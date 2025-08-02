#include "../include/monitor.h"

static pid_t monitor_pid = -1;
static int monitoring = 0;

/**
 * @brief Starts the system monitor in a child process.
 *
 * It forks a new process to run the monitor program. The child process's
 * standard output and error are redirected to /dev/null to run silently.
 * The parent process updates the monitor status and prints a confirmation message.
 */
void start_monitor()
{
    if (monitor_pid == -1)
    {
        monitor_pid = fork();
        if (monitor_pid == 0)
        {
            int devnull = open("/dev/null", O_RDWR);
            if (devnull != -1)
            {
                dup2(devnull, STDOUT_FILENO);
                dup2(devnull, STDERR_FILENO);
                close(devnull);
            }
            execlp("../lib/tp1_sol_so1/build/tp1_sol_so1", "tp1_sol_so1", NULL);
            perror("execlp failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            monitoring = 1;
            printf("Monitor started.\n");
        }
    }
    else
    {
        printf("The monitor is already running.\n");
    }
}

/**
 * @brief Stops the system monitor.
 *
 * It sends a SIGTERM signal to the monitor process to terminate it and waits
 * for the process to exit. It then resets the monitor status.
 */
void stop_monitor()
{
    if (monitor_pid != -1)
    {
        kill(monitor_pid, SIGTERM);
        waitpid(monitor_pid, NULL, 0);
        monitor_pid = -1;
        monitoring = 0;
        printf("Monitor stopped.\n");
    }
    else
    {
        printf("The monitor is not running.\n");
    }
}

/**
 * @brief Displays the status of the system monitor.
 *
 * It provides an interactive menu to allow the user to choose a specific
 * metric to view. It reads the metric data from a named pipe (FIFO),
 * parses the JSON response, and prints the requested information.
 */
void status_monitor()
{
    const char* fifo_path = "/tmp/monitor_fifo";
    int option = 0;
    printf("Monitoring running (PID: %d).\n\n", getpid());

    printf("Select the metric you want to view:\n");
    printf("    1. CPU Usage\n");
    printf("    2. Memory Usage\n");
    printf("    3. Disk Usage\n");
    printf("    4. Network Usage\n");
    printf("    5. Number of Processes\n");
    printf("    6. Context Switches\n");
    printf("    7. All Metrics\n");
    printf("    8. Exit\n");
    printf("    Select an option (1-8): ");
    if (scanf("%d", &option) != 1)
    {
        fprintf(stderr, "Error reading input\n");
    }
    printf("\n");

    if (option == 8)
    {
        return;
    }

    if (mkfifo(fifo_path, 0666) == -1 && errno != EEXIST)
    {
        perror("Error creating FIFO");
        return;
    }

    int fifo_fd = open(fifo_path, O_RDONLY);
    if (fifo_fd == -1)
    {
        perror("Error opening FIFO");
        return;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_read = read(fifo_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        cJSON* json = cJSON_Parse(buffer);
        if (json)
        {
            cJSON* cpu_usage = cJSON_GetObjectItem(json, "cpu_usage");
            cJSON* memory_usage = cJSON_GetObjectItem(json, "memory_usage");
            cJSON* disk_usage = cJSON_GetObjectItem(json, "disk_usage");
            cJSON* network_usage = cJSON_GetObjectItem(json, "network_usage");
            cJSON* process_count = cJSON_GetObjectItem(json, "process_count");
            cJSON* context_switches = cJSON_GetObjectItem(json, "context_switches");

            if (option == 1 && cpu_usage)
            {
                printf("CPU Usage: %.2f%%\n\n", cpu_usage->valuedouble);
            }
            if (option == 2 && memory_usage)
            {
                printf("Memory Usage: %.2f MB\n\n", memory_usage->valuedouble);
            }
            if (option == 3 && disk_usage)
            {
                printf("Disk Usage: %.2f GB\n\n", disk_usage->valuedouble);
            }
            if (option == 4 && network_usage)
            {
                printf("Network Usage: %.2f KB/s\n\n", network_usage->valuedouble);
            }
            if (option == 5 && process_count)
            {
                printf("Number of Processes: %d\n\n", process_count->valueint);
            }
            if (option == 6 && context_switches)
            {
                printf("Context Switches: %d\n\n", context_switches->valueint);
            }
            if (option == 7)
            {
                if (cpu_usage)
                {
                    printf("CPU Usage: %.2f%%\n", cpu_usage->valuedouble);
                }
                if (memory_usage)
                {
                    printf("Memory Usage: %.2f MB\n", memory_usage->valuedouble);
                }
                if (disk_usage)
                {
                    printf("Disk Usage: %.2f GB\n", disk_usage->valuedouble);
                }
                if (network_usage)
                {
                    printf("Network Usage: %.2f KB/s\n", network_usage->valuedouble);
                }
                if (process_count)
                {
                    printf("Number of Processes: %d\n", process_count->valueint);
                }
                if (context_switches)
                {
                    printf("Context Switches: %d\n", context_switches->valueint);
                }
                printf("\n");
            }
            cJSON_Delete(json);
        }
        else
        {
            fprintf(stderr, "Error parsing the received JSON.\n");
        }
    }
    else
    {
        fprintf(stderr, "Error reading data from FIFO.\n");
    }
    close(fifo_fd);
}