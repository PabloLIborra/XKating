#include "MatchState.h"
#include "../Game.h"
#include "../GameFacade/IAudioFacade.h"

#include "../GlobalVariables.h"

//==============================================
// MAN FUNCTIONS
//==============================================
void MatchState::init() {

    if(initialized == false){

        audioManager    = &AudioManager::getInstance();     //Initialize true audio manager
        eventManager    = &EventManager::getInstance();     //Initilize event manager
        renderManager   = &RenderManager::getInstance();    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
        inputManager    = &InputManager::getInstance();     //Once we've initialized the renderManager, we can do the same with our inputManager
        objectManager   = &ObjectManager::getInstance();    //Initialize object manager
        physicsManager  = &PhysicsManager::getInstance();   //Initialize physics manager
        waypointManager = &WaypointManager::getInstance();  //Initialize Waypoint Manager 
        aiManager       = &AIManager::getInstance();        //Initialize AI manager
        sensorManager   = &SensorManager::getInstance();    //Initialize Sensor manager
        itemManager     = &ItemManager::getInstance();      //Initialize Sensor manager
        scoreManager    = &ScoreManager::getInstance();     //Initialize Score Manager

        //Turn scheduling off initially
        schedulingClock = new Clock();
        schedulingClock->init();

        //Set as initialized
        initialized = true;
    }

    Game::getInstance().setAccumulatedTime(0);
}

void MatchState::update(float &accumulatedTime) {
    
    float maxDTime = GlobalVariables::getInstance().getMaxDTime();
    if(accumulatedTime > maxDTime)
    {
        accumulatedTime = maxDTime;
    }
    //divide with ratio so we can accelerate or slow down the game
    accumulatedTime /= ratio;
    
    //Out of loop
    renderManager->update(accumulatedTime);

    //If time surpassed the loopTime
    if(accumulatedTime >= loopTime){
        //Update managers
        updateManagers(accumulatedTime);
        Game::getInstance().setStay(objectManager->getGameRunning());
        accumulatedTime = 0;
    }

    //Manage scheduling
    manageScheduling();

    //Always interpolate
    interpolate(accumulatedTime);

}

void MatchState::updateManagers(float dTime){
    //Input manager has to be the first to be updated
    inputManager->update();

    if(ratio != 0)
    {
        physicsManager->update(dTime);

        aiManager->update(dTime);

        waypointManager->update(dTime);

        sensorManager->update();

        itemManager->update(dTime);
        
        scoreManager->update();

        audioManager->update();
    }

    EventData data;
    data.Component      = AudioManager().getInstance().getListenerComponent();

    EventManager::getInstance().addEvent(Event {EventType::Music_MainTheme, data});

    //Event manager has to be the last to be updated
    eventManager->update();
}

void MatchState::draw() {
    renderManager->draw();
}

void MatchState::interpolate(float &accumulatedTime) {
    //Interpolate positions
    physicsManager->interpolate(accumulatedTime, loopTime);

    //Update each position in Render Manager
    for(unsigned int i=0; i<physicsManager->getMovingCharacterList().size(); ++i){
        //Interpolate every moving object
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(
                physicsManager->getMovingCharacterList()[i].moveComponent.get()->getGameObject().getId(), 
                physicsManager->getMovingCharacterList()[i].moveComponent.get()->getGameObject().getTransformData()
        );
    }

    //Update camera position
    renderManager->getRenderFacade()->interpolateCamera(accumulatedTime, loopTime);
}

void MatchState::manageScheduling()
{
    //AI Scheduling and timing
    if(aiManager->getScheduling())
    {
        double timePassed = schedulingClock->getElapsedTime();
        schedulingClock->restart();
        aiManager->updateScheduling(timePassed, loopTime);
    }
    else
    {
        //Clear if not empty the queue of events
        if(!aiManager->emptyAIQueue())
        {   
            aiManager->clearAIQueue();
        }
    }
}

void MatchState::close() {
    //Set to no initalized
    initialized = false;

    //delete variables
    delete schedulingClock;
}