#include "CollisionEvents.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"

//===========================================================
//      FACTORY REGISTER
//===========================================================
REGISTER_FACTORY(DefaultCollisionEvent);
REGISTER_FACTORY(RampCollisionEvent);
REGISTER_FACTORY(ItemBoxCollisionEvent);
REGISTER_FACTORY(TrapCollisionEvent);

//===========================================================
//      DEFAULT COLLISION EVENT
//===========================================================
void DefaultCollisionEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;
    std::string name = "DefaultCollision"+std::to_string(event.Component->getGameObject().getId());

    if(player >= 0 && player <= 4 && !audioFMOD->existsSoundEvent(name)){

        if(player == 3) player = 1;
        else if(player == 1) player = 3;

        //Load sound event
        if(GlobalVariables::getInstance().getLanguage() == 0)
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterEN"], &soundInstance);
        else
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (int)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)5);

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
    }

}


//===========================================================
//      RAMP COLLISION EVENT
//===========================================================
void RampCollisionEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;
    std::string name = "RampCollision"+std::to_string(event.Component->getGameObject().getId());

    if(player >= 0 && player <= 4 && !audioFMOD->existsSoundEvent(name)){

        if(player == 3) player = 1;
        else if(player == 1) player = 3;

        //Load sound event
        if(GlobalVariables::getInstance().getLanguage() == 0)
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterEN"], &soundInstance);
        else
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)9);

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

    }

}

//===========================================================
//      ITEM BOX COLLISION EVENT
//===========================================================
void ItemBoxCollisionEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;
    std::string name = "ItemBoxCollision"+std::to_string(event.Component->getGameObject().getId());

    if(player >= 0 && player <= 4 && !audioFMOD->existsSoundEvent(name)){
        
        if(player == 3) player = 1;
        else if(player == 1) player = 3;

        //Load sound event
        if(GlobalVariables::getInstance().getLanguage() == 0)
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterEN"], &soundInstance);
        else
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)1);

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
        audioFMOD->insertSoundEvent(name,(ISoundEvent*)this);

    }

}

//===========================================================
//      TRAP COLLISION EVENT
//===========================================================
void TrapCollisionEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;
    std::string name = "TrapCollision"+std::to_string(event.Component->getGameObject().getId());

    if(player >= 0 && player <= 4 && !audioFMOD->existsSoundEvent(name)){

        if(player == 3) player = 1;
        else if(player == 1) player = 3;

        //Load sound event
        if(GlobalVariables::getInstance().getLanguage() == 0)
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterEN"], &soundInstance);
        else
            FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)4);

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
        audioFMOD->insertSoundEvent(name,(ISoundEvent*)this);

    }

}