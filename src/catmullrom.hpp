#pragma once

class CatmullRom {
public:
    CatmullRom();
    ~CatmullRom();

    std::vector<glm::vec3>& getControlPoints() { return controlPoints; }
    std::vector<glm::vec3>& getCentrelinePoints() { return centrelinePoints; }
    std::vector<glm::vec3>& getCentrelineNormals() { return centrelineNormals; }

    void uniformlySampleControlPoints(std::vector<glm::vec3>&& points, int numSamples);

private:
    void computeLengthsAlongControlPoints();
    void uniformSample(int numSamples);
    bool sample(float d, glm::vec3& p, glm::vec3& n) const;

    std::vector<float> distances;
    std::vector<glm::vec3> controlPoints;
    std::vector<glm::vec3> centrelinePoints;
    std::vector<glm::vec3> centrelineNormals;
};