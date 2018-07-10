#include "PhysicsManager.h"
#include "../GlobalVariables.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================
void addMoveComponent(EventData eData); 
void addCollisionComponent(EventData eData); 
void addTerrainComponent(EventData eData); 
void collideRamp(EventData eData);
void collideTrap(EventData eData);
void collideBlueShell(EventData eData);
void collideRedShell(EventData eData);
void collideItemBox(EventData eData);
void collideStartLine(EventData eData);
void objectDeletedCollide(EventData eData);   
void objectDeletedMove(EventData eData);
void objectDeletedCharacter(EventData eData);

//==============================================
// PHYSICS MANAGER FUNCTIONS
//============================================== 
PhysicsManager& PhysicsManager::getInstance() {
    static PhysicsManager instance;
    return instance;
}

void PhysicsManager::init() {

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::MoveComponent_Create, addMoveComponent});
    EventManager::getInstance().addListener(EventListener {EventType::CollisionComponent_Create, addCollisionComponent});
    EventManager::getInstance().addListener(EventListener {EventType::TerrainComponent_Create, addTerrainComponent});
    EventManager::getInstance().addListener(EventListener {EventType::RampComponent_Collision, collideRamp});
    EventManager::getInstance().addListener(EventListener {EventType::ItemBoxComponent_Collision, collideItemBox});
    EventManager::getInstance().addListener(EventListener {EventType::TrapComponent_Collision, collideTrap});
    EventManager::getInstance().addListener(EventListener {EventType::BlueShellComponent_Collision, collideBlueShell});
    EventManager::getInstance().addListener(EventListener {EventType::RedShellComponent_Collision, collideRedShell});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedCollide});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedMove});
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletedCharacter});
    EventManager::getInstance().addListener(EventListener {EventType::StartLineComponent_Collision, collideStartLine});

}

void PhysicsManager::update(const float dTime) {

    //For every moving character we have, calculate collisions with the others
    for(unsigned int i=0; i<movingCharacterList.size(); ++i){

        GameObject& gameObject = movingCharacterList[i].moveComponent.get()->getGameObject();

        gameObject.setOldTransformData(gameObject.getNewTransformData());
        gameObject.setTransformData(gameObject.getNewTransformData());

        //Get components in variables
        auto ourMove = movingCharacterList[i].moveComponent;
        auto ourTerr = movingCharacterList[i].terrainComponent;
        auto ourColl = movingCharacterList[i].collisionComponent;

        //==============================================================================
        // Move character
        //==============================================================================
        ourMove->update(dTime);
        
        //==============================================================================
        // Check collisions with other objects
        //==============================================================================
        calculateObjectsCollision(ourMove, ourColl, dTime);

        //==============================================================================
        // Check collisions with terrain limits and terrain change
        //==============================================================================
        calculateTerrainCollision(movingCharacterList[i], ourMove, ourTerr, ourColl, dTime);

        gameObject.setNewTransformData(gameObject.getTransformData());
    }
}

void PhysicsManager::close() {
    moveComponentList.clear();
    collisionComponentList.clear();
    movingCharacterList.clear();  
}

void PhysicsManager::interpolate(float accumulatedTime, const float maxTime) {

    //For every moving character we have, interpolate its coordinates
    for(unsigned int i=0; i<movingCharacterList.size(); ++i){

        //Get components in variables
        GameObject& gameObject = movingCharacterList[i].moveComponent.get()->getGameObject();

        auto oldTrans = gameObject.getOldTransformData();
        auto newTrans = gameObject.getNewTransformData();

        GameObject::TransformationData fTrans;
        fTrans.position = newTrans.position - oldTrans.position;
        fTrans.rotation = newTrans.rotation - oldTrans.rotation;
        fTrans.scale = newTrans.scale - oldTrans.scale;

        GameObject::TransformationData currTrans; //Here we store current transformation

        currTrans.position = oldTrans.position + (accumulatedTime * fTrans.position)/maxTime;
        currTrans.rotation = oldTrans.rotation + (accumulatedTime * fTrans.rotation)/maxTime;
        currTrans.scale = oldTrans.scale + (accumulatedTime * fTrans.scale)/maxTime;

        gameObject.setTransformData(currTrans);

    }

}

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

