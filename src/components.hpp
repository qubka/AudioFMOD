#pragma once

struct TransformComponent {
    glm::vec3 translation{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f};

    operator glm::mat4() const {
        glm::mat4 m{1.0f};
        return glm::translate(m, translation)
               * glm::mat4_cast(rotation)
               * glm::scale(m, scale);
    };
};

class Mesh;
struct MeshComponent {
    std::shared_ptr<Mesh> mesh;
    float radius{ 1.0f };

    std::shared_ptr<Mesh>& operator()() { return mesh; }
    const std::shared_ptr<Mesh>& operator()() const { return mesh; }
};

class Model;
struct ModelComponent {
    std::shared_ptr<Model> model;
    float radius{ 1.0f };

    std::shared_ptr<Model>& operator()() { return model; }
    const std::shared_ptr<Model>& operator()() const { return model; }
};

struct ShipComponent {
    glm::vec3 velocity{0.0f};
    float speed{1.0f};
    float maxSpeed{0.01f};
    int path{0};
};