#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <common/shader.h>
#include <common/windowCreater.h>
#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include FT_FREETYPE_H
#include <iostream>
#include "title.h"

constexpr unsigned int WIDTH = 1440;
constexpr unsigned int HEIGHT = 900;

bool title()
{
    Window window(WIDTH, HEIGHT, "開成祭 FPS-TITLE");
    window.windowInit();

    glfwSetInputMode(window.getWindowptr(), GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window.getWindowptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glClearColor(0.21f, 0.24f, 0.29f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint programID = LoadShaders("shaders/titleVertexShader.vertexshader",
                                   "shaders/titleFragmentShader.fragmentshader");

    GLuint vertexPosition_screenspaceID =
        glGetAttribLocation(programID, "vertexPosition_screenspace");
    GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
    GLint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    // freetype

    int error;
    FT_Library library;
    FT_Face face;
    const char32_t text[] = U"Press Enter";
    const unsigned int textLength = sizeof(text)/sizeof(text[0]);
    float pen_x, pen_y;
    pen_x = 300;
    pen_y = 300;

    error = FT_Init_FreeType(&library);
    if(error)
    {
        fprintf(stderr, "an occurred during library initialization.\n");
        return false;
    }

    error = FT_New_Face(library, "/usr/share/fonts/noto/NotoSans-Regular.ttf",
                        0, &face);
    if(error)
    {
        fprintf(stderr,
                "An error occured during loading fonts from memory %d.\n",
                error);
        return false;
    }

    error = FT_Set_Char_Size(face, 0, 16 * 64, 96, 0);
    if(error)
    {
        fprintf(stderr, "An error occued during setting character size.\n");
        return false;
    }

    std::array<std::array<std::array<GLfloat, 2>, 4>, textLength> vertex_buffer_data;
    std::array<GLuint, textLength> char_textures;

    glGenTextures(char_textures.size(), char_textures.data());

    for(int n = 0; text[n] != '\0'; n++)
    {
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(face, text[n]);
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if(error){
            std::cerr<<"Load glyph error: "<<error<<std::endl;continue;}
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if(error){
            std::cerr<<"Render glyph error: "<<error<<std::endl;continue;}
        FT_GlyphSlot slot = face->glyph;
        const FT_Bitmap& bitmap = slot->bitmap;

        glBindTexture(GL_TEXTURE_2D, char_textures[n]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.rows, 0,
                     GL_BGR, GL_UNSIGNED_BYTE, bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);

        vertex_buffer_data[n][0][0] = pen_x;
        vertex_buffer_data[n][0][1] = pen_y;
        vertex_buffer_data[n][1][0] = pen_x + bitmap.width;
        vertex_buffer_data[n][1][1] = pen_y;
        vertex_buffer_data[n][2][0] = pen_x + bitmap.width;
        vertex_buffer_data[n][2][1] = pen_y + bitmap.rows;
        vertex_buffer_data[n][3][0] = pen_x;
        vertex_buffer_data[n][3][1] = pen_y + bitmap.rows;
        pen_x += bitmap.width;
    }

    GLuint vertexbuffer[textLength];
    glGenBuffers(textLength, vertexbuffer);
    for(int i = 0;i < textLength;i++){
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data.data(),
                 GL_STATIC_DRAW);
    }

    static const GLfloat uv_buffer_data[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
    };

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data,GL_STATIC_DRAW);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        for(int i = 0;i < char_textures.size();i++){
            glEnableVertexAttribArray(vertexPosition_screenspaceID);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
            glVertexAttribPointer(vertexPosition_screenspaceID, 2, GL_FLOAT,
                                  GL_FALSE, 0, static_cast<GLvoid*>(0));

            glBindTexture(GL_TEXTURE_2D, char_textures[i]);

            glUniform1i(TextureID, 0);
            glEnableVertexAttribArray(vertexUVID);
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
            glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0,
                                  static_cast<GLvoid*>(0));
            glDrawArrays(GL_QUADS, 0, 4);
        }

        glDisableVertexAttribArray(vertexPosition_screenspaceID);
        glDisableVertexAttribArray(vertexUVID);

        glfwSwapBuffers(window.getWindowptr());
        glfwPollEvents();
    } while(glfwGetKey(window.getWindowptr(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window.getWindowptr()) == 0);

    glDeleteTextures(char_textures.size(), char_textures.data());

    glDeleteBuffers(1,&uvbuffer);
    glDeleteBuffers(textLength, vertexbuffer);
    glDeleteProgram(programID);

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    glfwTerminate();
    return true;
}
