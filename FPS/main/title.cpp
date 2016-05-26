#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/windowCreater.hpp>
#include <common/shader.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "title.hpp"

extern GLFWwindow* window;
const int windowWidth = 1440;
const int windowHeight = 900;

bool title(){
    if(!windowInit(windowWidth,windowHeight,"開成祭 FPS-TITLE"))
        return false;

    glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glClearColor(0.21f,0.24f,0.29f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint programID = LoadShaders("titleVertexShader.vertexshader","titleFragmentShader.fragmentshader");

    GLuint vertexPosition_screenspaceID = glGetAttribLocation(programID, "vertexPosition_screenspace");
    
    //freetype

    int error;
    FT_Library library;
    FT_Face face;
    const unsigned char text[] = "Enterを押してください";
    FT_GlyphSlot slot = face->glyph;
    int pen_x, pen_y, n;
    GLuint texIDs[sizeof(text)];

    error = FT_Init_FreeType(&library);
    if(error){
        fprintf(stderr, "An error occurred during library initialization.\n");
        return false;
    }

    error = FT_New_Memory_Face(
                library,
                text,
                sizeof(text),
                0,
                &face);
    if(error){
        fprintf(stderr, "An error occured during loading fonts from memory %d .\n",error);
        return false;
    }

    error = FT_Set_Char_Size(
                face,
                0,
                16*64,
                96,
                0);
    if(error){
        fprintf(stderr, "An error occued during setting character size.\n");
        return false;
    }

    pen_x = 300;
    pen_y = 200;

    for( n = 0; text[n]!='\0'; n++ ){
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index( face, text[n]);
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
        if( error )
            continue;
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        if( error )
            continue;
        const FT_Bitmap &bitmap = face->glyph->bitmap;
        GLuint textureID;
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, bitmap.width, bitmap.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        texIDs[n] = textureID;

    }

    static const GLfloat texuv[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, windowWidth, 0.0f, windowHeight, -1.0f, 1.0f);

    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        for(int i=0;i<sizeof(texIDs);i++){
            glTexCoordPointer(2,GL_FLOAT,0,texuv);
            glBindTexture(GL_TEXTURE_2D, texIDs[i]);
            glTranslatef(100.0f,0.0f,0.0f);
            glDrawArrays(GL_QUADS,0,4);
        }
        glDisable(GL_TEXTURE_2D);

        glTexCoordPointer(2,GL_FLOAT,0,texuv);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    
    glfwTerminate();
    return true;
}
