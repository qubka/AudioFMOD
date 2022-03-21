#include "skybox.hpp"
#include "opengl.hpp"

Skybox::Skybox(const std::array<std::string, 6>& faces) : cubemap{faces} {
    std::vector<glm::vec3> vertices {
        //front
        {-1.f, -1.f,  1.f},
        { 1.f, -1.f,  1.f},
        { 1.f,  1.f,  1.f},
        {-1.f,  1.f,  1.f},

        //right
        { 1.f, -1.f,  1.f},
        { 1.f, -1.f, -1.f},
        { 1.f,  1.f, -1.f},
        { 1.f,  1.f,  1.f},

        //back
        { 1.f, -1.f, -1.f},
        {-1.f, -1.f, -1.f},
        {-1.f,  1.f, -1.f},
        { 1.f,  1.f, -1.f},

        //left
        {-1.f, -1.f, -1.f},
        {-1.f, -1.f,  1.f},
        {-1.f,  1.f,  1.f},
        {-1.f,  1.f, -1.f},

        //top
        {-1.f,  1.f,  1.f},
        { 1.f,  1.f,  1.f},
        { 1.f,  1.f, -1.f},
        {-1.f,  1.f, -1.f},

        //bottom
        {-1.f, -1.f, -1.f},
        { 1.f, -1.f, -1.f},
        { 1.f, -1.f,  1.f},
        {-1.f, -1.f,  1.f},
    };

    std::vector<uint32_t> indices {
        2,  1,  0,		3,  2,  0,  //front
        6,  5,  4,		7,  6,  4,  //right
        10, 9,  8,		11, 10, 8,  //back
        14, 13, 12,		15, 14, 12, //left
        18, 17, 16,		19, 18, 16, //upper
        22, 21, 20,		23, 22, 20  //bottom
    };
    indexCount = static_cast<int>(indices.size());

    glCall(glGenVertexArrays, 1, &vao);
    glCall(glGenBuffers, 1, &vbo);
    glCall(glGenBuffers, 1, &ebo);

    glCall(glBindVertexArray, vao);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);
    glCall(glBufferData, GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, ebo);
    glCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glCall(glEnableVertexAttribArray, 0);
    glCall(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
    glCall(glBindVertexArray, 0);
}

Skybox::~Skybox() {
    glCall(glDeleteVertexArrays, 1, &vao);
    glCall(glDeleteBuffers, 1, &vbo);
    glCall(glDeleteBuffers, 1, &ebo);
}

void Skybox::render() {
    cubemap.bind();
    glCall(glBindVertexArray, vao);
    glCall(glDrawElements, GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (GLvoid*)0);
    glCall(glBindVertexArray, 0);
    cubemap.unbind();
}
