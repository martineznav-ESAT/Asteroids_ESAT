#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

#ifndef AudioLib_H
#define AudioLib_h

namespace AudioLib{

    enum SoundsType{
        ACTION,
        BANG_LARGE,
        BANG_MEDIUM,
        BANG_SMALL,
        BEAT_1,
        BEAT_2,
        EXTRA_SHIP,
        FIRE,
        UFO_BIG,
        UFO_SMALL,
        THRUST,
        TOTAL_SOUNDS
    };

    SoLoud::Soloud audio_channel;

    SoLoud::Wav *sounds;

    void Init();

    void PlaySound(SoundsType type);

    void StopSound(SoundsType type);

    void EmptyMemory();
}

#endif