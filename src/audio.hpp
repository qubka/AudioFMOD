#pragma once

#include <fmod.hpp>

class Audio
{
public:
    Audio();
    ~Audio();

    bool initialise();
    bool loadEventSound(const std::string& filename);
    bool playEventSound();
    bool loadMusicStream(const std::string& filename);
    bool playMusicStream();
    void toggleMusicFilter();
    void update();

private:
    static void FmodErrorCheck(FMOD_RESULT result);

    FMOD_RESULT result;
    FMOD::System* fmodSystem;    // the global variable for talking to FMOD
    FMOD::Sound* eventSound;

    FMOD::Sound* music;
    FMOD::DSP* musicFilter;
    bool musicFilterActive;
    FMOD::Channel* musicChannel;
    FMOD::DSP* musicDSPHead;
    FMOD::DSP* musicDSPHeadInput;

};
