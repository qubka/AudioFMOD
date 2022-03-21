#pragma once

class Font;

class TextMesh {
public:
    TextMesh();
    ~TextMesh();

    void render(const std::unique_ptr<Font>& font, const std::string& text, float x, float y, float scale) const;

private:
    GLuint vao, vbo;
};