#pragma once

#include "IGameState.h"
#include "../Game.h"

class SelectionState : public IGameState {

public: 

    //Constructor
    SelectionState (){ type = IGameState::SELECTION; };

    //Destructor
    virtual ~SelectionState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    bool getLoadState() {   return load;    }

    //Static class getter
    static SelectionState& getInstance() {
        static SelectionState instance;
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

    GameObject::Pointer camera;

    bool load = false;
    int currPlayer = 0;
    glm::vec3 cameraPositions[6];   //[0],[1] initial target/position
                                    //[2],[3] final target/position
                                    //[4],[5] current target/position
    float currTime;                 //Current cummulated time
    float maxTime = 1;
    bool ongoing = false;

    int initialized = false;

};