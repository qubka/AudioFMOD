#include "catmullrom.hpp"
#include "common.hpp"

CatmullRom::CatmullRom() {
}

CatmullRom::~CatmullRom() {
}

// Determine lengths along the control points, which is the set of control points forming the closed curve
void CatmullRom::computeLengthsAlongControlPoints() {
    int M = static_cast<int>(controlPoints.size());

    float accumulatedLength = 0.0f;
    distances.push_back(accumulatedLength);
    for (int i = 1; i < M; i++) {
        accumulatedLength += glm::distance(controlPoints[i - 1], controlPoints[i]);
        distances.push_back(accumulatedLength);
    }

    // Get the distance from the last point to the first
    accumulatedLength += glm::distance(controlPoints[M - 1], controlPoints[0]);
    distances.push_back(accumulatedLength);
}

// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CatmullRom::sample(float d, glm::vec3& p, glm::vec3& n) const {
    if (d < 0)
        return false;

    int M = static_cast<int>(controlPoints.size());
    if (M == 0)
        return false;

    float totalLength = distances.back();

    // The the current length along the control polygon; handle the case where we've looped around the track
    float length = d - static_cast<int>(d / totalLength) * totalLength;

    // Find the current segment
    int j = -1;
    for (int i = 0; i < static_cast<int>(distances.size()) - 1; i++) {
        if (length >= distances[i] && length < distances[i + 1]) {
            j = i; // found it!
            break;
        }
    }

    if (j == -1)
        return false;

    // Interpolate on current segment -- get t
    float segmentLength = distances[j + 1] - distances[j];
    float t = (length - distances[j]) / segmentLength;

    // Get the indices of the four points along the control polygon for the current segment
    int prev = ((j - 1) + M) % M;
    int cur = j;
    int next = (j + 1) % M;
    int nextNext = (j + 2) % M;

    auto& v1 = controlPoints[prev];
    auto& v2 = controlPoints[cur];
    auto& v3 = controlPoints[next];
    auto& v4 = controlPoints[nextNext];

    // Interpolate to get the point
    p = glm::catmullRom(v1, v2, v3, v4, t);
    n = glm::normalize(v2 - v1);

    return true;
}

// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CatmullRom::uniformlySampleControlPoints(std::vector<glm::vec3>&& points, int numSamples) {
    controlPoints = points;

    uniformSample(numSamples);

    // Repeat once more for truly equidistant points
    controlPoints = centrelinePoints;
    centrelinePoints.clear();
    centrelineNormals.clear();
    distances.clear();

    uniformSample(numSamples);
}

// Sample a set of control points
void CatmullRom::uniformSample(int numSamples) {
    // Compute the lengths of each segment along the control polygon, and the total length
    computeLengthsAlongControlPoints();
    float totalLength = distances.back();

    // The spacing will be based on the control polygon
    float spacing = totalLength / numSamples;

    // Call PointAt to sample the spline, to generate the points
    glm::vec3 p, n;
    for (int i = 0; i < numSamples; i++) {
        if (sample(i * spacing, p, n)) {
            centrelinePoints.push_back(p);
            centrelineNormals.push_back(n);
        }
    }
}