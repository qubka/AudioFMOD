#include "audio.hpp"

#include <fmod_errors.h>

Audio::Audio() {}

Audio::~Audio() {}

bool Audio::initialise() {
    // Create an FMOD system
    result = FMOD::System_Create(&fmodSystem);
    FmodErrorCheck(result);
    if (result != FMOD_OK)
        return false;

    // Initialise the system
    result = fmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);
    FmodErrorCheck(result);
    if (result != FMOD_OK)
        return false;

    return true;

}

// Load an event sound
bool Audio::loadEventSound(const std::string& filename) {
    result = fmodSystem->createSound(filename.c_str(), FMOD_LOOP_OFF, nullptr, &eventSound);
    FmodErrorCheck(result);
    if (result != FMOD_OK)
        return false;

    return true;
}

// Play an event sound
bool Audio::playEventSound() {
    result = fmodSystem->playSound(eventSound, nullptr, false, nullptr);
    FmodErrorCheck(result);
    if (result != FMOD_OK)
        return false;
    return true;
}


// Load a music stream
bool Audio::loadMusicStream(const std::string& filename) {
    result = fmodSystem->createStream(filename.c_str(), FMOD_LOOP_NORMAL, nullptr, &music);
    FmodErrorCheck(result);

    if (result != FMOD_OK)
        return false;

    // create a low-pass filter DSP object
    result = fmodSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &musicFilter);

    if (result != FMOD_OK)
        return false;

    // you can start the DSP in an inactive state
    musicFilter->setActive(false);

    return true;


}

// Play a music stream
bool Audio::playMusicStream() {
    result = fmodSystem->playSound(music, nullptr, false, &musicChannel);
    FmodErrorCheck(result);

    if (result != FMOD_OK)
        return false;

    // connecting the music filter to the music stream
    // 1) Get the DSP head and it's input
    musicChannel->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &musicDSPHead);
    musicDSPHead->getInput(0, &musicDSPHeadInput, nullptr);
    // 2) Disconnect them
    musicDSPHead->disconnectFrom(musicDSPHeadInput);
    // 3) Add input to the music head from the filter
    result = musicDSPHead->addInput(musicFilter);
    FmodErrorCheck(result);

    if (result != FMOD_OK)
        return false;

    // 4) Add input to the filter head music DSP head input
    result = musicFilter->addInput(musicDSPHeadInput);
    FmodErrorCheck(result);

    if (result != FMOD_OK)
        return false;

    // set the DSP object to be active
    musicFilter->setActive(true);
    // initially set the cutoff to a high value
    musicFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 22000);
    // this state is used for toggling
    musicFilterActive = false;

    return true;
}

// Check for error
void Audio::FmodErrorCheck(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        const char* errorString = FMOD_ErrorString(result);
        // MessageBox(NULL, errorString, "FMOD Error", MB_OK);
        // Warning: error message commented out -- if headphones not plugged into computer in lab, error occurs
    }
}

void Audio::update() {
    fmodSystem->update();
}

void Audio::toggleMusicFilter() {
    // called externally from Game::ProcessEvents
    // toggle the effect on/off
    musicFilterActive = !musicFilterActive;
    if (musicFilterActive) {
        // set the parameter to a low value
        musicFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 700);
    } else {
        // set the parameter to a high value
        // you could also use m_musicFilter->setBypass(true) instead...
        musicFilter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 22000);
    }
}