#include "ItemBlueShellComponent.h"



ItemBlueShellComponent::ItemBlueShellComponent(GameObject& newGameObject, GameObject& obj, IItemComponent::InstanceType m) : IItemComponent(newGameObject), player(obj), mode(m)
{
    speed = 1.f;
    consTime = 0.1f;
    decTime = 1.f;
}

ItemBlueShellComponent::~ItemBlueShellComponent()
{

}

void ItemBlueShellComponent::init(float actualVector)
{
    lastVector = player.getComponent<PathPlanningComponent>()->getLastPosVector();
    enemy = ScoreManager::getInstance().getPlayers()[0];
    valueY = 0;
    getGameObject().getComponent<PathPlanningComponent>()->setLastPosVector(actualVector);
    waypoints = WaypointManager::getInstance().getWaypoints();
    go = false;
    getGameObject().getComponent<MoveComponent>()->changeInvul(true);
}

void ItemBlueShellComponent::update(float dTime)
{

    //Return if object was created as a copy of a remote online object
    if(mode == IItemComponent::InstanceType::REMOTE)
        return;


    auto trans = getGameObject().getTransformData();
    auto pos = trans.position;
    auto maxSpeed = getGameObject().getComponent<MoveComponent>()->getMovemententData().max_vel;
    getGameObject().getComponent<MoveComponent>()->changeVel(0);

    auto moveComponent = getGameObject().getComponent<MoveComponent>().get();

    objective = enemy.get()->getGameObject().getTransformData().position; 
    

    //Animation missile
    
    if(valueY < 60 && go == false)
    {
        valueY += 4;
        trans.position.y = trans.position.y + valueY;
        trans.rotation.z = 180;
    }
    else if(valueY == 60)
    {
        go = true;
    }

    if(go == true)
    {
        auto terrain = moveComponent->getTerrain();
        float yTerrain = LAPAL::calculateExpectedY(terrain, pos);
        trans.position = objective;
        auto length = enemy->getGameObject().getComponent<CollisionComponent>()->getLength();
        trans.position.y +=  length+valueY;
        if(valueY == 60)
            valueY = 20;
        else if(valueY > 0)
            valueY -= 1;

        if(valueY == 0)
        {
            EventData data;
            data.Id             = getGameObject().getId();
            data.Component      = enemy.get()->getGameObject().getComponent<MoveComponent>();
            data.CollComponent  = getGameObject().getComponent<CollisionComponent>();

            EventManager::getInstance().addEvent(Event {EventType::BlueShellComponent_Collision, data});
        }
    }

    getGameObject().setNewTransformData(trans);
    RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);

}

void ItemBlueShellComponent::close()
{
    
} 
