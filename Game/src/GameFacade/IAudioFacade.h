#pragma once

#include <string>
#include <glm/ext.hpp>
#include "../GameObject/GameObject.h"

class IAudioFacade{

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    IAudioFacade() {}

    //Destructor
    virtual ~IAudioFacade() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openAudioEngine(int lang) = 0;

    //Updates window info in the engine
    virtual void update() = 0;

    //Closes engine window
    virtual void closeAudioEngine() = 0;

    //Load and unload backs
    virtual void loadBanks() = 0;
    virtual void unloadBanks() = 0;

    //==============================================================
    // Audio Related functions
    //==============================================================

    //Sets the basic volume of the game. Expected value between 0 and 1;
    virtual void setVolume(float vol) = 0;

    //Sets the 3D position of the listener
    virtual void setListenerPosition() = 0;

    //Getters
    virtual float getVolume() = 0;

    virtual void stop(std::string name) = 0;

    //Sets the gameObject to be the listener
    void setListener(GameObject& newListener) {
        listener = &newListener;
    }

    //Returns the gameObject that is listening
    GameObject* getListener() {
        return listener;
    }

protected:

    //Listener
    GameObject* listener = nullptr;

};