void PhysicsManager::calculateObjectsCollision(std::shared_ptr<MoveComponent> move, std::shared_ptr<CollisionComponent> coll, const float dTime) {

    if(move != nullptr && coll != nullptr)
    {
        CollisionComponent* ourColl = coll.get();

        for(unsigned int j=0; j<collisionComponentList.size(); ++j) {

            std::shared_ptr<CollisionComponent> hColl = std::dynamic_pointer_cast<CollisionComponent>(collisionComponentList[j]);
            CollisionComponent* hisColl = hColl.get();
            if( hisColl != ourColl  && move->getMovemententData().invul == false) { //If the collider is different to the one of ourselves

                bool collision;

                auto nexPosition = ourColl->getGameObject().getTransformData().position + (move.get()->getMovemententData().velocity * dTime);
                
                //Depending on the shape to collide with, check collision with it
                if(hisColl->getShape() == CollisionComponent::Shape::Circle) {
                    collision = LAPAL::checkCircleCircleCollision(  nexPosition, ourColl->getRadius(), ourColl->getLength(),
                                                                    hisColl->getGameObject().getTransformData().position, hisColl->getRadius(), hisColl->getLength());
                } 
                else if(hisColl->getShape() == CollisionComponent::Shape::Rectangle) {
                    collision = LAPAL::checkCircleRectangleCollision(   hisColl->getRectangle(),  nexPosition,
                                                                        hisColl->getLength(), ourColl->getLength());
                }

                //If collision is kinetic, apply collision physics
                if(collision && hisColl->getKinetic() && 
                    coll->getType() != CollisionComponent::Type::RedShell && coll->getType() != CollisionComponent::Type::BlueShell){

                    //Get other object move component
                    auto hisMove = hisColl->getGameObject().getComponent<MoveComponent>();

                    if(hisMove == nullptr) {    //If the object doesn't have move component, it's static
                            
                        calculateStaticCollision(move, hisColl->getGameObject().getTransformData().position, dTime);

                    }
                    else {  //The object is not static
                        calculateMovingCollision(move, hisMove, dTime);
                    }

                    EventData data;
                    data.Component      = std::static_pointer_cast<IComponent>(move);
                    data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                    EventManager::getInstance().addEvent(Event {EventType::Default_Collision, data});


                }
                //If collision isn't kinetic, react with events depending on the collision type
                else if(collision && !hisColl->getKinetic()){

                    if(hisColl->getType() == CollisionComponent::Type::Ramp)
                    {
                        EventData data;
                        data.Component      = std::static_pointer_cast<IComponent>(move);
                        data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                        EventManager::getInstance().addEvent(Event {EventType::RampComponent_Collision, data});
                    }
                    else if(hisColl->getType() == CollisionComponent::Type::Trap)
                    {
                        EventData data;
                        data.Id             = hisColl->getGameObject().getId();
                        data.Component      = std::static_pointer_cast<IComponent>(move);
                        data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                        EventManager::getInstance().addEvent(Event {EventType::TrapComponent_Collision, data});
                    }
                    else if(hisColl->getType() == CollisionComponent::Type::ItemBox)
                    {
                        EventData data;
                        data.Component      = std::static_pointer_cast<IComponent>(move);
                        data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                        EventManager::getInstance().addEvent(Event {EventType::ItemBoxComponent_Collision, data});
                    }
                    else if(hisColl->getType() == CollisionComponent::Type::BlueShell && coll == ScoreManager::getInstance().getPlayers()[0].get()->getGameObject().getComponent<CollisionComponent>())
                    {
                        EventData data;
                        data.Id             = hisColl->getGameObject().getId();
                        data.Component      = std::static_pointer_cast<IComponent>(move);
                        data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                        EventManager::getInstance().addEvent(Event {EventType::BlueShellComponent_Collision, data});
                    }
                    else if(hisColl->getType() == CollisionComponent::Type::RedShell)
                    {
                        EventData data;
                        data.Id             = hisColl->getGameObject().getId();
                        data.Component      = std::static_pointer_cast<IComponent>(move);
                        data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                        EventManager::getInstance().addEvent(Event {EventType::RedShellComponent_Collision, data});
                    }
                    else if(hisColl->getType() == CollisionComponent::Type::StartLine)
                    {
                        EventData data;
                        data.Component      = std::static_pointer_cast<IComponent>(move);
                        data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                        EventManager::getInstance().addEvent(Event {EventType::StartLineComponent_Collision, data});
                    }
                }
            }
            else if(hisColl->getType() == CollisionComponent::Type::StartLine && !hisColl->getKinetic())
            {
                EventData data;
                data.Component      = std::static_pointer_cast<IComponent>(move);
                data.CollComponent  = std::static_pointer_cast<IComponent>(hColl);

                EventManager::getInstance().addEvent(Event {EventType::StartLineComponent_Collision, data});
            }
        }
    }
}

