#pragma once

#include "../GameObject/GameObject.h"

class IAudioFacade;

class AudioManager{

public: 

    //Constructor
    AudioManager() {}

    //Destructor
    ~AudioManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Play sound
    void play();

    //Shutdown
    void close();

    //Static class getter
    static AudioManager& getInstance();

    //Gets Audio Facade
    IAudioFacade* getAudioFacade() {
        return audioFacade;
    }

    //Gets Listener
    IComponent::Pointer getListenerComponent() {
        return listenerComponent;
    }

    //Sets Listener
    void setListenerComponent(IComponent::Pointer newListenerComponent) {
        listenerComponent = newListenerComponent;
    }

    //Listener Component Creator
    IComponent::Pointer createListenerComponent(GameObject& newGameObject);

private:

    IAudioFacade* audioFacade;

    //We store just one listener component, so we can't have more than 1
    IComponent::Pointer listenerComponent;

};