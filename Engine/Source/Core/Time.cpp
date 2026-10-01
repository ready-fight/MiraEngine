#include "Core/Time.h"

#include <algorithm>

namespace Mira
{
    std::chrono::steady_clock::time_point Time::s_previousTime =
        std::chrono::steady_clock::now();

    float Time::s_deltaTime = 0.0f;
    float Time::s_totalTime = 0.0f;

    void Time::Tick()
    {
        const auto currentTime = std::chrono::steady_clock::now();

        s_deltaTime = std::min(
            std::chrono::duration<float>(
                currentTime - s_previousTime
            ).count(),
            0.1f
        );

        s_previousTime = currentTime;
        s_totalTime += s_deltaTime;
    }

    float Time::DeltaTime()
    {
        return s_deltaTime;
    }

    float Time::TotalTime()
    {
        return s_totalTime;
    }
}