//Change velocity of the objects when it collides with another one
void PhysicsManager::calculateStaticCollision(std::shared_ptr<MoveComponent> move, LAPAL::vec3f hisPos, const float dTime) {

    MoveComponent* ourMove = move.get(); 

    auto mData = ourMove->getMovemententData();
    auto hisVel = glm::vec3(0,0,0);

    LAPAL::calculateElasticCollision(mData.velocity, ourMove->getGameObject().getTransformData().position, 
                                        ourMove->getMass(), hisVel, hisPos, 1.0);

    mData.colVel = mData.velocity / 2;
    mData.vel = 0;
    mData.velocity = glm::vec3(0,0,0);
    mData.boost = false;
    mData.coll = true;

    ourMove->setMovementData(mData);
}

//Change velocity of the objects when it collides with another one
void PhysicsManager::calculateMovingCollision(std::shared_ptr<MoveComponent> move, std::shared_ptr<MoveComponent> hMove, const float dTime) {

    MoveComponent* ourMove = move.get(); 
    MoveComponent* hisMove = hMove.get(); 

    auto mData = ourMove->getMovemententData();
    LAPAL::vec3f hisVel = hisMove->getMovemententData().velocity;

    LAPAL::calculateElasticCollision(mData.velocity, ourMove->getGameObject().getTransformData().position, ourMove->getMass(), 
                                        hisVel, hisMove->getGameObject().getTransformData().position, hisMove->getMass());

    mData.colVel = mData.velocity / 2;
    mData.vel = 0;
    mData.velocity = glm::vec3(0,0,0);
    mData.boost = false;
    mData.coll = true;

    ourMove->setMovementData(mData);
}

