#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <vector>
#include <array>

class RenderText{
public:
    RenderText(std::u32string text,unsigned int text_size, float x, float y);
    ~RenderText();

    void render(float x, float y, GLuint vertexPositionID, GLuint vertexUVID, GLint TextureID);

private:
    FT_Face face;
    FT_Library library;
    std::u32string text_;
    std::vector<std::array<std::array<GLfloat, 2>, 4>> vertex_buffer_data;
    std::vector<GLuint> char_textures;
    std::vector<GLuint> vertexbuffer;
    GLuint uvbuffer;
};

#endif
