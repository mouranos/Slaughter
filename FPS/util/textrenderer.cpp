#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include "shader.h"
#include "textrenderer.h"

RenderText::RenderText(std::u32string text, unsigned int textSize)
    : text_(std::move(text)), textSize_(std::move(textSize))
{
    int error;
    float x = 0, y = 0;
    vertex_buffer_data.resize(text_.length());
    vertexbuffer.resize(text_.length());
    char_textures.resize(text_.length());

    programID = LoadShaders("shader/2DVertexShader.glsl",
                            "shader/2DFragmentShader.glsl");

    vertexPositionID =
        glGetAttribLocation(programID, "vertexPosition_screenspace");
    vertexUVID = glGetAttribLocation(programID, "vertexUV");
    textureID = glGetUniformLocation(programID, "myTextureSampler");

    if(FT_Init_FreeType(&library) != 0)
    {
        throw std::runtime_error(
            "An error occurred during FreeType library initialization.");
    }

    if(error = FT_New_Face(library, "/usr/share/fonts/noto/NotoSansCJK-Regular.ttc", 0,
                   &face) != 0)
    {
        fprintf(stderr,
                "An error occured during loading fonts from memory %d.\n",
                error);
    }

    error = FT_Set_Char_Size(face, 0, textSize_ << 6, 96, 0);
    if(error)
    {
        fprintf(stderr, "An error occued during setting character size.\n");
    }

    glGenTextures(char_textures.size(), char_textures.data());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int i = 0; i<text_.length(); i++)
    {
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(face, text_[i]);
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if(error)
        {
            std::cerr << "Load glyph error: " << error << std::endl;
            continue;
        }
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if(error)
        {
            std::cerr << "Render glyph error: " << error << std::endl;
            continue;
        }

        FT_GlyphSlot slot = face->glyph;
        const FT_Bitmap& bitmap = slot->bitmap;

        glBindTexture(GL_TEXTURE_2D, char_textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0,
                     GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

        vertex_buffer_data[i][0][0] = x + slot->bitmap_left;
        vertex_buffer_data[i][0][1] = y + slot->bitmap_top - bitmap.rows;
        vertex_buffer_data[i][1][0] = x + slot->bitmap_left + bitmap.width;
        vertex_buffer_data[i][1][1] = y + slot->bitmap_top - bitmap.rows;
        vertex_buffer_data[i][2][0] = x + slot->bitmap_left + bitmap.width;
        vertex_buffer_data[i][2][1] = y + slot->bitmap_top;
        vertex_buffer_data[i][3][0] = x + slot->bitmap_left;
        vertex_buffer_data[i][3][1] = y + slot->bitmap_top;
        x += static_cast<float>(slot->advance.x) / 64;
        y -= static_cast<float>(slot->advance.y) / 64;
    }

    constexpr GLfloat uv_buffer_data[] = {0.f, 1.f, 1.f, 1.f,
                                          1.f, 0.f, 0.f, 0.f};

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data,
                 GL_STATIC_DRAW);
}

void RenderText::setText(std::u32string text)
{
    text_ = std::move(text);
    int error;
    float x = 0, y = 0;
    vertex_buffer_data.resize(text_.length());
    vertexbuffer.resize(text_.length());
    char_textures.resize(text_.length());

    error = FT_Set_Char_Size(face, 0, textSize_ << 6, 96, 0);
    if(error)
    {
        fprintf(stderr, "An error occued during setting character size.\n");
    }

    glGenTextures(char_textures.size(), char_textures.data());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int i = 0; i<text_.length(); i++)
    {
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(face, text_[i]);
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if(error)
        {
            std::cerr << "Load glyph error: " << error << std::endl;
            continue;
        }
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if(error)
        {
            std::cerr << "Render glyph error: " << error << std::endl;
            continue;
        }

        FT_GlyphSlot slot = face->glyph;
        const FT_Bitmap& bitmap = slot->bitmap;

        glBindTexture(GL_TEXTURE_2D, char_textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0,
                     GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

        vertex_buffer_data[i][0][0] = x + slot->bitmap_left;
        vertex_buffer_data[i][0][1] = y + slot->bitmap_top - bitmap.rows;
        vertex_buffer_data[i][1][0] = x + slot->bitmap_left + bitmap.width;
        vertex_buffer_data[i][1][1] = y + slot->bitmap_top - bitmap.rows;
        vertex_buffer_data[i][2][0] = x + slot->bitmap_left + bitmap.width;
        vertex_buffer_data[i][2][1] = y + slot->bitmap_top;
        vertex_buffer_data[i][3][0] = x + slot->bitmap_left;
        vertex_buffer_data[i][3][1] = y + slot->bitmap_top;
        x += static_cast<float>(slot->advance.x) / 64;
        y -= static_cast<float>(slot->advance.y) / 64;
    }

    constexpr GLfloat uv_buffer_data[] = {0.f, 1.f, 1.f, 1.f,
                                          1.f, 0.f, 0.f, 0.f};

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data,
                 GL_STATIC_DRAW);
}

void RenderText::render(float x, float y)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(programID);

    decltype(vertex_buffer_data) buffer_data(vertex_buffer_data);

    for(int i = 0; i < buffer_data.size(); i++)
    {
        for(int j = 0; j < 4; j++)
        {
            buffer_data[i][j][0] += x;
            buffer_data[i][j][1] += y;
        }
    }
    glGenBuffers(text_.length(), vertexbuffer.data());
    for(int i = 0; i < text_.length(); i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data[i]), &buffer_data[i],
                     GL_STATIC_DRAW);

        glEnableVertexAttribArray(vertexPositionID);
        glVertexAttribPointer(vertexPositionID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glBindTexture(GL_TEXTURE_2D, char_textures[i]);

        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glUniform1i(textureID, 0);
        glDrawArrays(GL_QUADS, 0, 4);

        glDisableVertexAttribArray(vertexPositionID);
        glDisableVertexAttribArray(vertexUVID);
    }
    glDeleteBuffers(text_.length(), vertexbuffer.data());
    glDisable(GL_BLEND);
}

RenderText::~RenderText()
{
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    glDeleteTextures(char_textures.size(), char_textures.data());
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
}
