#include "AudioLib.h"

namespace AudioLib{

    SoLoud::Soloud audio_channel;

    SoLoud::Wav *sounds;

    void Init(){
        sounds = (SoLoud::Wav*) malloc(sizeof(SoLoud::Wav)*SoundsType::TOTAL_SOUNDS);

        audio_channel.init();

        //Loads sounds.
        (sounds+SoundsType::ACTION)->load("./Assets/Audio/action.wav");
        (sounds+SoundsType::BANG_LARGE)->load("./Assets/Audio/bangLarge.wav");
        (sounds+SoundsType::BANG_MEDIUM)->load("./Assets/Audio/bangMedium.wav");
        (sounds+SoundsType::BANG_SMALL)->load("./Assets/Audio/bangSmall.wav");
        (sounds+SoundsType::BEAT_1)->load("./Assets/Audio/beat1.wav");
        (sounds+SoundsType::BEAT_2)->load("./Assets/Audio/beat2.wav");
        (sounds+SoundsType::EXTRA_SHIP)->load("./Assets/Audio/extraShip.wav");
        (sounds+SoundsType::FIRE)->load("./Assets/Audio/fire.wav");
        (sounds+SoundsType::UFO_BIG)->load("./Assets/Audio/saucerBig.wav");
        (sounds+SoundsType::UFO_SMALL)->load("./Assets/Audio/saucerSmall.wav");
        (sounds+SoundsType::THRUST)->load("./Assets/Audio/thrust.wav");
    }

    
    void StopSound(SoundsType type){
        (sounds+type)->stop(); 
    }
    
    void PlaySound(SoundsType type){
        audio_channel.play(*(sounds+type)); 
    }

    void EmptyMemory(){
        free(sounds);
        audio_channel.deinit();
    }
}
