#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include "textrenderer.h"

RenderText::RenderText(std::u32string text, unsigned int text_size, float x, float y):text_(std::move(text))
{
    int error;
    vertex_buffer_data.reserve(text_.length());
    vertexbuffer.reserve(text_.length());
    char_textures.reserve(text_.length());
    error = FT_Init_FreeType(&library); 
    if(error)
    {
        throw std::runtime_error("An error occurred during FreeType library initialization.");
    }

    error = FT_New_Face(
        library, "/usr/share/fonts/noto/NotoSansCJK-Regular.ttc", 0, &face);
    if(error)
    {
        fprintf(stderr,
                "An error occured during loading fonts from memory %d.\n",
                error);
    }

    error = FT_Set_Char_Size(face, 0, text_size << 6, 96, 0);
    if(error)
    {
        fprintf(stderr, "An error occued during setting character size.\n");
    }

    glGenTextures(char_textures.size(), char_textures.data());

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int n = 0; text[n] != '\0'; n++)
    {
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(face, text[n]);
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

        glBindTexture(GL_TEXTURE_2D, char_textures[n]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0,
                     GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

        vertex_buffer_data[n][0][0] = x + slot->bitmap_left;
        vertex_buffer_data[n][0][1] = y + slot->bitmap_top - bitmap.rows;
        vertex_buffer_data[n][1][0] = x + slot->bitmap_left + bitmap.width;
        vertex_buffer_data[n][1][1] = y + slot->bitmap_top - bitmap.rows;
        vertex_buffer_data[n][2][0] = x + slot->bitmap_left + bitmap.width;
        vertex_buffer_data[n][2][1] = y + slot->bitmap_top;
        vertex_buffer_data[n][3][0] = x + slot->bitmap_left;
        vertex_buffer_data[n][3][1] = y + slot->bitmap_top;
        x += static_cast<float>(slot->advance.x) / 64;
        y -= static_cast<float>(slot->advance.y) / 64;
    }

    constexpr GLfloat uv_buffer_data[] = {0.0f, 1.0f, 1.0f, 1.0f,
                                          1.0f, 0.0f, 0.0f, 0.0f};

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data,
                 GL_STATIC_DRAW);

}

void RenderText::render(float x, float y, GLuint vertexPositionID, GLuint vertexUVID, GLint TextureID)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(int i = 0; i < text_.length(); i++)
    {
        for(int j = 0; j < 4; j++)
        {
            vertex_buffer_data[i][j][0] += x;
            vertex_buffer_data[i][j][1] += y;
        }
        glGenBuffers(text_.length(), vertexbuffer.data());
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data[i]),
                     &vertex_buffer_data[i], GL_STATIC_DRAW);

        glEnableVertexAttribArray(vertexPositionID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[i]);
        glVertexAttribPointer(vertexPositionID, 2, GL_FLOAT,
                              GL_FALSE, 0, static_cast<GLvoid*>(0));

        glBindTexture(GL_TEXTURE_2D, char_textures[i]);

        glUniform1i(TextureID, 0);
        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));
        glDrawArrays(GL_QUADS, 0, 4);
    }
    glDisable(GL_BLEND);

    glDisableVertexAttribArray(vertexPositionID);
    glDisableVertexAttribArray(vertexUVID);
}

RenderText::~RenderText(){
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    glDeleteTextures(char_textures.size(), char_textures.data());
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(vertex_buffer_data.size(), vertexbuffer.data());

}
