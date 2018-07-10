#pragma once

#include "ISoundEvent.h"
#include "../GameFacade/AudioFMOD.h"

//===========================================================
//      DEFAULT COLLISION EVENT
//===========================================================
class DefaultCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      RAMP COLLISION EVENT
//===========================================================
class RampCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ITEM BOX COLLISION EVENT
//===========================================================
class ItemBoxCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      Trap COLLISION EVENT
//===========================================================
class TrapCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};