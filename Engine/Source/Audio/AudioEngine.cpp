#define MINIAUDIO_IMPLEMENTATION
#include "Audio/AudioEngine.h"

#include <stdexcept>

namespace MiraEngine
{
    AudioEngine::AudioEngine()
    {
        if (ma_engine_init(nullptr, &m_engine) != MA_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to initialize audio engine"
            );
        }
    }

    AudioEngine::~AudioEngine()
    {
        ma_engine_uninit(&m_engine);
    }

    void AudioEngine::Play(const std::string& path)
    {
        ma_engine_play_sound(
            &m_engine,
            path.c_str(),
            nullptr
        );
    }

    void AudioEngine::SetVolume(float volume)
    {
        ma_engine_set_volume(&m_engine, volume);
    }
}