#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

unsigned long get_time_ms();
static void alarm_handler(int signum);

static volatile sig_atomic_t perform_sampling = 1;

int main(int argc, char **argv)
{
    printf("without called\n");

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

    // Convert input to long int and float
    // Use the strto* family of functions to store unwanted text input to a pointer
    float duration = strtof(argv[1], &noise);
    float interval = strtof(argv[2], &noise);
    // Replace old records
    FILE *f = fopen("without_gettimeofday.csv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Calculated how many entries will be needed for sampling

    unsigned int timestamp_entries = (int)(duration / interval + 1);
    //A us resolution is enough for our needs, thus we use a long data type and not
    // a float.
    long interval_ms = interval * 1000;

    unsigned int duration_ms = duration * 1000;
    timestamps = malloc(timestamp_entries * sizeof(unsigned long));

    struct itimerval t_timer;
    t_timer.it_value.tv_sec = duration_ms / 1000.0;
    t_timer.it_value.tv_usec = (duration_ms * 1000) % 1000000;

    t_timer.it_interval = t_timer.it_value;
    if (setitimer(ITIMER_REAL, &t_timer, NULL) == -1)
    {
        perror("error calling setitimer()");
        exit(1);
    }

    struct timespec t_sleep, t_rem;
    t_sleep.tv_sec = (int)interval_ms / 1000;
    t_sleep.tv_nsec = (interval_ms * 1000000) % 1000000000;;
    unsigned int counter_timestamps = 0;

    // When the timer expires the perform_sampling flag will be toggled
    while (perform_sampling)
    {
        timestamps[counter_timestamps] = get_time_ms();
        counter_timestamps++;
        if (nanosleep(&t_sleep, &t_rem) < 0)
        {
            if (!perform_sampling) break;
            printf("Nano sleep system call failed \n");
            return -1;
        }
    }

    // Write timestamps to file
    for (int i = 0; i < counter_timestamps; i++)
    {
        fprintf(f, "%ld\n", timestamps[i]);
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
    perform_sampling = 0;
}