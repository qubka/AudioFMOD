#include "poissonsampling.hpp"
#include "random.hpp"

std::vector<glm::vec2> poisson::diskSampler2D(float radius, const glm::vec2& sampleRegionSize, int numSamplesBeforeRejection) {
    float cellSize = radius / sqrtf(2);

    std::vector<std::vector<int>> grid(std::ceil(sampleRegionSize.x / cellSize), std::vector<int>(std::ceil(sampleRegionSize.y /cellSize), 0));
    std::vector<glm::vec2> points;
    std::vector<glm::vec2> spawnPoints;

    spawnPoints.push_back(sampleRegionSize / 2.0f);
    while (!spawnPoints.empty()) {
        int spawnIndex = Random::IntRange(0, static_cast<int>(spawnPoints.size()) - 1);
        const glm::vec2& spawnCentre = spawnPoints[spawnIndex];
        bool candidateAccepted = false;

        for (int i = 0; i < numSamplesBeforeRejection; i++) {
            float angle = Random::FloatValue() * 2.0f * M_PI;
            glm::vec2 dir{ sinf(angle), cosf(angle) };
            glm::vec2 candidate{ spawnCentre + dir * Random::FloatRange(radius, 2 * radius) };

            if (IsValid(candidate, sampleRegionSize, cellSize, radius, points, grid)) {
                points.push_back(candidate);
                spawnPoints.push_back(candidate);
                grid[static_cast<int>(candidate.x / cellSize)][static_cast<int>(candidate.y / cellSize)] = points.size();
                candidateAccepted = true;
                break;
            }
        }
        if (!candidateAccepted) {
            spawnPoints.erase(spawnPoints.begin() + spawnIndex);
        }
    }

    return points;
}

bool poisson::IsValid(const glm::vec2& candidate, const glm::vec2& sampleRegionSize, float cellSize, float radius, const std::vector<glm::vec2>& points, const std::vector<std::vector<int>>& grid) {
    if (candidate.x >= 0 && candidate.x < sampleRegionSize.x && candidate.y >= 0 && candidate.y < sampleRegionSize.y) {
        int cellX = static_cast<int>(candidate.x / cellSize);
        int cellY = static_cast<int>(candidate.y / cellSize);
        int searchStartX = std::max(0, cellX - 2);
        int searchEndX = std::min(cellX + 2, static_cast<int>(grid.size()) - 1);
        int searchStartY = std::max(0, cellY - 2);
        int searchEndY = std::min(cellY + 2, static_cast<int>(grid[0].size()) - 1);

        for (int x = searchStartX; x <= searchEndX; x++) {
            for (int y = searchStartY; y <= searchEndY; y++) {
                int pointIndex = grid[x][y]-1;
                if (pointIndex != -1) {
                    float sqrDst = glm::length2(candidate - points[pointIndex]);
                    if (sqrDst < radius*radius) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}
