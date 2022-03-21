#include "openal.hpp"

bool check_al_errors(const std::string& filename, uint32_t line) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error) {
            case AL_INVALID_NAME:
                std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
                break;
            case AL_OUT_OF_MEMORY:
                std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                break;
            default:
                std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

bool check_alc_errors(const std::string& filename, uint32_t line) {
    ALCenum error = alcGetError(nullptr);
    if (error != ALC_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n";
        switch (error) {
            case ALC_INVALID_ENUM:
                std::cerr << "ALC_INVALID_ENUM: an invalid enum value was passed to an OpenALC function";
                break;
            case ALC_INVALID_VALUE:
                std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenALC function";
                break;
            case ALC_INVALID_DEVICE:
                std::cerr << "ALC_INVALID_DEVICE: the invalid device handle";
                break;
            case ALC_INVALID_CONTEXT:
                std::cerr << "ALC_INVALID_CONTEXT: the invalid context handle";
                break;
            default:
                std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}