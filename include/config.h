#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int monitor_interval;
    int max_background_jobs;
    char monitor_metrics[256];
} ShellConfig;

int load_config(const char* config_file, ShellConfig* config);
int save_config(const char* config_file, const ShellConfig* config);

#endif // CONFIG_H