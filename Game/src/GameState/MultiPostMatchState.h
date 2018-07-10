#pragma once

#include "IGameState.h"
#include "../Game.h"

class MultiPostMatchState : public IGameState {

public: 

    //Constructor
    MultiPostMatchState (){ type = IGameState::MULTIPOSTMATCH; };

    //Destructor
    virtual ~MultiPostMatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    void interpolate(float &accumulatedTime);

    //Static class getter
    static MultiPostMatchState& getInstance() {
        static MultiPostMatchState instance;
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
    //Physics manager
    PhysicsManager* physicsManager;
    //Waypoint manager
    WaypointManager* waypointManager;
    //AI manager
    AIManager* aiManager;
    //Sensor manager
    SensorManager* sensorManager;

    Clock* schedulingClock;
    float remainingTime;

};