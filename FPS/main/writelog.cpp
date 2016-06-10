#include "writelog.h"
#include <stdio.h>
#include <time.h>

bool log(std::string username, const int score)
{
    const char *logfile = "log/log.txt";
    FILE *file = fopen(logfile, "a");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open log.txt");
        return false;
    }

    time_t tnow;
    char* cnow;
    time(&tnow);
    cnow = ctime(&tnow);


    fprintf(file, "%susername: %s | score: %d\n", time, username, score);

    fclose(file);
    return true;
}

