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

    void update(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up);

    void changeMusicFilter();

    static float filterValue;

private:
    FMOD::System* system;
    FMOD::Sound* music;

    FMOD::DSP* dsppitch;
    FMOD::DSP* dsplowpass;
    FMOD::DSP* dsphighpass;
    FMOD::DSP* dspecho;
    FMOD::DSP* dspflange;
    FMOD::DSP* dspdistortion;
    FMOD::DSP* dspchorus;
    FMOD::DSP* dspparameq;
    FMOD::DSP* dspcustom;

    bool lowpassActive{ false };
    bool highpassActive{ false };
    bool echoActive{ false };
    bool flangeActive{ false };
    bool distortionActive{ false };
    bool chorusActive{ false };
    bool parameqActive{ false };
    bool customActive{ false };

    //float filterValue{ 0.0f };
    float pan{ 0.0f };
    float volume;
    float frequency;
    float pitchf{ 1.0f };
    int frequencyCount{ 0 };
    int pitchCount{ 0 };
    int tempoChange{ 0 };
    float speed;
    float pitch{ 1 };

    FMOD::Channel* musicChannel;
    FMOD::DSP* musicDSPHead;
    FMOD::DSP* musicDSPHeadInput;

    std::unordered_map<std::string, FMOD::Sound*> sounds;
    FMOD::Channel* soundChannel;
};