//Calculate if we are inside a terrain or if we are going to another one
void PhysicsManager::calculateTerrainCollision(MovingCharacter& movingChar, std::shared_ptr<MoveComponent> move, std::shared_ptr<TerrainComponent> terr, std::shared_ptr<CollisionComponent> coll, const float dTime) {
    if(move != nullptr && terr != nullptr && coll != nullptr && (coll->getType() != CollisionComponent::Type::BlueShell || move->getGameObject().getComponent<CollisionComponent>()->getType() != CollisionComponent::Type::BlueShell))
    {
        MoveComponent* ourMove = move.get();
        TerrainComponent* ourTerr = terr.get();
        CollisionComponent* ourColl = coll.get();
    

        auto terrain    = ourMove->getTerrain();
        auto ourMData   = ourMove->getGameObject().getTransformData();
        auto ourMovementData = ourMove ->getMovemententData();
        float radius    = ourColl->getRadius();
        
        //Future position:
        glm::vec3 nextPosition;

        if(ourMovementData.vel != 0){
            nextPosition = glm::vec3( ourMData.position + ourMovementData.velocity / ourMovementData.vel * radius);
        }else{
            nextPosition = ourMData.position;
        }

        //Check if we are out of front bounds
        if(!LAPAL::position2DLinePoint(terrain.p1, terrain.p2, nextPosition)){
            if( ourTerr->getNext() == nullptr ) {   //If there isn't next plane, collision
                calculateLineCollision(move, terrain.p1, terrain.p2);
                checkCollisionShellTerrain(move.get()->getGameObject());
                return;
            }
            //Check if our center is still in the same terrain
            else if(!LAPAL::position2DLinePoint(terrain.p1, terrain.p2, ourMData.position)){
                    //If not, change to next terrain
                    ourMove->setTerrain(ourTerr->getNext()->getTerrain()); //Set new terrain
                    movingChar.terrainComponent = ourTerr->getNext(); //Set new terrain component
            }
        }

        //Check if we are out of right bounds
        if(!LAPAL::position2DLinePoint(terrain.p2, terrain.p3, nextPosition)){
            if( ourTerr->getRight() == nullptr ) {   //If there isn't next plane, collision
                calculateLineCollision(move, terrain.p2, terrain.p3);
                checkCollisionShellTerrain(move.get()->getGameObject());
                return;
            }
            //Check if our center is still in the same terrain
            else if(!LAPAL::position2DLinePoint(terrain.p2, terrain.p3, ourMData.position)){
                    //If not, change to next terrain
                    ourMove->setTerrain(ourTerr->getRight()->getTerrain()); //Set new terrain
                    movingChar.terrainComponent = ourTerr->getRight(); //Set new terrain component
            }
        }
        
        //Check if we are out of back bounds
        if(!LAPAL::position2DLinePoint(terrain.p3, terrain.p4, nextPosition)){
            if( ourTerr->getPrev() == nullptr ) {   //If there isn't next plane, collision
                calculateLineCollision(move, terrain.p3, terrain.p4);
                checkCollisionShellTerrain(move.get()->getGameObject());
                return;
            }
            //Check if our center is still in the same terrain
            else if(!LAPAL::position2DLinePoint(terrain.p3, terrain.p4, ourMData.position)){
                    //If not, change to next terrain
                    ourMove->setTerrain(ourTerr->getPrev()->getTerrain()); //Set new terrain
                    movingChar.terrainComponent = ourTerr->getPrev(); //Set new terrain component
            }
        }

        //Check if we are out of left bounds
        if(!LAPAL::position2DLinePoint(terrain.p4, terrain.p1, nextPosition)){
           if( ourTerr->getLeft() == nullptr ) {   //If there isn't next plane, collision
                calculateLineCollision(move, terrain.p4, terrain.p1);
                checkCollisionShellTerrain(move.get()->getGameObject());
                return;
            }
            //Check if our center is still in the same terrain
            else if(!LAPAL::position2DLinePoint(terrain.p4, terrain.p1, ourMData.position)){
                    //If not, change to next terrain
                    ourMove->setTerrain(ourTerr->getLeft()->getTerrain()); //Set new terrain
                    movingChar.terrainComponent = ourTerr->getLeft(); //Set new terrain component
            }
        }
    }
}

void PhysicsManager::calculateLineCollision(std::shared_ptr<MoveComponent> move, LAPAL::vec3f p1, LAPAL::vec3f p2) {
    MoveComponent* ourMove = move.get();
    LAPAL::movementData mData = ourMove->getMovemententData();

    LAPAL::calculateReflectedVector(mData.velocity, p1, p2);

    mData.colVel = mData.velocity / 2;
    mData.vel = 0;
    mData.velocity = glm::vec3(0,0,0);
    mData.boost = false;
    mData.coll = true;

    ourMove->setMovementData(mData);

    EventData data;
    data.Component      = std::static_pointer_cast<IComponent>(move);

    EventManager::getInstance().addEvent(Event {EventType::Default_Collision, data});

}

void PhysicsManager::checkCollisionShellTerrain(GameObject& obj)
{
    if(obj.getComponent<ItemRedShellComponent>() != nullptr || obj.getComponent<ItemBlueShellComponent>() != nullptr)
    {
        EventData data;
        data.Id = obj.getId();
        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    }
}

//==============================================================================
// GETTER FUNCTIONS
//==============================================================================
PhysicsManager::MovingCharacter PhysicsManager::getMovingCharacter(uint16_t id) {
        PhysicsManager::MovingCharacter mChar;
        for(unsigned int i=0; i < movingCharacterList.size(); ++i){
            if(movingCharacterList[i].moveComponent->getGameObject().getId() == id){
                return movingCharacterList[i];
            }
        }
        return mChar; 
}

