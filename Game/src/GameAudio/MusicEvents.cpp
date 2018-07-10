#include "MusicEvents.h"

//===========================================================
//      FACTORY REGISTER
//===========================================================
REGISTER_FACTORY(MusicMainThemeEvent);
REGISTER_FACTORY(MusicFinishEvent);
REGISTER_FACTORY(MusicMenuEvent);

//===========================================================
//      MUSIC MAIN THEME EVENT
//===========================================================
void MusicMainThemeEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "MusicMainTheme";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Music/Music_Game"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 0.30);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.2);
    }

}

//===========================================================
//      MUSIC MENU EVENT
//===========================================================
void MusicMenuEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "MusicMenu";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Music/Music_Menu"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 0.30);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.3);
    }

}

//===========================================================
//      MUSIC VICTORY EVENT
//===========================================================
void MusicFinishEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "MusicFinish";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Music/Music_Victory"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 0.30);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.8);
    }

}