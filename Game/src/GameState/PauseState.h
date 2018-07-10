#pragma once

#include "IGameState.h"
#include "../Game.h"

class PauseState : public IGameState {

public: 

    //Constructor
    PauseState (){ type = IGameState::PAUSE; };

    //Destructor
    virtual ~PauseState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static PauseState& getInstance() {
        static PauseState instance;
        return instance;
    };

private:

    //==============================================================
    // Private data
    //==============================================================
    //Input manager
    InputManager* inputManager;
    //Event manager
    EventManager* eventManager;
    //Render manager
    RenderManager* renderManager;
    //Object manager
    ObjectManager* objectManager;
    //Audio Manager
    AudioManager* audioManager;

};