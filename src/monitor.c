#include "../include/monitor.h"
#include "../lab1/include/metrics.h"

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
        return;
    }
    printf("\n");

    if (option == 8)
    {
        return;
    }

    double cpu_usage, memory_usage, disk_usage, network_usage;
    int process_count, context_switches;

    switch (option)
    {
    case 1:
        cpu_usage = get_cpu_usage();
        if (cpu_usage >= 0)
        {
            printf("CPU Usage: %.2f%%\n\n", cpu_usage);
        }
        else
        {
            printf("Error getting CPU usage\n\n");
        }
        break;

    case 2:
        memory_usage = get_memory_usage();
        if (memory_usage >= 0)
        {
            printf("Memory Usage: %.2f%%\n\n", memory_usage);
        }
        else
        {
            printf("Error getting memory usage\n\n");
        }
        break;

    case 3:
        disk_usage = get_IOdisk();
        if (disk_usage >= 0)
        {
            printf("Disk Usage: %.2f%%\n\n", disk_usage);
        }
        else
        {
            printf("Error getting disk usage\n\n");
        }
        break;

    case 4:
        network_usage = get_network_transfer_rate();
        if (network_usage >= 0)
        {
            printf("Network Usage: %.2f bytes/s\n\n", network_usage);
        }
        else
        {
            printf("Error getting network usage\n\n");
        }
        break;

    case 5:
        process_count = get_processcounter();
        if (process_count >= 0)
        {
            printf("Number of Processes: %d\n\n", process_count);
        }
        else
        {
            printf("Error getting process count\n\n");
        }
        break;

    case 6:
        context_switches = get_context_switchs();
        if (context_switches >= 0)
        {
            printf("Context Switches: %d\n\n", context_switches);
        }
        else
        {
            printf("Error getting context switches\n\n");
        }
        break;

    case 7:
        printf("=== All System Metrics ===\n");

        cpu_usage = get_cpu_usage();
        if (cpu_usage >= 0)
        {
            printf("CPU Usage: %.2f%%\n", cpu_usage);
        }
        else
        {
            printf("CPU Usage: Error\n");
        }

        memory_usage = get_memory_usage();
        if (memory_usage >= 0)
        {
            printf("Memory Usage: %.2f%%\n", memory_usage);
        }
        else
        {
            printf("Memory Usage: Error\n");
        }

        disk_usage = get_IOdisk();
        if (disk_usage >= 0)
        {
            printf("Disk Usage: %.2f%%\n", disk_usage);
        }
        else
        {
            printf("Disk Usage: Error\n");
        }

        network_usage = get_network_transfer_rate();
        if (network_usage >= 0)
        {
            printf("Network Usage: %.2f bytes/s\n", network_usage);
        }
        else
        {
            printf("Network Usage: Error\n");
        }

        process_count = get_processcounter();
        if (process_count >= 0)
        {
            printf("Number of Processes: %d\n", process_count);
        }
        else
        {
            printf("Number of Processes: Error\n");
        }

        context_switches = get_context_switchs();
        if (context_switches >= 0)
        {
            printf("Context Switches: %d\n", context_switches);
        }
        else
        {
            printf("Context Switches: Error\n");
        }
        printf("\n");
        break;

    default:
        printf("Invalid option. Please select 1-8.\n\n");
        break;
    }
}