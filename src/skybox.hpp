#pragma once

#include "cubemap.hpp"
#include "mesh.hpp"

class Skybox {
public:
    Skybox(const std::array<std::string, 6>& faces);
    ~Skybox();

    void render();

private:
    GLuint vao, vbo, ebo;
    GLint indexCount;
    Cubemap cubemap;
};