#include "count.hpp"
#include <stdio.h>
#include <time.h>

bool log(const char *username, const int score)
{
    const char *logfile = "log.txt";
    FILE *file = fopen(logfile, "a");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open log.txt");
        return false;
    }

    time_t tnow;
    char *cnow;
    time(&tnow);
    cnow = ctime(&tnow);

    fprintf(file, "%susername: %s | score: %d\n", cnow, username, score);

    fclose(file);
    return true;
}