std::shared_ptr<TerrainComponent> PhysicsManager::getTerrainFromPos(LAPAL::vec3f pos) {
    
    for( unsigned int i=0; i < terrainComponentList.size(); ++i){
        
        auto terrain = std::dynamic_pointer_cast<TerrainComponent>(terrainComponentList[i]);

        //Checks if terrain collides with the object
        if(LAPAL::checkCircleRectangleCollision(terrain.get()->getTerrain(), pos, 20, 0))
            return terrain;
    }

    return nullptr;

}

//==============================================================================
// COMPONENT CREATOR FUNCTIONS
//==============================================================================

IComponent::Pointer PhysicsManager::createMoveComponent(GameObject& newGameObject, LAPAL::movementData newMData, LAPAL::plane3f newPlane, float newMass) {
    //Make shared pointer of the move component
    IComponent::Pointer component = std::make_shared<MoveComponent>(newGameObject, newMData, newPlane, newMass);

    //Attach to object
    newGameObject.addComponent(component);

    //Send event of creation
    EventData data;
    data.Component = component;
    EventManager::getInstance().addEvent(Event {EventType::MoveComponent_Create, data});

    return component;
}

IComponent::Pointer PhysicsManager::createTerrainComponent(GameObject& newGameObject, LAPAL::plane3f newPlane) {
    //Make shared pointer of the terrain component
    IComponent::Pointer component = std::make_shared<TerrainComponent>(newGameObject, newPlane);

    //Add terrain component to game object
    newGameObject.addComponent(component);

    //Send event of creation
    EventData data;
    data.Component = component;
    EventManager::getInstance().addEvent(Event {EventType::TerrainComponent_Create, data});

    return component;
}


IComponent::Pointer PhysicsManager::createCollisionComponent(GameObject& newGameObject, const float radius, const float length, const bool kinetic, const CollisionComponent::Type type) {
    //Make shared pointer of collision component
    IComponent::Pointer component = std::make_shared<CollisionComponent>(newGameObject, radius, length, kinetic, type);

    //Add collision component to game object
    newGameObject.addComponent(component);

    //Send event of creation
    EventData data;
    data.Component = component;
    EventManager::getInstance().addEvent(Event {EventType::CollisionComponent_Create, data});

    return component;
}

IComponent::Pointer PhysicsManager::createCollisionComponent(GameObject& newGameObject, const LAPAL::plane3f plane, const bool kinetic, const float length, const CollisionComponent::Type type) {
    //Make shared pointer of collision component
    IComponent::Pointer component = std::make_shared<CollisionComponent>(newGameObject, plane, length, kinetic, type);

    //Add collision component to game object
    newGameObject.addComponent(component);

    //Send event of creation
    EventData data;
    data.Component = component;
    EventManager::getInstance().addEvent(Event {EventType::CollisionComponent_Create, data});

    return component;
}

//Create and add a new updateable character to the movingCharacterList
void PhysicsManager::createMovingCharacter(IComponent::Pointer moveComponent, IComponent::Pointer terrainComponent, IComponent::Pointer collisionComponent) {

    MovingCharacter mChar;

    mChar.moveComponent = std::dynamic_pointer_cast<MoveComponent>(moveComponent);
    mChar.terrainComponent = std::dynamic_pointer_cast<TerrainComponent>(terrainComponent);
    mChar.collisionComponent = std::dynamic_pointer_cast<CollisionComponent>(collisionComponent);

    movingCharacterList.push_back(mChar);

}

IComponent::Pointer PhysicsManager::createRampComponent(GameObject& newGameObject, const float speed, const float cTime, const float dTime) {
    //Make shared pointer of ramp component
    IComponent::Pointer component = std::make_shared<RampComponent>(newGameObject, speed, cTime, dTime);

    //Attach to object
    newGameObject.addComponent(component);

    return component;
}

//==============================================
// DELEGATES
//==============================================
void addMoveComponent(EventData data) {
    PhysicsManager::getInstance().getMoveComponentList().push_back(data.Component);
    data.Component.get()->init();
}

