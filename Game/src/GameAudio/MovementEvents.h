#pragma once

#include "ISoundEvent.h"
#include "../GameFacade/AudioFMOD.h"

//===========================================================
//      ON SPEED EVENT
//===========================================================
class OnSpeedEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON SLIDE EVENT
//===========================================================
class OnSlideEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON JUMP EVENT
//===========================================================
class OnJumpEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON COLLISION EVENT
//===========================================================
class OnCollisionEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON TRAP EVENT
//===========================================================
class OnTrapEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON SHELL EVENT
//===========================================================
class OnShellEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON PLAYER SELECT EVENT
//===========================================================
class OnPlayerSelectEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};