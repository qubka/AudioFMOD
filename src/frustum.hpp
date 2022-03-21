#pragma once

/// @brief Create frustum from matrix
/// if extracted from projection matrix only, planes will be in eye-space
/// if extracted from view*projection, planes will be in world space
/// if extracted from model*view*projection planes will be in model space
class Frustum {
public:
    enum side { LEFT = 0, RIGHT = 1, TOP = 2, BOTTOM = 3, BACK = 4, FRONT = 5 };
    std::array<glm::vec4, 6> planes;

    void update(const glm::mat4& mat) {
        // Based on: Fast Extraction of Viewing Frustum Planes from the WorldView-Projection Matrix
        //       by: Gil Gribb and Klaus Hartmann
        /// @link https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
        planes[TOP] = {mat[0][3] - mat[0][1], mat[1][3] - mat[1][1], mat[2][3] - mat[2][1], mat[3][3] - mat[3][1]};
        planes[BOTTOM] = {mat[0][3] + mat[0][1], mat[1][3] + mat[1][1], mat[2][3] + mat[2][1], mat[3][3] + mat[3][1]};
        planes[LEFT] = {mat[0][3] + mat[0][0], mat[1][3] + mat[1][0], mat[2][3] + mat[2][0], mat[3][3] + mat[3][0]};
        planes[RIGHT] = {mat[0][3] - mat[0][0], mat[1][3] - mat[1][0], mat[2][3] - mat[2][0], mat[3][3] - mat[3][0]};
        planes[BACK] = {mat[0][3] + mat[0][2], mat[1][3] + mat[1][2], mat[2][3] + mat[2][2], mat[3][3] + mat[3][2]};
        planes[FRONT] = {mat[0][3] - mat[0][2], mat[1][3] - mat[1][2], mat[2][3] - mat[2][2], mat[3][3] - mat[3][2]};

        /*for (auto& plane : planes) {
            float length = sqrtf(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
            plane /= length;
        }*/

        for (auto& plane : planes) {
            plane = glm::normalize(plane);
        }
    }

    bool checkSphere(const glm::vec3& pos, float radius) const {
        for (auto& plane : planes) {
            if ((plane.x * pos.x) + (plane.y * pos.y) + (plane.z * pos.z) + plane.w <= -radius) {
                return false;
            }
        }
        return true;
    }
};