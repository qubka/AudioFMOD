#pragma once

namespace poisson {
    std::vector<glm::vec2> diskSampler2D(float radius, const glm::vec2& sampleRegionSize, int numSamplesBeforeRejection = 30);
    bool IsValid(const glm::vec2& candidate, const glm::vec2& sampleRegionSize, float cellSize, float radius, const std::vector<glm::vec2>& points, const std::vector<std::vector<int>>& grid);
};