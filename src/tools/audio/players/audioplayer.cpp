#include "audioplayer.h"

constexpr int MAX_VOLUME = 100;

/// Normalize the volume to a float in range [0,1]
auto AudioPlayer::normalizeVolume(int volume) -> float
{
    volume = std::clamp(volume, 0, MAX_VOLUME);
    return static_cast<float>(volume) / MAX_VOLUME;
}
