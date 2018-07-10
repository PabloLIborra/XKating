#pragma once

#include "IGameState.h"
#include "../Game.h"

#include <iostream>
#include <csound/csound.hpp>
#include <string>
#include <vector>
#include <csound/csPerfThread.hpp>
#include <stdlib.h>  
#include <time.h>  
#include <stdio.h>
#include <climits>

class PreMatchState : public IGameState {

public: 

    //Constructor
    PreMatchState (){ type = IGameState::PREMATCH; };

    //Destructor
    virtual ~PreMatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static PreMatchState& getInstance() {
        static PreMatchState instance;
        return instance;
    };

private:

    struct track {
    uint8_t channel;
    uint8_t program;
    uint8_t maxTone;
    uint8_t minTone;
    uint8_t noteSize;
    float   probability;
    float*** notes;
    };

    struct trackDuration {
        float currentDuration;
        float cummulatedDuration;
        float trackSilence;
        float trackPlay;
    };

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
    
    #ifdef __linux__
    std::vector<track*> tracks;
    Csound* csound = nullptr;
    CsoundPerformanceThread* perfThread = nullptr;
    #endif

    void getNextNote(const track& newTrack, uint8_t & lastNote, float & duration);
    void playCsound(const char* path, int duration, int seed);

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