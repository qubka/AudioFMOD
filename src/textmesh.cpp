#include "textmesh.hpp"
#include "font.hpp"
#include "opengl.hpp"

TextMesh::TextMesh() {
    glCall(glGenVertexArrays, 1, &vao);
    glCall(glGenBuffers, 1, &vbo);

    glCall(glBindVertexArray, vao);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);
    glCall(glBufferData, GL_ARRAY_BUFFER, 6 * sizeof(glm::vec4), (GLvoid*) nullptr, GL_DYNAMIC_DRAW);

    glCall(glEnableVertexAttribArray, 0);
    glCall(glVertexAttribPointer, 0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
    glCall(glBindVertexArray, 0);
}

TextMesh::~TextMesh() {
    glCall(glDeleteVertexArrays, 1, &vao);
    glCall(glDeleteBuffers, 1, &vbo);
}

void TextMesh::render(const std::unique_ptr<Font>& font, const std::string& text, float x, float y, float scale) const {
    glCall(glBindVertexArray, vao);

    float initial = x;

    for (const auto& c : text) {
        if (c == '\n') {
            x = initial;
            y -= font->metrics;
            continue;
        }

        const auto it = font->glyphs.find(c);
        const auto& glyph = it != font->glyphs.end() ? it->second : font->glyphs.at(127);

        float px = x + glyph.bearing.x * scale;
        float py = y - (glyph.size.y - glyph.bearing.y) * scale;
        float ox = glyph.size.x / font->width;
        float oy = glyph.size.y / font->height;
        float tx = glyph.uv.x;
        float ty = glyph.uv.y;

        float w = glyph.size.x * scale;
        float h = glyph.size.y * scale;

        float vertices[6][4] = {
            {
                px,
                py + h,
                tx,
                ty
            },
            {
                px,
                py,
                tx,
                ty + oy
            },
            {
                px + w,
                py,
                tx + ox,
                ty + oy
            },
            {
                px,
                py + h,
                tx,
                ty
            },
            {
                px + w,
                py,
                tx + ox,
                ty + oy
            },
            {
                px + w,
                py + h,
                tx + ox,
                ty
            }
        };

        glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);
        glCall(glBufferSubData, GL_ARRAY_BUFFER, 0, 6 * sizeof(glm::vec4), &vertices);
        glCall(glDrawArrays, GL_TRIANGLES, 0, 6);

        x += glyph.advance.x * scale;
    }

    glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
    glCall(glBindVertexArray, 0);
}

