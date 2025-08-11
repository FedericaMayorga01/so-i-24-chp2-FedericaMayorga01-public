#include "../include/commands.h"
#include "../include/executions.h"
#include "../include/monitor.h"
#include "../include/shell.h"
#include "../lab1/include/metrics.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    // Initialize the metrics system
    init_metrics();

    // Initialize the shell
    init_shell(argc, argv);

    return 0;
}