void addCollisionComponent(EventData data) {
    PhysicsManager::getInstance().getCollisionComponentList().push_back(data.Component);
    data.Component.get()->init();
}

void addTerrainComponent(EventData data) {
    PhysicsManager::getInstance().getTerrainComponentList().push_back(data.Component);
    data.Component.get()->init();
}

void collideRamp(EventData eData) {

    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto ramp = coll->getGameObject().getComponent<RampComponent>();

    if(ramp != nullptr) {
        move->changeMaxSpeedOverTime(ramp.get()->getSpeed(), ramp.get()->getConstTime(), ramp.get()->getDecTime());
    }
}

void collideTrap(EventData eData) {
    //Get collision components from data
    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto trap = coll->getGameObject().getComponent<ItemTrapComponent>();

    if(trap != nullptr) {
        move->changeMaxSpeedOverTime(trap.get()->getSpeed(), trap.get()->getConsTime(), trap.get()->getDecTime());

        EventData data;
        data.Id = trap->getGameObject().getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    }
}
void collideBlueShell(EventData eData) {
    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto shell = coll->getGameObject().getComponent<ItemBlueShellComponent>();

    if(shell != nullptr) {
        move->changeMaxSpeedOverTime(shell.get()->getSpeed(), shell.get()->getConsTime(), shell.get()->getDecTime());

        EventData data;
        data.Id = shell->getGameObject().getId();
        
        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    }
}

void collideRedShell(EventData eData) {
    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);
    auto shell = coll->getGameObject().getComponent<ItemRedShellComponent>();

    if(shell != nullptr) {
        move->changeMaxSpeedOverTime(shell.get()->getSpeed(), shell.get()->getConsTime(), shell.get()->getDecTime());

        EventData data;
        data.Id = shell->getGameObject().getId();

        EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
    }
}

//Collide Item Box
void collideItemBox(EventData eData){

    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto itemBox = coll->getGameObject().getComponent<ItemBoxComponent>();

    auto obj = move->getGameObject();

    if(itemBox != nullptr){
       if(move->getGameObject().getComponent<IItemComponent>() == nullptr && coll->getGameObject().getTransformData().scale.x != 0 && coll->getGameObject().getTransformData().scale.y != 0 && coll->getGameObject().getTransformData().scale.z != 0){
           itemBox->asignItem(obj);
           itemBox->deactivateBox();
       }
    }
}

void collideStartLine(EventData eData) {

    auto move = std::static_pointer_cast<MoveComponent>(eData.Component);
    auto coll = std::static_pointer_cast<CollisionComponent>(eData.CollComponent);

    auto line = move->getGameObject().getComponent<StartLineComponent>();

    if(line != nullptr) {
        if(line->getActive() == true)
        {
            auto score = move->getGameObject().getComponent<ScoreComponent>();
            int vuelta = score->getLap();
            score->setLap(vuelta + 1);
            line->setActive(false);
        }
    }
}

void objectDeletedCollide(EventData eData) {

    auto& collisionComponentList = PhysicsManager::getInstance().getCollisionComponentList();

    for(unsigned int i = 0; i<collisionComponentList.size(); ++i) {
        if(eData.Id == collisionComponentList[i].get()->getGameObject().getId()) {
            collisionComponentList.erase(collisionComponentList.begin() + i);
            return;
        }
    }
}

void objectDeletedMove(EventData eData) {

    auto& moveComponentList = PhysicsManager::getInstance().getMoveComponentList();

    for(unsigned int i = 0; i<moveComponentList.size(); ++i) {
        if(eData.Id == moveComponentList[i].get()->getGameObject().getId()) {
            moveComponentList.erase(moveComponentList.begin() + i);
            return;
        }
    }
}


void objectDeletedCharacter(EventData eData) {

    auto& movingCharacterList = PhysicsManager::getInstance().getMovingCharacterList();

    for(unsigned int i = 0; i<movingCharacterList.size(); ++i) {
        if(eData.Id == movingCharacterList[i].moveComponent->getGameObject().getId()) {
            movingCharacterList.erase(movingCharacterList.begin() + i);
            return;
        }
    }
}
