#pragma once

//#include <assimp/types.h>

class FMOD_VECTOR;

namespace glm {
    /*/GLM_INLINE glm::vec3 vec3_cast(const aiVector3D& v) { return {v.x, v.y, v.z}; }
    GLM_INLINE glm::vec3 vec3_cast(const aiColor3D& c) { return {c.r, c.g, c.b}; }
    GLM_INLINE glm::vec2 vec2_cast(const aiVector3D& v) { return {v.x, v.y}; }
    GLM_INLINE glm::quat quat_cast(const aiQuaternion& q) { return {q.w, q.x, q.y, q.z}; }
    GLM_INLINE glm::mat4 mat4_cast(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }
    GLM_INLINE glm::mat4 mat4_cast(const aiMatrix3x3& m) { return glm::transpose(glm::make_mat3(&m.a1)); }
    */
    GLM_INLINE const FMOD_VECTOR* fmod_vector(const glm::vec3& v) { return reinterpret_cast<const FMOD_VECTOR*>(&v); }
}

namespace vec3 {
    GLM_INLINE glm::vec3 right = glm::vec3{1,0,0};
    GLM_INLINE glm::vec3 left = glm::vec3{-1,0,0};
    GLM_INLINE glm::vec3 up = glm::vec3{0,1,0};
    GLM_INLINE glm::vec3 down = glm::vec3{0,-1,0};
    GLM_INLINE glm::vec3 forward = glm::vec3{0,0,1};
    GLM_INLINE glm::vec3 back = glm::vec3{0,0,-1};

    GLM_INLINE glm::vec3 zero = glm::vec3{0};
    GLM_INLINE glm::vec3 nan = glm::vec3{NAN};
}

namespace vec2 {
    GLM_INLINE glm::vec2 right = glm::vec2{1,0};
    GLM_INLINE glm::vec2 left = glm::vec2{-1,0};
    GLM_INLINE glm::vec2 up = glm::vec2{0,1};
    GLM_INLINE glm::vec2 down = glm::vec2{0,-1};

    GLM_INLINE glm::vec2 zero = glm::vec2{0};
    GLM_INLINE glm::vec2 nan = glm::vec2{NAN};
}