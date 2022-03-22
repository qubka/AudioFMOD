#include "audio.hpp"
#include "common.hpp"
#include "input.hpp"

#define FMOD_ERROR(result) if (result != FMOD_OK) std::cerr << "***ERROR*** (" << __FILE__ << ": " << __LINE__ << ") " << FMOD_ErrorString(result) << std::endl;

Audio::Audio() {
    // Create an FMOD system
    auto result = FMOD::System_Create(&system);
    FMOD_ERROR(result);
    if (result != FMOD_OK)
        return;

    // Initialise the system
    result = system->init(32, FMOD_INIT_NORMAL, nullptr);
    FMOD_ERROR(result);
    if (result != FMOD_OK)
        return;
}

Audio::~Audio() {

}

void Audio::update(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up) {
    changeMusicFilter();

    system->set3DListenerAttributes(0, glm::fmod_vector(position), glm::fmod_vector(velocity), glm::fmod_vector(forward), glm::fmod_vector(up));
    system->update();
}

bool Audio::loadSound(const std::string& filename, bool looped, bool spatial) {
    if (sounds.find(filename) != sounds.end())
        return false;

    FMOD::Sound* sound;
    auto type = spatial ? FMOD_3D : FMOD_2D;
    auto loop = looped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    auto result = system->createSound(filename.c_str(), type | loop, nullptr, &sound);
    FMOD_ERROR(result);
    if (result != FMOD_OK)
        return false;

    sounds.emplace(filename, sound);

    return true;
}

bool Audio::playSound(const std::string& filename) {
    auto sound = sounds.find(filename);
    if (sound == sounds.end())
        return false;

    auto result = system->playSound(sound->second, nullptr, false, nullptr);
    FMOD_ERROR(result);
    if (result != FMOD_OK)
        return false;

    return true;
}

// DSP callback
FMOD_RESULT F_CALLBACK DSPCallback(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int* outchannels) {
    //auto* thisdsp = reinterpret_cast<FMOD::DSP*>(dsp_state->instance);

    float dt = 0.01f;

    for (int samp = 0; samp < length; samp++) {
        for (int chan = 0; chan < *outchannels; chan++) {
            /*
                This DSP filter just halves the volume!
                Input is modified, and sent to output.
            */
            dt += 0.01f;
            outbuffer[(samp * *outchannels) + chan] = inbuffer[(samp * inchannels) + chan] * std::min(sinf(dt), cosf(dt));
        }
    }

    return FMOD_OK;
}

bool Audio::loadMusicStream(const std::string& filename) {
    auto result = system->createStream(filename.c_str(), FMOD_LOOP_NORMAL, nullptr, &music);
    FMOD_ERROR(result);

    if (result != FMOD_OK)
        return false;
    
    //Create the DSP effects.
    result = system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsppitch);
    FMOD_ERROR(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &dsplowpass);
    FMOD_ERROR(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &dsphighpass);
    FMOD_ERROR(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspecho);
    FMOD_ERROR(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspflange);
    FMOD_ERROR(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dspdistortion);
    FMOD_ERROR(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dspchorus);
    FMOD_ERROR(result);
    result = system->createDSPByType(FMOD_DSP_TYPE_PARAMEQ, &dspparameq);
    FMOD_ERROR(result);

    // Create the Custom DSP effect
    {
        FMOD_DSP_DESCRIPTION dspdesc;
        memset(&dspdesc, 0, sizeof(dspdesc));
        std::strcpy(dspdesc.name, "DSP Custom Filter");

        dspdesc.numinputbuffers = 1;
        dspdesc.numoutputbuffers = 1;
        dspdesc.read = DSPCallback;

        result = system->createDSP(&dspdesc, &dspcustom);
        FMOD_ERROR(result);
    }

    if (result != FMOD_OK)
        return false;

    return true;
}

bool Audio::playMusicStream() {
    auto result = system->playSound(music, nullptr, false, &musicChannel);
    FMOD_ERROR(result);

    if (result != FMOD_OK)
        return false;

    return true;
}

