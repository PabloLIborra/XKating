#pragma once
#include <memory>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>

#include "../Game.h"

class Game;

class MultiMatchState : public IGameState {

public: 

    //Constructor
    MultiMatchState (){ type = IGameState::MULTIMATCH; };

    //Destructor
    virtual ~MultiMatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Update managers
    void updateManagers(float dTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static MultiMatchState& getInstance() {
        static MultiMatchState instance;
        return instance;
    };

    //Interpolate
    void interpolate(float &accumulatedTime);

private:
    
    //==============================================================
    // Private data
    //==============================================================
    //Object manager
    ObjectManager* objectManager;
    //Input manager
    InputManager* inputManager;
    //Render manager
    RenderManager* renderManager;
    //Event manager
    EventManager* eventManager;
    //Physics manager
    PhysicsManager* physicsManager;
    //Waypoint manager
    WaypointManager* waypointManager;
    //Audio Manager
    AudioManager* audioManager;
    //AI manager
    AIManager* aiManager;
    //Sensor manager
    SensorManager* sensorManager;
    //Item manager
    ItemManager* itemManager;
    //Score manager
    ScoreManager* scoreManager;
    //Network manager
    NetworkManager* networkManager;
    //Debug manager
    DebugManager* debugManager;

    //Update's loop time
    const float loopTime = 1.0f/30.0f;

    //Update's server time
    float serverTime;

    //Multimatch first time initialization
    bool initialized = false;

};