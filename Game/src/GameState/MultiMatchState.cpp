#include "MultiMatchState.h"

//Additional functions
void createPlayer();

void MultiMatchState::init() {

    audioManager    = &AudioManager::getInstance();     //Initialize true audio manager
    aiManager       = &AIManager::getInstance();        //Initialize AI manager
    eventManager    = &EventManager::getInstance();     //Initialize event manager
    renderManager   = &RenderManager::getInstance();    //First we initialize renderManager, who creates a device and passes this reference to the inputManager
    inputManager    = &InputManager::getInstance();     //Once we've initialized the renderManager, we can do the same with our inputManager
    objectManager   = &ObjectManager::getInstance();    //Initialize object manager
    waypointManager = &WaypointManager::getInstance();  //Initialize Waypoint Manager
    sensorManager   = &SensorManager::getInstance();    //Initialize Sensor manager
    physicsManager  = &PhysicsManager::getInstance();   //Initialize physics manager
    itemManager     = &ItemManager::getInstance();      //Initialize Sensor manager
    scoreManager    = &ScoreManager::getInstance();     //Initialize Score Manager
    networkManager  = &NetworkManager::getInstance();   //Initialize Sensor manager
    debugManager    = &DebugManager::getInstance();     //Initialize Debug manager
    
    Game::getInstance().setAccumulatedTime(0);
}

void MultiMatchState::update(float &accumulatedTime) {
   
    float maxDTime = GlobalVariables::getInstance().getMaxDTime();
    if(accumulatedTime > maxDTime)
    {
        accumulatedTime = maxDTime;
    }

    //Render first
    renderManager->update(accumulatedTime);

    //No gelding
    inputManager->update();

    debugManager->update();

    //If time surpassed the loopTime
    if(accumulatedTime > loopTime){
        //Update managers
        updateManagers(accumulatedTime);
        Game::getInstance().setStay(objectManager->getGameRunning());
        accumulatedTime = 0;
    }

    //Do before interpolation, since it receives new positions that break the spot
    networkManager->update();

    //Always interpolate
    interpolate(accumulatedTime);
}

void MultiMatchState::updateManagers(float dTime){
    //Input manager has to be the first to be updated
    physicsManager->update(dTime);

    sensorManager->update();

    itemManager->update(dTime);
    
    scoreManager->update();

    audioManager->update();

    EventData data;
    data.Component      = AudioManager().getInstance().getListenerComponent();

    EventManager::getInstance().addEvent(Event {EventType::Music_MainTheme, data});

    //Event manager has to be the last to be updated
    eventManager->update();
}

void MultiMatchState::draw() {
    renderManager->draw();
}

void MultiMatchState::interpolate(float &accumulatedTime) {
    //Interpolate position of objects
    physicsManager->interpolate(accumulatedTime,loopTime);

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


void MultiMatchState::close() {
    //Game set to no initialized
    initialized = false;
}