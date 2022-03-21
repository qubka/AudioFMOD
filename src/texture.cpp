#include "texture.hpp"
#include "image.hpp"
#include "opengl.hpp"

Texture::Texture(const std::string& path, bool linear, bool clamp, const glm::vec2& scale) : path{path}, scale{scale} {
    Image image{path};
    GLenum internalFormat = GL_R8, dataFormat = GL_RED;
    switch (image.channels) {
        case 3:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
    }

    glCall(glGenTextures, 1, &textureId);
    glCall(glBindTexture, GL_TEXTURE_2D, textureId);
    glCall(glTexImage2D, GL_TEXTURE_2D, 0, internalFormat, image.width, image.height, 0, dataFormat, GL_UNSIGNED_BYTE, image.pixels);

    if (linear) {
        glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if (clamp) {
        glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    glCall(glGenerateMipmap, GL_TEXTURE_2D);

    glCall(glBindTexture, GL_TEXTURE_2D, 0);
}

Texture::Texture(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t data[] = {r, g, b};

    glCall(glGenTextures, 1, &textureId);
    glCall(glBindTexture, GL_TEXTURE_2D, textureId);
    glCall(glPixelStorei, GL_UNPACK_ALIGNMENT, 1);

    glCall(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glCall(glBindTexture, GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glCall(glDeleteTextures, 1, &textureId);
}

void Texture::bind(int i) const {
    glCall(glActiveTexture, GL_TEXTURE0 + i);
    glCall(glBindTexture, GL_TEXTURE_2D, textureId);
}

void Texture::unbind() const {
    glCall(glBindTexture, GL_TEXTURE_2D, 0);
}