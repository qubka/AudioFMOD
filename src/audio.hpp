#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>

class Audio
{
public:
    Audio();
    ~Audio();

    bool loadEventSound(const std::string& filename);
    bool playEventSound();
    bool loadMusicStream(const std::string& filename);
    bool playMusicStream();
    void toggleMusicFilter();
    void update();

    void changeVolume(bool increase);

private:
    static void FmodErrorCheck(FMOD_RESULT result);

    FMOD::System* fmodSystem;    // the global variable for talking to FMOD
    FMOD::Sound* eventSound;

    FMOD::Sound* music;
    FMOD::DSP* musicFilter;
    bool musicFilterActive;
    FMOD::Channel* musicChannel;
    FMOD::DSP* musicDSPHead;
    FMOD::DSP* musicDSPHeadInput;

};
