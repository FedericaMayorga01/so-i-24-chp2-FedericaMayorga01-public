#ifndef MONITOR_H
#define MONITOR_H

#include "commands.h"
#include "executions.h"

#include <cjson/cJSON.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Starts the monitoring process.
 *
 * This function creates a background process to run the metrics monitor.
 * It redirects stdout and stderr to `/dev/null` in the child process.
 */
void start_monitor();

/**
 * @brief Stops the monitoring process.
 *
 * This function sends a `SIGTERM` signal to the background monitoring process,
 * stops the process, and resets the state of the `monitor_pid` and `monitoring` variables.
 */
void stop_monitor();

/**
 * @brief Displays the status of metrics monitoring.
 *
 * This function provides an interactive menu to select specific metrics or all
 * available metrics. It reads data from a FIFO in JSON format and presents the
 * metrics based on the selected option.
 */
void status_monitor();

#endif // MONITOR_H