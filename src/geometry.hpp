#pragma once

class Mesh;
class Texture;

namespace geometry {
    std::shared_ptr<Mesh> cuboid(const glm::vec3& halfExtents, bool inwards, const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Mesh> sphere(uint32_t stacks, uint32_t slices, float radius, const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Mesh> quad(const glm::vec2& extent, const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Mesh> octahedron(const glm::vec3& extent, const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Mesh> tetrahedron(const glm::vec3& extent, const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Mesh> line(const std::vector<glm::vec3>& points, const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Mesh> torus(int sides, int cs_sides, float radius, float cs_radius, const std::shared_ptr<Texture>& texture);
    std::shared_ptr<Mesh> tube(const std::vector<glm::vec3>& points, float radius, int stacks, const std::shared_ptr<Texture>& texture);
}