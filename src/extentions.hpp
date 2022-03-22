#pragma once

namespace glm {
    glm::mat4 lookAt(const glm::mat4& matrix, const glm::vec3& target);
    glm::vec3 moveTowards(const glm::vec3& current, const glm::vec3& target, float maxDistanceDelta);
    glm::vec3 smoothDamp(const glm::vec3& current, const glm::vec3& target, glm::vec3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
    //int findClosestPoint(const glm::vec3& position, std::vector<glm::vec3>& points);
}
