#pragma once
//Basic includes
#include <memory>
#include <iostream>
#include <vector>
#include <queue>

#include "../GlobalVariables.h"
#include "../GameFacade/Clock.h"
#include "../GameObject/IComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameEvent/EventManager.h"

//Inner components includes
#include "../GameObject/AIComponent/AIDrivingComponent.h"
#include "../GameObject/AIComponent/AIBattleComponent.h"
#include "../GameObject/AIComponent/VSensorComponent.h"
#include "../GameObject/AIComponent/MSensorComponent.h"
#include "../GameObject/AIComponent/PathPlanningComponent.h"
#include "../GameObject/AIComponent/VObject.h"

//Extra includes
#include "../GameObject/ItemComponent/IItemComponent.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"


//AI Type event
enum AIEventType {
    UPDATE_BATTLE,
    UPDATE_DRIVING_TURN,
    UPDATE_DRIVING_ACCELERATION,
    UPDATE_LOD
};

//Event to be updated by the scheduling
struct AIEvent{
    IComponent::Pointer object; //Object to be updated;
    AIEventType event;          //Event type to be updated
    double average;             //Average time of the function for processing
    double timeStamp;           //When it was generated
};

class AIManager{

public: 

    //Constructor
    AIManager();

    //Destructor
    ~AIManager() {
        delete clock;
        delete clock_scheduling;
    }

    //Initialization
    void init();

    //Update
    void update(const float dTime);

    //Update
    void updateScheduling(const float dTime, const float loopTime);

    //Shutdown
    void close();

    std::vector<IComponent::Pointer>& getAIDrivingComponentList() {
        return objectsAI;
    }

    std::vector<IComponent::Pointer>& getAIBattleComponentList()
    {
        return battleAI;
    }

    //Static class getter
    static AIManager& getInstance();


    //Component creators
    IComponent::Pointer createAIDrivingComponent(GameObject& newGameObject);
    IComponent::Pointer createAIBattleComponent(GameObject& newGameObject);

    //Update AI driving
    void updateDriving(AIDrivingComponent* aiDrivingComponent);

    //Level of Detail
    void calculateLoD(GameObject AI, float dTime);
    
    //Update AI speed depending on score
    void updateAISpeed();

    //==============================================
    // GETTERS AND SETTERS
    //==============================================
    void setScheduling(bool s){ scheduling_on = s;     }
    bool getScheduling()      { return scheduling_on;  }
    bool emptyAIQueue()       { return AIQueue.empty();}
    void clearAIQueue()       { while(!AIQueue.empty()){ AIQueue.pop();};}

private:
    std::vector<IComponent::Pointer> objectsAI; //AIDrivingComponents to be processed
    std::vector<IComponent::Pointer> battleAI;  //BattleBehaviour componentes to be processed

    bool itemLoD;                   //variable to know if we took item in this waypoint or not (only for lod)
    bool updateBattleBehaviour;     //Checker of updating battle behaviour or not

    //==============================================
    // SCHEDULING
    //==============================================
    std::queue<AIEvent> AIQueue;    //Processing queue
    double maxTimeSchedule;         //Maximum time for all processes per turn
    double accumulatedTimeSchedule; //Time accumulated between all the events
    bool scheduling_on = true;      //Sets the scheduling to on/off
    
    //Samples to measure the mean value of each average time
    int samplesBattle;
    int samplesDriving;
    int samplesLOD;

    //Average time info of each function
    double averageTimeBattle;
    double averageTimeDriving;
    double averageTimeLOD;

    //Clocks
    Clock* clock;
    Clock* clock_scheduling;
};