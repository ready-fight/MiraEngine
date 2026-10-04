#pragma once

#include <miniaudio.h>

#include <string>

namespace MiraEngine
{
    class AudioEngine
    {
    public:
        AudioEngine();
        ~AudioEngine();

        void Play(const std::string& path);
        void SetVolume(float volume);

    private:
        ma_engine m_engine{};
    };
}