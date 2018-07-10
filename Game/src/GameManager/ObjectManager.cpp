#include "ObjectManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
//An event of creation or deletion will contain the shared pointer 
//pointing to the created/deleted object
//When an object is created or deleted, they are automatically added
//or deleted to the objectMap
void objectCreated(EventData eData); 
void objectDeleted(EventData eData); 
void gameClosed(EventData eData); 

//==============================================
// OBJECT MANAGER FUNCTIONS
//============================================== 
ObjectManager& ObjectManager::getInstance() {
    static ObjectManager instance;
    return instance;
}

void ObjectManager::init() {

    //Bind functions
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Create, objectCreated});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleted});
    EventManager::getInstance().addListener(EventListener {EventType::Game_Close, gameClosed});

}

void ObjectManager::close() {

    while(!objectsMap.empty()){
        objectsMap.erase(objectsMap.begin());
    }

}

GameObject::Pointer ObjectManager::createObject(uint16_t id, GameObject::TransformationData transform) {
    //make shared pointer
    GameObject::Pointer object = std::make_shared<GameObject>(id, transform);
   
    //Creation
    ObjectManager::getInstance().addObject(object);

    return object;
}


void ObjectManager::addObject(GameObject::Pointer ptr) {
    //If object is not inserted, insert it
    if(objectsMap.find(ptr.get()->getId()) != objectsMap.end())
        std::cerr << "Couldn't insert object. ID: " << ptr.get()->getId() << " already exists." << std::endl;
    else 
        objectsMap.insert(std::pair<uint16_t,GameObject::Pointer>(ptr.get()->getId(), ptr));


}

void ObjectManager::deleteObject(uint16_t id) {

    auto obj = getObject(id);
    if(obj!=nullptr)
        obj.get()->close();

    if(objectsMap.erase(id) == 0)
        std::cerr << "Couldn't erase object. ID: " << id << " doesn't exist." << std::endl;

}

GameObject::Pointer ObjectManager::getObject(uint16_t id) {

    auto object = objectsMap.find(id);

    if(object == objectsMap.end()){
        std::cerr << "Couldn't find object. ID: " << id << " doesn't exist." << std::endl;
        return nullptr;
    }
    else{
        return object->second;
    }
}

void ObjectManager::showObjects() {
    for(auto obj : objectsMap){}
        //std::cout << obj.second.get()->getId() << std::endl;
}

void ObjectManager::initObjects() {
    for(auto obj : objectsMap)
        obj.second.get()->init();
}


//==============================================
// PLAYER CREATOR
//  pos:        
//              init pos player
//  type:       
//              0 -> Punk
//              1 -> Crocodile
//              2 -> Cyborg
//              3 -> Witch
//  move:       
//              0 -> Player (Input)
//              1 -> IA
// id:
//              id player
//============================================== 

void ObjectManager::createPlayer(GameObject::TransformationData tansform, int type, int move, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    if(type == PlayerType::punk)
    {
        createMove(createPunk(tansform, id, terrain, terrainComponent), move);
    }
    else if(type == PlayerType::witch)
    {
        createMove(createWitch(tansform, id, terrain, terrainComponent), move);
    }
    else if(type == PlayerType::cyborg)
    {
        createMove(createCyborg(tansform, id, terrain, terrainComponent), move);
    }
    else if(type == PlayerType::crocodile)
    {
        createMove(createCrocodile(tansform, id, terrain, terrainComponent), move);
    }
}

GameObject::Pointer ObjectManager::createPunk(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    //Create shared pointer of the object
    auto ob = ObjectManager::getInstance().createObject(id, tansform);
    
    //Fill needed data
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle = 0.0f;
    mData.spin = 0;
    mData.spin_inc = 0.001;
    mData.max_spin = 0.09;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f*30;
    mData.max_rotate = 3.f;
    mData.vel = 0;
    mData.max_vel = 430.0f;
    mData.real_max_vel = 430.0f;
    mData.brake_vel = 50.f;
    mData.velY = 10.f;
    mData.acc = 0;
    mData.max_acc = 110.f;
    mData.dAcc = 0.f;
    mData.brake_acc = 120.f;
    mData.player = 0;
    mData.driftAngleIncrMax = 1.f;
    mData.driftBoostTime    = 1.f;
    mData.driftSpeedBoost   = 250.f;
    mData.driftConstTime    = 0.1f;
    mData.driftDecTime      = 4.f;

    //Create components needed for its existence
    createComponents(ob, terrain, terrainComponent, mData, 0);

    return ob;

}

GameObject::Pointer ObjectManager::createWitch(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    //Create shared pointer of the object
    auto ob = ObjectManager::getInstance().createObject(id, tansform);
    
    //Fill needed data
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle = 0.0f;
    mData.spin = 0;
    mData.spin_inc = 0.001;
    mData.max_spin = 0.09;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f*30;
    mData.max_rotate = 3.f;
    mData.vel = 0;
    mData.max_vel = 430.0f;
    mData.real_max_vel = 430.0f;
    mData.brake_vel = 50.f;
    mData.velY = 10.f;
    mData.acc = 0;
    mData.max_acc = 110.f;
    mData.dAcc = 0.f;
    mData.brake_acc = 120.f;
    mData.player = 1;
    mData.driftAngleIncrMax = 1.2f;
    mData.driftAngleIncr    = 0.3f;
    mData.driftBoostTime    = 1.f;
    mData.driftSpeedBoost   = 250.f;
    mData.driftConstTime    = 0.1f;
    mData.driftDecTime      = 4.f;

    //Create components needed for its existence
    createComponents(ob, terrain, terrainComponent, mData, 3);

    return ob;

}

