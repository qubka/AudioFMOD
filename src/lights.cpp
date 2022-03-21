#include "lights.hpp"
#include "shader.hpp"

void DirectionalLight::submit(const std::unique_ptr<Shader>& shader, uint32_t light_index) {
    shader->setUniform("gDirectionalLight.Base.Color", color);
    shader->setUniform("gDirectionalLight.Base.AmbientIntensity", ambientIntensity);
    shader->setUniform("gDirectionalLight.Direction", glm::normalize(direction));
    shader->setUniform("gDirectionalLight.Base.DiffuseIntensity", diffuseIntensity);
}

void PointLight::submit(const std::unique_ptr<Shader>& shader, uint32_t point_light_index) {
    auto id = std::to_string(point_light_index);
    shader->setUniform("gPointLights[" + id + "].Base.Color", color);
    shader->setUniform("gPointLights[" + id + "].Base.AmbientIntensity", ambientIntensity);
    shader->setUniform("gPointLights[" + id + "].Position", position);
    shader->setUniform("gPointLights[" + id + "].Base.DiffuseIntensity", diffuseIntensity);
    shader->setUniform("gPointLights[" + id + "].Atten.Constant", attenuation.constant);
    shader->setUniform("gPointLights[" + id + "].Atten.Linear", attenuation.linear);
    shader->setUniform("gPointLights[" + id + "].Atten.Exp", attenuation.exp);
}

void SpotLight::submit(const std::unique_ptr<Shader>& shader, uint32_t spot_light_index) {
    auto id = std::to_string(spot_light_index);
    shader->setUniform("gSpotLights[" + id + "].Base.Base.Color", color);
    shader->setUniform("gSpotLights[" + id + "].Base.Base.AmbientIntensity", ambientIntensity);
    shader->setUniform("gSpotLights[" + id + "].Base.Position", position);
    shader->setUniform("gSpotLights[" + id + "].Base.Base.DiffuseIntensity", diffuseIntensity);
    shader->setUniform("gSpotLights[" + id + "].Base.Atten.Constant", attenuation.constant);
    shader->setUniform("gSpotLights[" + id + "].Base.Atten.Linear", attenuation.linear);
    shader->setUniform("gSpotLights[" + id + "].Base.Atten.Exp", attenuation.exp);
    shader->setUniform("gSpotLights[" + id + "].Direction", glm::normalize(direction));
    shader->setUniform("gSpotLights[" + id + "].Cutoff", cutoff);
}