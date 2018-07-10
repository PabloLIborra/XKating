#include "MovementEvents.h"
#include "../GlobalVariables.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"
//===========================================================
//      FACTORY REGISTER
//===========================================================
REGISTER_FACTORY(OnSpeedEvent);
REGISTER_FACTORY(OnSlideEvent);
REGISTER_FACTORY(OnJumpEvent);
REGISTER_FACTORY(OnCollisionEvent);
REGISTER_FACTORY(OnTrapEvent);
REGISTER_FACTORY(OnShellEvent);
REGISTER_FACTORY(OnPlayerSelectEvent);
//===========================================================
//      ON SPEED EVENT
//===========================================================
void OnSpeedEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnSpeedEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Skate"], &soundInstance);

        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "speed", event.grade);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.4);
    }

}

//===========================================================
//      ON SLIDE EVENT
//===========================================================
void OnSlideEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnSlideEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Slide"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.7);
    }

}

//===========================================================
//      ON JUMP EVENT
//===========================================================
void OnJumpEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnJumpEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Jump"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.7);
    }

}

//===========================================================
//      ON COLLISION EVENT
//===========================================================
void OnCollisionEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnCollisionEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Collision"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(0.6);
    }

}

//===========================================================
//      ON TRAP EVENT
//===========================================================
void OnTrapEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnTrapEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Items_ItemTrap"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(1);
    }

}

//===========================================================
//      ON SHELL EVENT
//===========================================================
void OnShellEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    std::string name = "OnShellEvent";

    if(!audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["Movement/Items_ItemShell"], &soundInstance);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);
        FMOD_Studio_EventInstance_SetVolume(soundInstance, 1);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(1);
    }

}

//===========================================================
//      ON PLAYER SELECT EVENT
//===========================================================
void OnPlayerSelectEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {
    
    int player = event.Id;
    std::string name = "OnPlayerSelectEvent";

    if(player >= 0 && player <= 4 && !audioFMOD->existsSoundEvent(name)){

        //Load sound event
        if(GlobalVariables::getInstance().getLanguage() == 0)
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterEN"], &soundInstance);
        else
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (int)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)0);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);

        sVolume(1);
    }

}