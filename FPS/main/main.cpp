#include "writelog.h"
#include "game.h"
#include "title.h"
#include "util/windowcreater.h"
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include "util/bulletgenobj.h"

int main()
{
    GLFWOneTimeInit glfw;
    if(!title())
    {
        fprintf(stderr, "Failed to open TITLE.\n");
        return -1;
    }
    if(!startGame())
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
