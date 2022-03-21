#pragma once

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texture)
        : position{position}, normal{normal}, texture{texture} {}
};
