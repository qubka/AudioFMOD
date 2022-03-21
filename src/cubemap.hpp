#pragma once

class Cubemap {
public:
    Cubemap(const std::array<std::string, 6>& faces);
    ~Cubemap();

    void bind() const;
    void unbind() const;

private:
    GLuint textureId;
};