#pragma once

#include <memory>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>


#include "../GameFacade/Clock.h"

#include "../Game.h"

class Game;

class MatchState : public IGameState {

public: 

    //Constructor
    MatchState (){ type = IGameState::MATCH; };

    //Destructor
    virtual ~MatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Update managers
    void updateManagers(float dTime);

    //Drawer
    virtual void draw();

    //Interpolate function
    void interpolate(float &accumulatedTime);
    
    //Manage scheduling
    void manageScheduling();

    //Shutdown
    virtual void close();

    //Static class getter
    static MatchState& getInstance() {
        static MatchState instance;
        return instance;
    };

    //Game ratio
    void setRatio(float r)          {       ratio = r;      }
    float getRatio()                {       return ratio;   }

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

    //Update's loop time
    const float loopTime = 1.0f/30.0f;

    //Game velocity
    float ratio = 1.0;

    //Scheduling checker and clock
    Clock* schedulingClock;

    //Variable to check if the match state has already been initialized
    bool initialized = false;

};