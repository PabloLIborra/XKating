#pragma once

#include "ISoundEvent.h"
#include "../GameFacade/AudioFMOD.h"

//===========================================================
//      MUSIC MAIN THEME EVENTS
//===========================================================
class MusicMainThemeEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      MUSIC MENU EVENTS
//===========================================================
class MusicMenuEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};

//===========================================================
//      MUSIC FINISH EVENTS
//===========================================================
class MusicFinishEvent : public ISoundEvent {

public: 

    virtual void initalizeSound(AudioFMOD*, const EventData&);

};