#pragma once

//External includes
#include <memory>
#include <vector>
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <fstream>
#include <stdio.h>

//Basic include
#include "GlobalVariables.h"
#include "GameFacade/Clock.h"

//Managers
#include "GameManager/InputManager.h"
#include "GameManager/RenderManager.h"
#include "GameManager/ObjectManager.h"
#include "GameManager/PhysicsManager.h"
#include "GameManager/WaypointManager.h"
#include "GameManager/AIManager.h"
#include "GameManager/SensorManager.h"
#include "GameManager/AudioManager.h"
#include "GameManager/ScoreManager.h"
#include "GameManager/ItemManager.h"
#include "GameManager/NetworkManager.h"
#include "GameManager/DebugManager.h"
#include "GameEvent/EventManager.h"

//State includes
#include "GameState/IntroState.h"
#include "GameState/SelectionState.h"
#include "GameState/PreMatchState.h"
#include "GameState/MatchState.h"
#include "GameState/PostMatchState.h"
#include "GameState/ClientLobbyState.h"
#include "GameState/MultiMatchState.h"
#include "GameState/MultiSelectionState.h"
#include "GameState/MultiPreMatchState.h"
#include "GameState/MultiPostMatchState.h"
#include "GameState/PauseState.h"

class IntroState;
class MultiMatchState;
class MatchState;
class ClientLobbyState;
class MultiMatchState;

class AIManager;
class ObjectManager;
class DebugManager;

class Game {

public: 

    //Constructor
    Game() {}

    //Destructor
    ~Game() {
        //Delete all new thingies
        delete clock;
    }

    //Infinite game client loop
    void Run();

    //Infinite Server side loop
    void RunServer();

    //State setter
    void setState(IGameState::stateType type);
    void setAccumulatedTime(float time) {   accumulatedTime = time;    } //Set accumulated time


    //Basic setters and getters
    void setStay(bool s)       {     stay = s;              }//Stay setter
    void setRenderEngine(int n){     renderEngine = n;      }//Engine setter
    void setInputEngine(int n) {     inputEngine = n;       }//Input setter
    void setRatio(float r)     {     ratio = r;             }//Set game ratio
    IGameState* getState()     {     return state;          }//State getter
    int getRenderEngine()      {     return renderEngine;   }//Engine getter
    int getInputEngine()       {     return inputEngine;    }//Input getter
    float getAccumulatedTime() {     return accumulatedTime;}//Get accumulated time
    float getRatio()           {     return ratio;          }//Get game ratio


    void loadMap();
    std::vector<std::string> split(const std::string& s, const char& c);

    //Static class getter
    static Game& getInstance() {
        static Game instance;
        return instance;
    };



private:

    //==============================================================
    // Private functions
    //==============================================================

    //Initialization
    void init();

    //Update client
    void update(float dTime);

    //Render draw
    void draw();

    //Shutdown
    void close();

    //==============================================================
    // Private data
    //==============================================================
    //Global Variables
    GlobalVariables* globalVariables;
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
    //Selected renderEngine
    int renderEngine;
    //Selected inputEngine
    int inputEngine;

    //Stay in the main loop
    bool stay;

    //current state
    IGameState* state;

    //Accumulate time in game
    float accumulatedTime;
    float elapsedTime;

    //Game velocity
    float ratio = 1.0f;

    //Time clock
    Clock* clock;
};
