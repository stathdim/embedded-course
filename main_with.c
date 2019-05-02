#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

unsigned long get_time_ms();

int main(int argc, char **argv) {

    if (argc < 3) {
        printf("Invalid number of arguments.\n");
        return 1;
    }

    char *noise;

    unsigned long *timestamps;

    // Convert input to long int and double
    // Use the strto* family of functions to store unwanted text input to a pointer
    unsigned int duration = strtol(argv[1], &noise, 10);
    double interval = strtod(argv[2], &noise);
    FILE *f = fopen("with_gettimeofday.csv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }


    printf("Version using gettimeofday value. Duration is %d with an interval of %.2lf between samples\n", duration, interval);

    // Calculated how many entries will be needed for sampling
    unsigned int timestamp_entries = duration / interval;
    unsigned int interval_ms = interval * 1000;
    timestamps = malloc(timestamp_entries * sizeof(unsigned long));
    for (unsigned int counter_timestamps = 0; counter_timestamps < timestamp_entries; counter_timestamps++) {

        unsigned long time_ms = get_time_ms();

        timestamps[counter_timestamps] = time_ms;
        counter_timestamps++;
        fprintf(f, "%ld,", time_ms);

        // Sleep for ${interval} seconds
        usleep( timestamps[counter_timestamps-1]+interval_ms-get_time_ms());

    }
    fclose(f);

    return 0;
}

unsigned long get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // Get the microseconds of the timestamp and convert them to milliseconds
    unsigned long current_time_us_to_ms = tv.tv_usec / 1000;
    // Get the seconds of the timestamp, convert them to ms
    unsigned long current_time_s_to_ms = tv.tv_sec * 1000;
    // Calc the current timestamp in ms and return it
    return current_time_s_to_ms + current_time_us_to_ms;
}
