#pragma once

#include <fmod_studio.hpp>
#include "fmod.hpp"
#include "fmod_errors.h"
#include <glm/ext.hpp>
#include <iostream>
#include <map>
#include <string>
#include <memory>
#include "../GameEvent/EventManager.h"

class AudioFMOD;

class ISoundEvent;

//Macro for registering new Factories
#define REGISTER_FACTORY(newClass) \
    class newClass##Factory : public SoundFactory { \
    public: \
        newClass##Factory() { \
            ISoundEvent::registerFactory(ISoundEvent::getFactoryMap(), #newClass, this); \
        } \
        virtual ISoundEvent* createSound() { \
            return new newClass(); \
        } \
    }; \
    static newClass##Factory newClass##FactoryInstance;

//Define a factory for instancing sound events
class SoundFactory {

public:

    virtual ISoundEvent* createSound() = 0;

};

//Typedef Map Factory 
typedef std::map<std::string, SoundFactory*> FactoryMap;
    
//ISoundEventClass
class ISoundEvent {
    
public:

    //Constructor
    ISoundEvent() {};

    //Destrcutor 
    virtual ~ISoundEvent() {};
    
   //Starts playing the event
    virtual void start();
    
    //Stops the event inmediately
    virtual void stop();

    //Pauses the reproduction of the event
    virtual void pause();
    
    //Sets the volume of the event
    virtual void setVolume(float vol);

    //Sets the volume
    virtual void sVolume(float vol) { volume = vol;}
    virtual float getVolume()       { return volume;}
    
    //Sets the gain of the event
    virtual void setGain(float gain);
    
    //Sets the 3D position of the event
    virtual void setPosition(glm::vec3 pos);
    
    //Checks if the event is playing
    virtual bool isPlaying();

    //Register a new factory for sounds
    static void registerFactory (FactoryMap& map, const std::string& name, SoundFactory* factory) {
        map[name] = factory;
    }

    //Create a new factory map 
    //(we create it inside a function in order to control the instation order of the static map variable,
    //if we didn't do so, we would get a segmentation fault error in the map)
    static FactoryMap& getFactoryMap() {
        static FactoryMap map;
        return map;
    }

    //Create a new instance of a sound
    static ISoundEvent* createSound(FactoryMap map, const std::string& name) {
        return map[name]->createSound();
    }

    //Create a new instance out of an event
    virtual void initalizeSound(AudioFMOD*, const EventData&) = 0;

    //Emitter getter
    std::weak_ptr<IComponent> getEmitter() { return emitter; }
    
    
protected:

    FMOD_STUDIO_EVENTINSTANCE* soundInstance = NULL;
    std::weak_ptr<IComponent> emitter;
    float volume = 1.0;

};