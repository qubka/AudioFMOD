#include "audio.hpp"
#include "common.hpp"
#include "input.hpp"

#define FMOD_ERROR_(result) if (result != FMOD_OK) { std::cerr << "***ERROR*** (" << __FILE__ << ": " << __LINE__ << ") " << FMOD_ErrorString(result) << std::endl; return; }
#define FMOD_ERROR(result) if (result != FMOD_OK) { std::cerr << "***ERROR*** (" << __FILE__ << ": " << __LINE__ << ") " << FMOD_ErrorString(result) << std::endl; return false; }

Audio::Audio() {
    // Create an FMOD system
    auto result = FMOD::System_Create(&system);
    FMOD_ERROR_(result);

    // Initialise the system
    result = system->init(32, FMOD_INIT_NORMAL, nullptr);
    FMOD_ERROR_(result);

    // Set 3D settings
    result = system->set3DSettings(1.0f, 1.0f, 1.0f);
    FMOD_ERROR_(result);
}

Audio::~Audio() {
}

void Audio::update(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up) {
    changeMusicFilter();

    // Update listener position in the world
    auto result = system->set3DListenerAttributes(0, glm::fmod_vector(position), glm::fmod_vector(velocity), glm::fmod_vector(forward), glm::fmod_vector(up));
    FMOD_ERROR_(result);
    // Update fmod system
    result = system->update();
    FMOD_ERROR_(result);
}

bool Audio::loadSound(const std::string& filename) {
    // Load an event sound
    auto result = system->createSound(filename.c_str(), FMOD_3D | FMOD_LOOP_NORMAL, nullptr, &spatialSound);
    FMOD_ERROR(result);
    return true;
}

bool Audio::playSound(const glm::vec3& position, float volume) {
    // Play an event sound
    auto result = system->playSound(spatialSound, nullptr, false, &soundChannel);
    FMOD_ERROR(result);
    result = soundChannel->setVolumeRamp(false);
    FMOD_ERROR(result);
    result = soundChannel->set3DAttributes(glm::fmod_vector(position), glm::fmod_vector(glm::vec3{0}));
    FMOD_ERROR(result);
    result = soundChannel->setPaused(true);
    FMOD_ERROR(result);
    result = soundChannel->setVolume(volume);
    FMOD_ERROR(result);

    return true;
}

bool Audio::toggleSound() {
    bool paused;

    auto result = soundChannel->getPaused(&paused);
    FMOD_ERROR(result);

    paused = !paused;

    result = soundChannel->setPaused(paused);
    FMOD_ERROR(result);

    return true;
}

bool Audio::setSoundPositionAndVelocity(const glm::vec3& position, const glm::vec3& velocity) {
    auto result = soundChannel->set3DAttributes(glm::fmod_vector(position), glm::fmod_vector(velocity));
    FMOD_ERROR(result);

    return true;
}

FMOD_RESULT F_CALLBACK DSPCallback(FMOD_DSP_STATE* dsp_state, float* inbuffer, float* outbuffer, unsigned int length, int inchannels, int* outchannels) {
    auto thisdsp = (FMOD::DSP *)dsp_state->instance;

    void* ud;
    thisdsp->getUserData(&ud);

    auto dud = static_cast<DSPUserdata*>(ud);

    for (unsigned int samp = 0; samp < length; samp++) {
        /*
            Feel free to unroll this.
        */
        for (int chan = 0; chan < *outchannels; chan++) {
            /*
                This DSP filter just halves the volume!
                Input is modified by filter value, and sent to output.
            */
            outbuffer[(samp * *outchannels) + chan] = inbuffer[(samp * inchannels) + chan] * dud->volume;
        }
    }

    return FMOD_OK;
}

bool Audio::loadMusicStream(const std::string& filename) {
    // Load a music sound
    auto result = system->createStream(filename.c_str(), FMOD_LOOP_NORMAL, nullptr, &musicSound);
    FMOD_ERROR(result);

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
    FMOD_DSP_DESCRIPTION dspdesc;
    memset(&dspdesc, 0, sizeof(dspdesc));
    std::strcpy(dspdesc.name, "DSP Custom Filter");

    dspdesc.numinputbuffers = 1;
    dspdesc.numoutputbuffers = 1;
    dspdesc.read = DSPCallback;
    dspdesc.userdata = &data;

    result = system->createDSP(&dspdesc, &dspcustom);
    FMOD_ERROR(result);

    return true;
}

bool Audio::playMusicStream() {
    // Play a music sound
    auto result = system->playSound(musicSound, nullptr, false, &musicChannel);
    FMOD_ERROR(result);

    return true;
}

