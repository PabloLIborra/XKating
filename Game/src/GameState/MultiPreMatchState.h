#pragma once

#include "IGameState.h"
#include "../Game.h"

class MultiPreMatchState : public IGameState {

public: 

    //Constructor
    MultiPreMatchState (){ type = IGameState::MULTIPREMATCH; };

    //Destructor
    virtual ~MultiPreMatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static MultiPreMatchState& getInstance() {
        static MultiPreMatchState instance;
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

    glm::vec3 cameraPositions[6];   //[0],[1] initial target/position
                                    //[2],[3] final target/position
                                    //[4],[5] current target/position

    float currTime;                 //Current cummulated time
    float maxTime = 4;              //Max time per camera plane
    int stateCounter = -1;          // 0 - first camera movement
                                    // 1 - second camera movement
                                    // 2 - third camera movement
                                    // 3 - countdown
                                    // 4 - change state

};