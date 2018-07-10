#pragma once

#include "ISoundEvent.h"
#include "../GameFacade/AudioFMOD.h"

//===========================================================
//      ON NEW LAP EVENT
//===========================================================
class OnNewLapEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON NEW LAP SOUND EVENT
//===========================================================
class OnNewLapSoundEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON OVERTAKE EVENT
//===========================================================
class OnOvertakeEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON OVERTAKE SOUND EVENT
//===========================================================
class OnOvertakeSoundEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      ON OVERTAKEN EVENT
//===========================================================
class OnOvertakenEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};