/*bool Audio::playSound3D(const std::string& filename, const glm::vec3& pos, float volume) {
    auto sound = sounds.find(filename);
    if (sound == sounds.end())
        return false;

    auto result = fmodSystem->playSound(sound->second, nullptr, false, &soundChannel);
    FmodErrorCheck(result);
    result = soundChannel->setVolumeRamp(false); // For fixing popping noise at low volume.
    FmodErrorCheck(result);
    result = soundChannel->set3DAttributes(glm::fmod_vector(pos), glm::fmod_vector(glm::vec3{0}));
    FmodErrorCheck(result);
    result = soundChannel->setPaused(false);
    FmodErrorCheck(result);
    result = soundChannel->setVolume(volume);
    FmodErrorCheck(result);

    if (result != FMOD_OK)
        return false;

    return true;
}*/

void Audio::changeMusicFilter() {
    FMOD_RESULT result;
    
    if (Input::GetKeyDown(GLFW_KEY_Q)) {
        bool paused;

        musicChannel->getPaused(&paused);
        FMOD_ERROR(result);

        paused = !paused;

        result = musicChannel->setPaused(paused);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_1)) {
        //	Play Sound From Left Speakers
        musicChannel->getVolume(&volume);
        result = musicChannel->setMixLevelsOutput(1, 0, 0, 0, 0, 0, 0, 0);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_2)) {
        //	Play Sound From Right Speakers
        musicChannel->getVolume(&volume);
        std::cout << "Volume: " << volume << std::endl;
        result = musicChannel->setMixLevelsOutput(0, 1, 0, 0, 0, 0, 0, 0);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_3)) {
        //	Play Sound From Both Speakers
        musicChannel->getVolume(&volume);
        result = musicChannel->setMixLevelsOutput(1, 1, 0, 0, 0, 0, 0, 0);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_4)) {
        //	Decrmenent Tempo
        //if(frequencyCount > -12){
        //if (tempoChange + pitchCount > -12 && std::abs(pitchCount) < 12 && tempoChange > -12) {
        if (pitchf < 1.98f) {
            frequencyCount--;
            tempoChange++;
            std::cout << "Pitch Count: " << pitchCount << std::endl;
            std::cout << "Tempo Count: " << tempoChange << std::endl;

            musicChannel->getFrequency(&frequency);
            std::cout << "Z Initial Frequency: " << frequency << std::endl;

            float base = 2.0f;

            std::cout << "inc by: " << std::pow(base,(-1.0f/12.0f)) << std::endl;
            float newTempo = frequency * std::pow(base, (-1.0f / 12.0f));

            //float pitchf = 1.0f;

            //if (pitchCount == 0 && tempoChange == 0) {
            //	pitchf = 1.0f;
            //}
            if (pitchf == 1.0f) {
                pitchf = std::pow(1.059f, std::abs(tempoChange + pitchCount));
            } else if (pitchf > 1.0f) {
                pitchf = std::pow(1.059f, std::abs(tempoChange + pitchCount));
            } else if (pitchf < 1.0f) {
                pitchf = std::pow(.9438f, std::abs(tempoChange + pitchCount));
            }

            std::cout << "pitchf: " << pitchf << std::endl;
            std::cout << "Frequency: " << newTempo << std::endl;

            result = musicChannel->removeDSP(dsppitch);
            FMOD_ERROR(result);

            result = musicChannel->addDSP(0, dsppitch);
            FMOD_ERROR(result);

            musicChannel->setFrequency(newTempo);

            result = dsppitch->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitchf);
            FMOD_ERROR(result);

        } else {
            std::cout << "Reached Min Tempo" << std::endl;
        }
    }
    else if (Input::GetKeyDown(GLFW_KEY_5)) {
        //	Incrmenent Tempo
        if (pitchf > 0.52f) {
            frequencyCount++;
            tempoChange--;
            std::cout << "Pitch Count: " << pitchCount << std::endl;
            std::cout << "Tempo Count: " << tempoChange << std::endl;

            musicChannel->getFrequency(&frequency);
            std::cout << "Z Initial Frequency: " << frequency << std::endl;

            float base = 2.0f;
            float newTempo = frequency * std::pow(base, (1.0f / 12.0f));

            if (pitchf == 1.0f) {
                std::cout << "tempo one" << std::endl;
                pitchf = std::pow(.9438f, std::abs(tempoChange + pitchCount));
            } else if (pitchf > 1.0f) {
                std::cout << "tempo inc" << std::endl;
                pitchf = std::pow(1.059f, std::abs(tempoChange + pitchCount));
            } else if (pitchf < 1.0f) {
                std::cout << "tempo dec" << std::endl;
                pitchf = std::pow(.9438f, std::abs(tempoChange + pitchCount));
            }

            std::cout << "pitchf: " << pitchf << std::endl;
            std::cout << "Frequency: " << newTempo << std::endl;

            result = musicChannel->removeDSP(dsppitch);
            FMOD_ERROR(result);

            result = musicChannel->addDSP(0, dsppitch);
            FMOD_ERROR(result);

            musicChannel->setFrequency(newTempo);

            result = dsppitch->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitchf);
            FMOD_ERROR(result);

        } else {
            std::cout << "Reached Max Tempo" << std::endl;
        }
    }
    else if (Input::GetKeyDown(GLFW_KEY_EQUAL)) {
        //	Increment Volume
        musicChannel->getVolume(&volume);
        volume += 0.1f;
        if (volume > 1) {
            volume = 1;
        }
        std::cout << "Volume: " << volume << std::endl;
        musicChannel->setVolume(volume);
    }
    else if (Input::GetKeyDown(GLFW_KEY_MINUS)) {
        //	Decrement Volume
        musicChannel->getVolume(&volume);
        volume -= 0.1f;
        if (volume < 0) {
            volume = 0;
        }
        std::cout << "Volume: " << volume << std::endl;
        musicChannel->setVolume(volume);
    }
    else if (Input::GetKeyDown(GLFW_KEY_LEFT_BRACKET)) {
        // Pan Left
        pan -= 0.1f;
        if (pan < -1) {
            pan = -1;
        }
        std::cout << "Pan: " << pan << std::endl;
        musicChannel->setPan(pan);
    }
    else if (Input::GetKeyDown(GLFW_KEY_RIGHT_BRACKET)) {
        //	Pan Right
        pan += 0.1f;
        if (pan > 1) {
            pan = 1;
        }
        std::cout << "Pan: " << pan << std::endl;
        musicChannel->setPan(pan);
    }
    else if (Input::GetKeyDown(GLFW_KEY_N)) {
        //	Decremental Pitch
        if (pitchf > 0.52f) {
            frequencyCount--;
            pitchCount--;
            std::cout << "Pitch Count: " << pitchCount << std::endl;
            std::cout << "Tempo Count: " << tempoChange << std::endl;

            if (pitchf == 1.0f) {
                pitchf = std::pow(.9438f, std::abs(tempoChange + pitchCount));
            } else if (pitchf > 1.0f) {
                pitchf = std::pow(1.059f, tempoChange + pitchCount);
            } else if (pitchf < 1.0f) {
                pitchf = std::pow(.9438f, std::abs(tempoChange + pitchCount));
            }

            std::cout << "Pitch: " << pitchf << std::endl;

            result = musicChannel->removeDSP(dsppitch);
            FMOD_ERROR(result);

            result = musicChannel->addDSP(0, dsppitch);
            FMOD_ERROR(result);

            result = dsppitch->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitchf);
            FMOD_ERROR(result);
        } else {
            std::cout << "Reached Min Pitch" << std::endl;
        }
    }
    else if (Input::GetKeyDown(GLFW_KEY_M)) {
        //	Incremental Pitch
        if (pitchf < 1.98f) {
            frequencyCount++;
            pitchCount++;
            std::cout << "Pitch Count: " << pitchCount << std::endl;
            std::cout << "Tempo Count: " << tempoChange << std::endl;

            if (pitchf == 1.0f) {
                pitchf = std::pow(1.059f, std::abs(tempoChange + pitchCount));
            } else if (pitchf > 1.0f) {
                pitchf = std::pow(1.059f, tempoChange + pitchCount);
            } else if (pitchf < 1.0f) {
                pitchf = std::pow(.9438f, std::abs(tempoChange + pitchCount));
            }

            std::cout << "Pitch: " << pitchf << std::endl;

            result = musicChannel->removeDSP(dsppitch);
            FMOD_ERROR(result);

            result = musicChannel->addDSP(0, dsppitch);
            FMOD_ERROR(result);

            result = dsppitch->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitchf);
            FMOD_ERROR(result);
        } else {
            std::cout << "Reached Max Pitch" << std::endl;
        }
    }
    else if (Input::GetKeyDown(GLFW_KEY_R)) {
        lowpassActive = !lowpassActive;

        if (!lowpassActive) {
            result = musicChannel->removeDSP(dsplowpass);
            FMOD_ERROR(result);
        } else {
            result = musicChannel->addDSP(0, dsplowpass);
            FMOD_ERROR(result);
        }

        std::cout << "Lowpass Filter: " << (lowpassActive ? "ON" : "OFF") << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_T)) {
        highpassActive = !highpassActive;

        if (!highpassActive) {
            result = musicChannel->removeDSP(dsphighpass);
            FMOD_ERROR(result);
        } else {
            result = musicChannel->addDSP(0, dsphighpass);
            FMOD_ERROR(result);
        }

        std::cout << "Highpass Filter: " << (highpassActive ? "ON" : "OFF") << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_Y)) {
        echoActive = !echoActive;

        if (!echoActive) {
            result = musicChannel->removeDSP(dspecho);
            FMOD_ERROR(result);
        } else {
            result = musicChannel->addDSP(0, dspecho);
            FMOD_ERROR(result);

            result = dspecho->setParameterFloat(FMOD_DSP_ECHO_DELAY, 50.0f);
            FMOD_ERROR(result);
        }

        std::cout << "Echo Filter: " << (echoActive ? "ON" : "OFF") << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_U)) {
        flangeActive = !flangeActive;

        if (!flangeActive) {
            result = musicChannel->removeDSP(dspflange);
            FMOD_ERROR(result);
        } else {
            result = musicChannel->addDSP(0, dspflange);
            FMOD_ERROR(result);
        }

        std::cout << "Flange Filter: " << (flangeActive ? "ON" : "OFF") << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_I)) {
        distortionActive = !distortionActive;

        if (!distortionActive) {
            result = musicChannel->removeDSP(dspdistortion);
            FMOD_ERROR(result);
        } else {
            result = musicChannel->addDSP(0, dspdistortion);
            FMOD_ERROR(result);

            result = dspdistortion->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, 0.8f);
            FMOD_ERROR(result);
        }

        std::cout << "Distortion Filter: " << (distortionActive ? "ON" : "OFF") << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_O)) {
        chorusActive = !chorusActive;

        if (!chorusActive) {
            result = musicChannel->removeDSP(dspchorus);
            FMOD_ERROR(result);
        } else {
            result = musicChannel->addDSP(0, dspchorus);
            FMOD_ERROR(result);
        }

        std::cout << "Chorus Filter: " << (chorusActive ? "ON" : "OFF") << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_P)) {
        parameqActive = !parameqActive;

        if (!parameqActive) {
            result = musicChannel->removeDSP(dspparameq);
            FMOD_ERROR(result);
        } else {
            result = musicChannel->addDSP(0, dspparameq);
            FMOD_ERROR(result);

            result = dspparameq->setParameterFloat(FMOD_DSP_PARAMEQ_CENTER, 5000.0f);
            FMOD_ERROR(result);
            result = dspparameq->setParameterFloat(FMOD_DSP_PARAMEQ_GAIN, 0.0f);
            FMOD_ERROR(result);
        }

        std::cout << "Parameq Filter: " << (parameqActive ? "ON" : "OFF") << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_C)) {
        customActive = !customActive;

        if (!customActive) {
            result = musicChannel->removeDSP(dspcustom);
            FMOD_ERROR(result);
        } else {
            result = musicChannel->addDSP(0, dspcustom);
            FMOD_ERROR(result);
        }

        std::cout << "Custom Filter: " << (customActive ? "ON" : "OFF") << std::endl;
    }
}