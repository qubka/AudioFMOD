#include "cubemap.hpp"
#include "image.hpp"
#include "opengl.hpp"

Cubemap::Cubemap(const std::array<std::string, 6>& faces) {
    glCall(glGenTextures, 1, &textureId);
    glCall(glBindTexture, GL_TEXTURE_CUBE_MAP, textureId);

    for (size_t i = 0; i < faces.size(); i++) {
        Image image{ faces[i] };

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

        glCall(glTexImage2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, image.width, image.height, 0, dataFormat, GL_UNSIGNED_BYTE, image.pixels);
    }

    glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glCall(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glCall(glBindTexture, GL_TEXTURE_CUBE_MAP, 0);
}

Cubemap::~Cubemap() {
    glCall(glDeleteTextures, 1, &textureId);
}

void Cubemap::bind() const {
    glCall(glActiveTexture, GL_TEXTURE0);
    glCall(glBindTexture, GL_TEXTURE_CUBE_MAP, textureId);
}

void Cubemap::unbind() const {
    glCall(glBindTexture, GL_TEXTURE_CUBE_MAP, 0);
}