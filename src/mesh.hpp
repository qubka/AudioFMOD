#pragma once

#include "vertex.hpp"

class Shader;
class Texture;

class Mesh {
public:
    Mesh(std::vector<Vertex>&& vertices, GLenum mode = GL_TRIANGLES);
    Mesh(std::vector<Vertex>&& vertices, const std::shared_ptr<Texture>& texture, GLenum mode = GL_TRIANGLES);
    Mesh(std::vector<Vertex>&& vertices, std::vector<std::shared_ptr<Texture>>&& textures, GLenum mode = GL_TRIANGLES);
    Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, const std::shared_ptr<Texture>& texture, GLenum mode = GL_TRIANGLES);
    Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::vector<std::shared_ptr<Texture>>&& textures, GLenum mode = GL_TRIANGLES);
    ~Mesh();

    void render(const std::unique_ptr<Shader>& shader) const;
    void render() const; // no textures

private:
    GLuint vao, vbo, ebo;
    GLenum mode;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    void initMesh();

    friend class Model;
};
