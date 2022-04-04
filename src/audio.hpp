#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>

struct DSPUserdata {
    float volume;
};

class Audio {
public:
    Audio();
    ~Audio();

    bool loadSound(const std::string& filename);
    bool playSound(const glm::vec3& position, float volume = 1.0f);
    bool toggleSound();
    bool setSoundPositionAndVelocity(const glm::vec3& position, const glm::vec3& velocity);

    bool loadMusicStream(const std::string& filename);
    bool playMusicStream();
    bool toggleMusicStream();

    bool changeMusicFilter();
    bool createGeometry(const glm::vec2& extent, const glm::vec3& position, const glm::quat& rotation);

    void update(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up);

private:
    FMOD::System* system;

    FMOD::Sound* musicSound;
    FMOD::Channel* musicChannel;

    FMOD::Sound* spatialSound;
    FMOD::Channel* soundChannel;

    FMOD::DSP* dsppitch;
    FMOD::DSP* dsplowpass;
    FMOD::DSP* dsphighpass;
    FMOD::DSP* dspecho;
    FMOD::DSP* dspflange;
    FMOD::DSP* dspdistortion;
    FMOD::DSP* dspchorus;
    FMOD::DSP* dspparameq;
    FMOD::DSP* dspcustom;

    FMOD::Geometry* geometry;

    bool lowpassActive{ false };
    bool highpassActive{ false };
    bool echoActive{ false };
    bool flangeActive{ false };
    bool distortionActive{ false };
    bool chorusActive{ false };
    bool parameqActive{ false };
    bool customActive{ false };

    DSPUserdata data{ 0.5f };
    float pan{ 0.0f };
    float volume;
    float frequency;
    float pitchf{ 1.0f };
    int frequencyCount{ 0 };
    int pitchCount{ 0 };
    int tempoChange{ 0 };
    float speed;
    float pitch{ 1 };
};