bool Audio::toggleMusicStream() {
    bool paused;

    auto result = musicChannel->getPaused(&paused);
    FMOD_ERROR(result);

    paused = !paused;

    result = musicChannel->setPaused(paused);
    FMOD_ERROR(result);

    return true;
}

bool Audio::changeMusicFilter() {
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
        result = musicChannel->getVolume(&volume);
        FMOD_ERROR(result);
        result = musicChannel->setMixLevelsOutput(1, 0, 0, 0, 0, 0, 0, 0);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_2)) {
        //	Play Sound From Right Speakers
        result = musicChannel->getVolume(&volume);
        FMOD_ERROR(result);
        std::cout << "Volume: " << volume << std::endl;
        result = musicChannel->setMixLevelsOutput(0, 1, 0, 0, 0, 0, 0, 0);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_3)) {
        //	Play Sound From Both Speakers
        result = musicChannel->getVolume(&volume);
        FMOD_ERROR(result);
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

            result = musicChannel->getFrequency(&frequency);
            FMOD_ERROR(result);
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

            std::cout << "Pitch: " << pitchf << std::endl;
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

            result = musicChannel->getFrequency(&frequency);
            FMOD_ERROR(result);
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

            std::cout << "Pitch: " << pitchf << std::endl;
            std::cout << "Frequency: " << newTempo << std::endl;

            result = musicChannel->removeDSP(dsppitch);
            FMOD_ERROR(result);

            result = musicChannel->addDSP(0, dsppitch);
            FMOD_ERROR(result);

            result = musicChannel->setFrequency(newTempo);
            FMOD_ERROR(result);

            result = dsppitch->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, pitchf);
            FMOD_ERROR(result);

        } else {
            std::cout << "Reached Max Tempo" << std::endl;
        }
    }
    else if (Input::GetKeyDown(GLFW_KEY_6)) {
        toggleMusicStream();
    }
    else if (Input::GetKeyDown(GLFW_KEY_7)) {
        toggleSound();
    }
    else if (Input::GetKeyDown(GLFW_KEY_KP_ADD)) {
        data.volume += 0.1f;
        if (data.volume > 2) {
            data.volume = 2;
        }
        std::cout << "Filter Power: " << data.volume << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_KP_SUBTRACT)) {
        data.volume -= 0.1f;
        if (data.volume < 0) {
            data.volume = 0;
        }
        std::cout << "Filter Power: " << data.volume << std::endl;
    }
    else if (Input::GetKeyDown(GLFW_KEY_EQUAL)) {
        //	Increment Volume
        result = musicChannel->getVolume(&volume);
        FMOD_ERROR(result);
        volume += 0.1f;
        if (volume > 1) {
            volume = 1;
        }
        std::cout << "Volume: " << volume << std::endl;
        result = musicChannel->setVolume(volume);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_MINUS)) {
        //	Decrement Volume
        result = musicChannel->getVolume(&volume);
        FMOD_ERROR(result);
        volume -= 0.1f;
        if (volume < 0) {
            volume = 0;
        }
        std::cout << "Volume: " << volume << std::endl;
        result = musicChannel->setVolume(volume);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_LEFT_BRACKET)) {
        // Pan Left
        pan -= 0.1f;
        if (pan < -1) {
            pan = -1;
        }
        std::cout << "Pan: " << pan << std::endl;
        result = musicChannel->setPan(pan);
        FMOD_ERROR(result);
    }
    else if (Input::GetKeyDown(GLFW_KEY_RIGHT_BRACKET)) {
        //	Pan Right
        pan += 0.1f;
        if (pan > 1) {
            pan = 1;
        }
        std::cout << "Pan: " << pan << std::endl;
        result = musicChannel->setPan(pan);
        FMOD_ERROR(result);
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

    return true;
}

bool Audio::createGeometry(const glm::vec2& extent, const glm::vec3& position, const glm::quat& rotation) {
    // Create geometry to occlusion
    auto result = system->createGeometry(2, 6, &geometry);
    FMOD_ERROR(result);

    FMOD_VECTOR quad[4] = {
        { -extent.x, -extent.y, 0 },
        { -extent.x,  extent.y, 0 },
        { extent.x,  extent.y, 0 },
        { extent.x, -extent.y, 0 }
    };

    // Add polygon to object geometry
    int index = 0;
    result = geometry->addPolygon(1, 1, true, 4, quad, &index);
    FMOD_ERROR(result);

    // Using to position object geometry
    result = geometry->setPosition(glm::fmod_vector(position));
    FMOD_ERROR(result);
    // Using to rotation object geometry
    result = geometry->setRotation(glm::fmod_vector(rotation * vec3::forward), glm::fmod_vector(rotation * vec3::up));
    FMOD_ERROR(result);

    return true;
}
