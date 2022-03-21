#pragma once

#include "freefont.hpp"

struct Glyph {
    glm::vec2 advance;
    glm::vec2 size;
    glm::vec2 bearing;
    glm::vec2 uv;
};

class Font {
public:
    Font(const FontFace& face, int size);
    ~Font();

    void bind() const;
    void unbind() const;

private:
    GLuint textureId;
    int width;
    int height;
    int metrics;
    std::map<unsigned char, Glyph> glyphs;

    friend class TextMesh;
};
