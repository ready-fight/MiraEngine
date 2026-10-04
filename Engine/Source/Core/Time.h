#pragma once

#include <chrono>

namespace MiraEngine
{
    class Time
    {
    public:
        static void Tick();

        static float DeltaTime();
        static float TotalTime();

    private:
        static std::chrono::steady_clock::time_point s_previousTime;
        static float s_deltaTime;
        static float s_totalTime;
    };
}