#include "writelog.h"
#include "main.h"
#include "title.h"
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <util/bulletfunc.h>

int main()
{
    if(!title())
    {
        fprintf(stderr, "Failed to open TITLE.\n");
        return -1;
    }
    if(!start_fps())
    {
        fprintf(stderr, "Failed to start FPS.\n");
        return -1;
    }
    const char* username = "rei";
    const int score = 33;
    if(!log(username, score))
    {
        fprintf(stderr, "Failed to write log file.\n");
        return -1;
    }
    return 0;
}
