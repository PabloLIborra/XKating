#pragma once

#include "../Game.h"

class Game;

class IntroState : public IGameState {

public: 

    //Constructor
    IntroState (){ type = IGameState::INTRO; };

    //Destructor
    virtual ~IntroState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static IntroState& getInstance() {
        static IntroState instance;
        return instance;
    };

    //Do anything that is necessary to change to match state
    void swapToMatch();

    //Do anything that is necessary to change to client match state
    void swapToClientLobby();

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

    //Update's loop time
    const float loopTime = 1.0f/30.0f;

    //Mode choosing
    bool stay = false;

    //Background image
    int32_t background;

    //Server IP
    std::string ip;

    //==============================================================
    // Private functions
    //==============================================================
    //Reads server IP value from local pc network
    void execLocalServer();

    //Reads server IP value from file
    void execFileServer();

    bool initialized = false;

};