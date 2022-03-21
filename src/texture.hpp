#pragma once

class Texture {
public:
    Texture(const std::string& path, bool linear, bool clamp, const glm::vec2& scale = glm::vec2{1.0f});
    Texture(uint8_t r, uint8_t g, uint8_t b);
    ~Texture();

    void bind(int i) const;
    void unbind() const;

    const std::string& getPath() const { return path; }
    const glm::vec2& getScale() const { return scale; }
    int getType() const { return type; }
    void setType(int i) { type = i; }

private:
    GLuint textureId;
    std::string path;
    glm::vec2 scale{ 1.0f };
    int type{ 1 }; /* aiTextureType_DIFFUSE */
};