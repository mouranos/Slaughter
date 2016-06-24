#include "writelog.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>

bool log(const char* username, const int score)
{
    const char *logfile = "log/log.txt";
    FILE *file = fopen(logfile, "a");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open log.txt\n");
        return false;
    }

    time_t tnow;
    char* cnow;
    if(time(&tnow) == -1){
        fprintf(stderr ,"Failed to get time.\n");
        return false;
    }
    cnow = ctime(&tnow);


    fprintf(file, "%susername: %s | score: %d\n", cnow, username, score);

    fclose(file);
    return true;
}
