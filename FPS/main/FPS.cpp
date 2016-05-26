#include "main.hpp"
#include "title.hpp"
#include "count.hpp"
#include <btBulletDynamicsCommon.h>
#include <common/bulletfunc.hpp>
#include <GLFW/glfw3.h>

GLFWwindow* window;

btDynamicsWorld* dynamicsWorld;
btRigidBody* groundBody;

int main(){
//    if(!title()){
//        fprintf(stderr,"Failed to open TITLE.\n");
//        return -1;
//    }
    if(!start_fps()){
        fprintf(stderr,"Failed to start FPS.\n");
        return -1;
    }
    const char* username = "rei";
    const int score = 33;
    if(!log(username,score)){
        fprintf(stderr,"Failed to write log file.\n");
        return -1;
    }
    return 0;
}
