#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>

class Audio {
public:
    Audio();
    ~Audio();

    bool loadSound(const std::string& filename, bool looped, bool spatial);
    bool playSound(const std::string& filename);
    //bool playSound3D(const std::string& filename, const glm::vec3& pos, float volume);

    bool loadMusicStream(const std::string& filename);
    bool playMusicStream();
    void toggleMusicFilter();

    void update(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up);

    void changeMusicFilter();

private:
    FMOD::System* system;

    FMOD::Sound* music;

    FMOD::DSP* dsppitch;

    FMOD::DSP* dsplowpass;
    bool lowpassActive{ false };
    FMOD::DSP* dsphighpass;
    bool highpassActive{ false };
    FMOD::DSP* dspecho;
    bool echoActive{ false };
    FMOD::DSP* dspflange;
    bool flangeActive{ false };
    FMOD::DSP* dspdistortion;
    bool distortionActive{ false };
    FMOD::DSP* dspchorus;
    bool chorusActive{ false };
    FMOD::DSP* dspparameq;
    bool parameqActive{ false };
    FMOD::DSP* dspcustom;
    bool customActive{ false };

    float pan{ 0.0f };
    float volume;
    float frequency;
    float pitchf{ 1.0f };
    int frequencyCount{ 0 };
    int pitchCount{ 0 };
    int tempoChange{ 0 };
    float speed;
    float pitch{ 1 };

    //bool musicFilterActive;
    FMOD::Channel* musicChannel;
    FMOD::DSP* musicDSPHead;
    FMOD::DSP* musicDSPHeadInput;

    std::unordered_map<std::string, FMOD::Sound*> sounds;
    FMOD::Channel* soundChannel;
};
