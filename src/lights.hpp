#pragma once

class Shader;

struct BaseLight {
    glm::vec3 color{1.0f};
    float ambientIntensity{0.0f};
    float diffuseIntensity{0.0f};

    virtual void submit(const std::unique_ptr<Shader>& shader, uint32_t light_index = 0) = 0;
};

struct DirectionalLight : public BaseLight {
    glm::vec3 direction{0.0f};

    void submit(const std::unique_ptr<Shader>& shader, uint32_t light_index = 0) override;
};

struct PointLight : public BaseLight {
    glm::vec3 position{0.0f};
    struct {
        float constant{1.0f};
        float linear{0.001f};
        float exp{0.001f};
    } attenuation;

    void submit(const std::unique_ptr<Shader>& shader, uint32_t point_light_index) override;
};

struct SpotLight : public PointLight {
    glm::vec3 direction{0.0f};
    float cutoff{0.0f};

    void submit(const std::unique_ptr<Shader>& shader, uint32_t spot_light_index) override;
};