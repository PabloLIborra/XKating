#include "ISoundEvent.h"  

//Play Sound once
void ISoundEvent::start()
{
    if(soundInstance!=NULL)
    {
        FMOD_Studio_EventInstance_Start(soundInstance);
    }
}
 
//Stop sound inmediately 
void ISoundEvent::stop()
{
    FMOD_Studio_EventInstance_Stop(soundInstance, FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_IMMEDIATE);
}

//Pause the sound
void ISoundEvent::pause() 
{

}

//Change sound Volume
void ISoundEvent::setVolume(float vol)
{
    if(vol>=0 && vol<=1)
    {
        FMOD_Studio_EventInstance_SetVolume(soundInstance, vol * volume);
    }
} 

//Change sound Gain
void ISoundEvent::setGain(float gain)
{

}

//Change sound position
void ISoundEvent::setPosition(glm::vec3 pos)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.position.x = pos.x;
    attributes.position.y = pos.y;
    attributes.position.z = pos.z;

    FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
}

//Check if sound is being played
bool ISoundEvent::isPlaying()
{
    if(soundInstance!=NULL)
    {
        FMOD_STUDIO_PLAYBACK_STATE state;
        FMOD_Studio_EventInstance_GetPlaybackState(soundInstance, &state);
        if(state == FMOD_STUDIO_PLAYBACK_PLAYING || state == FMOD_STUDIO_PLAYBACK_STARTING)
        {
            return true;
        }
    }
    return false;
}