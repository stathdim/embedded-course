#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

unsigned long get_time_ms();
static void alarm_handler(int signum);

static volatile sig_atomic_t perform_sampling = 0;

int main(int argc, char **argv)
{

    if (argc < 3)
    {
        printf("Invalid number of arguments.\n");
        return 1;
    }
    // // Setup the action handler
    struct sigaction action;

    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = alarm_handler;

    if (sigaction(SIGALRM, &action, NULL) == -1)
    {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    // Read supplied arguments
    char *noise;
    unsigned long *timestamps;

    // Convert input to long int and double
    // Use the strto* family of functions to store unwanted text input to a pointer
    unsigned int duration = strtol(argv[1], &noise, 10);
    double interval = strtod(argv[2], &noise);
    // Replace old records
    FILE *f = fopen("without_gettimeofday.csv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Calculated how many entries will be needed for sampling
    unsigned int timestamp_entries = duration / interval;
    unsigned int interval_ms = interval * 1000;
    timestamps = malloc(timestamp_entries * sizeof(unsigned long));

    perform_sampling = 1;

    for (unsigned int counter_timestamps = 0; counter_timestamps < timestamp_entries; counter_timestamps++)
    {
        unsigned long time_ms;
        if (perform_sampling)
        {
            perform_sampling = 0;
            time_ms = get_time_ms();
            timestamps[counter_timestamps] = time_ms;
            fprintf(f, "%ld\n", get_time_ms());
            struct itimerval t;
            t.it_value.tv_sec = interval_ms / 1000;
            t.it_value.tv_usec = (interval_ms * 1000) % 1000000;

            t.it_interval = t.it_value;

            if (setitimer(ITIMER_REAL, &t, NULL) == -1)
            {
                perror("error calling setitimer()");
                exit(1);
            }
        }
        else
        {
        }
        pause();
        // usleep((time_ms + interval_ms - get_time_ms()) * 1000);
    }
    fclose(f);
    return 0;
}

unsigned long get_time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // Get the microseconds of the timestamp and convert them to milliseconds
    unsigned long current_time_us_to_ms = tv.tv_usec / 1000;
    // Get the seconds of the timestamp, convert them to ms
    unsigned long current_time_s_to_ms = tv.tv_sec * 1000;
    // Calc the current timestamp in ms and return it
    return current_time_s_to_ms + current_time_us_to_ms;
}

static void alarm_handler(int signum)
{
    perform_sampling = 1;
}