GameObject::Pointer ObjectManager::createCyborg(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    //Create shared pointer of the object
    auto ob = ObjectManager::getInstance().createObject(id, tansform);
    
    //Fill needed data
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle = 0.0f;
    mData.spin = 0;
    mData.spin_inc = 0.001;
    mData.max_spin = 0.09;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 0;
    mData.max_vel = 430.0f;
    mData.real_max_vel = 430.0f;
    mData.brake_vel = 50.f;
    mData.velY = 10.f;
    mData.acc = 0;
    mData.max_acc = 110.f;
    mData.dAcc = 0.f;
    mData.brake_acc = 120.f;
    mData.player = 2;
    mData.driftAngleIncrMax = 2.f;

    //Create components needed for its existence
    createComponents(ob, terrain, terrainComponent, mData, 2);

    return ob;

}

GameObject::Pointer ObjectManager::createCrocodile(GameObject::TransformationData tansform, int id, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent)
{
    //Create shared pointer of the object
    auto ob = ObjectManager::getInstance().createObject(id, tansform);
    
    //Fill needed data
    LAPAL::movementData mData;
    mData.mov = false;
    mData.jump = false;
    mData.spi = false;
    mData.angle = 0.0f;
    mData.spin = 0;
    mData.spin_inc = 0.001;
    mData.max_spin = 0.09;
    mData.brake_spin = 0.2;
    mData.rotateX = 0.f;
    mData.rotateZ = 0.f;
    mData.rotate_inc = 0.15f;
    mData.max_rotate = 3.f;
    mData.vel = 0;
    mData.max_vel = 430.0f;
    mData.real_max_vel = 430.0f;
    mData.brake_vel = 50.f;
    mData.velY = 10.f;
    mData.acc = 0;
    mData.max_acc = 110.f;
    mData.dAcc = 0.f;
    mData.brake_acc = 120.f;
    mData.player = 3;

    //Create components needed for its existence
    createComponents(ob, terrain, terrainComponent, mData, 1);

    return ob;

}

//==============================================
// Create player auxiliars
//============================================== 

void ObjectManager::createComponents(GameObject::Pointer ob, LAPAL::plane3f terrain, IComponent::Pointer terrainComponent, LAPAL::movementData mData, int player)
{
    //Create representation of the model if there is a model
    RenderManager::getInstance().createAnimationRenderComponent(*ob.get(), "", 0, player, "");
    RenderManager::getInstance().createObjectRenderComponent(*ob.get(), ObjectRenderComponent::Shape::Mesh, "");

    //Create collision component
    std::shared_ptr<IComponent> collision = PhysicsManager::getInstance().createCollisionComponent(*ob.get(), 4, 7.5, true, CollisionComponent::Type::Default);

    //Create movement component and locate it on the map
    std::shared_ptr<IComponent> move = PhysicsManager::getInstance().createMoveComponent(*ob.get(), mData, terrain, 1);
    PhysicsManager::getInstance().createMovingCharacter(move, terrainComponent, collision);

    //Character can have items and throw them
    ItemManager::getInstance().createItemHolderComponent(*ob.get());

    //Character has a score stating its position on the map
    ScoreManager::getInstance().createScoreComponent(*ob.get());
    ScoreManager::getInstance().createStartLineComponent(*ob.get());

    //Create path planning component
    auto listNodes = WaypointManager::getInstance().getWaypoints();
    WaypointManager::getInstance().createPathPlanningComponent(ob, listNodes);
}

void ObjectManager::createMove(GameObject::Pointer obj, int move)
{
    auto mData = obj->getComponent<MoveComponent>()->getMovemententData();
    //The player
    if(move == 0)
    {
        mData.isPlayer = true;
        mData.spin_inc = 0.001;
        mData.max_spin = 0.04;
        obj->getComponent<MoveComponent>()->setMovementData(mData);
        //Create input
        InputManager::getInstance().createInputComponent(*obj.get());

        //Create camera focused on player
        RenderManager::getInstance().createCameraRenderComponent(*obj.get());

        //Create audio listener
        AudioManager::getInstance().createListenerComponent(*obj.get());

        //Set player as the main
        NetworkManager::getInstance().setPlayer(obj);

        //Set player as a global variable
        GlobalVariables::getInstance().setPlayer(obj.get());
        
    }
    //The AI
    else if(move == 1)
    {
        //Instantiate both AI's and sensors
        AIManager::getInstance().createAIDrivingComponent(*obj.get());
        AIManager::getInstance().createAIBattleComponent(*obj.get());
        SensorManager::getInstance().createVSensorComponent(*obj.get(), 55.f, mData.angle, 100.f, 10.f); 
        SensorManager::getInstance().createMSensorComponent(*obj.get(), 30.f, mData.angle);
        
    }
    //Online player imitator
    else if(move == 2)
    {
        //Create remote player
        NetworkManager::getInstance().createRemotePlayerComponent(*obj.get());
    }
}

//==============================================
// DELEGATES
//============================================== 
void objectCreated(EventData eData) {
    ObjectManager::getInstance().addObject(eData.Object);
}

void objectDeleted(EventData eData) {

    ObjectManager::getInstance().deleteObject(eData.Id);

}

void gameClosed(EventData eData) {

    ObjectManager::getInstance().setGameRunning(false);

}