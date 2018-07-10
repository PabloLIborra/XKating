#include "PostMatchState.h"

void PostMatchState::init() {
    //Bind all managers that are going to be used
    eventManager  = &EventManager::getInstance();
    inputManager  = &InputManager::getInstance();
    renderManager = &RenderManager::getInstance();
    objectManager = &ObjectManager::getInstance();
    audioManager = &AudioManager::getInstance();
    physicsManager = &PhysicsManager::getInstance();
    waypointManager = &WaypointManager::getInstance();
    aiManager = &AIManager::getInstance();
    sensorManager = &SensorManager::getInstance();

    GlobalVariables::getInstance().setIgnoreInput(true);
    Game::getInstance().setAccumulatedTime(0);

    GameObject::Pointer player = ObjectManager::getInstance().getObject(25000);

    //add AI to the player
    auto listNodes = WaypointManager::getInstance().getWaypoints();
    (WaypointManager::getInstance().createPathPlanningComponent(player, listNodes)).get()->init();
    (AIManager::getInstance().createAIDrivingComponent(*player.get())).get()->init();
    (AIManager::getInstance().createAIBattleComponent(*player.get())).get()->init();
    (SensorManager::getInstance().createVSensorComponent(*player.get(), 55.f, 0.0, 100.f, 10.f)).get()->init(); 
    (SensorManager::getInstance().createMSensorComponent(*player.get(), 30.f, 0.0)).get()->init();

    schedulingClock = new Clock();
    schedulingClock->init();

    remainingTime = 20.0;

    EventData data;
    data.Component      = objectManager->getObject(25000).get()->getComponent<ObjectRenderComponent>();

    EventManager::getInstance().addEvent(Event {EventType::Music_Finish, data});


}

void PostMatchState::update(float &accumulatedTime) {

    if(remainingTime < 0) {
        
        close();

        EventManager::getInstance().addEvent(Event {EventType::Game_Menu});

        Game::getInstance().setState(IGameState::stateType::INTRO);
    }

    //Update input manager
    inputManager->update();

    physicsManager->update(accumulatedTime);

    aiManager->update(accumulatedTime);

    waypointManager->update(accumulatedTime);

    sensorManager->update();
    
    //Update audio manager
    audioManager->update();

    //Event manager has to be the last to be updated
    eventManager->update();

    //AI Scheduling and timing
    double timePassed = schedulingClock->getElapsedTime();
    schedulingClock->restart();
    aiManager->updateScheduling(timePassed, 0.03);

    interpolate(accumulatedTime);

    //Sets if the game keeps running or not
    Game::getInstance().setStay(objectManager->getGameRunning());

    remainingTime -= accumulatedTime;
}

void PostMatchState::interpolate(float &accumulatedTime) {
    //Interpolate positions
    physicsManager->interpolate(accumulatedTime, 0.03);

    //Update each position in Render Manager
    for(unsigned int i=0; i<physicsManager->getMovingCharacterList().size(); ++i){
        //Interpolate every moving object
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(
                physicsManager->getMovingCharacterList()[i].moveComponent.get()->getGameObject().getId(), 
                physicsManager->getMovingCharacterList()[i].moveComponent.get()->getGameObject().getTransformData()
        );
    }

    //Update camera position
    renderManager->getRenderFacade()->interpolateCamera(accumulatedTime, 0.03);
}

void PostMatchState::draw() {

    renderManager->draw();
}

void PostMatchState::close() {
    
    delete schedulingClock;
    GameObject::Pointer player = ObjectManager::getInstance().getObject(25000);
    player.get()->deleteComponent<PathPlanningComponent>();
    player.get()->deleteComponent<AIDrivingComponent>();
    player.get()->deleteComponent<AIBattleComponent>();
    player.get()->deleteComponent<VSensorComponent>();
    player.get()->deleteComponent<MSensorComponent>();
}