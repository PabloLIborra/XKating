#include "ItemRedShellComponent.h"


ItemRedShellComponent::ItemRedShellComponent(GameObject& newGameObject, GameObject& obj, IItemComponent::InstanceType m) : IItemComponent(newGameObject), player(obj), mode(m)
{
    speed = 1.f;
    consTime = 0.1f;
    decTime = 1.f;
}

ItemRedShellComponent::~ItemRedShellComponent()
{

}

void ItemRedShellComponent::init()
{
    lastVector = player.getComponent<PathPlanningComponent>()->getLastPosVector();
    myPos = player.getComponent<ScoreComponent>()->getPosition(); 

    if(myPos > 1)
    {
        enemy = ScoreManager::getInstance().getPlayers()[myPos-2];
    }

    getGameObject().getComponent<PathPlanningComponent>()->setLastPosVector(lastVector);
    waypoints = WaypointManager::getInstance().getWaypoints();
}

void ItemRedShellComponent::update(float dTime)
{
    //if the object was created as an online copy of another object
    if(mode == IItemComponent::InstanceType::REMOTE)
        return;

    auto moveComponent = getGameObject().getComponent<MoveComponent>().get();

    if(myPos > 1)
    {

        auto trans = getGameObject().getTransformData();
        auto pos = trans.position;
        auto maxSpeed = moveComponent->getMovemententData().max_vel;
        getGameObject().getComponent<MoveComponent>()->changeVel(0);

        objective = enemy.get()->getGameObject().getTransformData().position; 
        glm::vec3 nextPos;

        auto distCover = (maxSpeed * maxSpeed) *0.1* dTime;
        glm::vec3 distination;


        float distancePlayer = (objective.x - pos.x) * (objective.x - pos.x) +
                            (objective.y - pos.y) * (objective.y - pos.y) +
                            (objective.z - pos.z) * (objective.z - pos.z);


        auto terrain = moveComponent->getTerrain();
        float yTerrain = LAPAL::calculateExpectedY(terrain, pos);

        unsigned int posVector = getGameObject().getComponent<PathPlanningComponent>()->getLastPosVector();

        float radius = waypoints[lastVector].get()->getComponent<WaypointComponent>()->getRadius();

        if((distancePlayer <= 12000))
        {   //When the distance enemy-missile is to small, we change the position of missile to enemy and event collision

            if(distancePlayer <= 500)
            {

                valueY = 0;
                nextPos = distination;

                EventData data;
                data.Id             = getGameObject().getId();
                data.Component      = enemy.get()->getGameObject().getComponent<MoveComponent>();
                data.CollComponent  = getGameObject().getComponent<CollisionComponent>();

                EventManager::getInstance().addEvent(Event {EventType::RedShellComponent_Collision, data});
            }
            else
            {
                if(valueY > 0)
                {
                    valueY -= 0.1;
                }
                valueY = 0;
                distination = objective;
                nextPos = ((distCover/3000) * (distination - trans.position)) + trans.position;
            }
        }
        else
        {

            float distaneActualWay = (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) * (waypoints[posVector].get()->getTransformData().position.x - trans.position.x) +
                                    (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) * (waypoints[posVector].get()->getTransformData().position.y - trans.position.y) +
                                    (waypoints[posVector].get()->getTransformData().position.z - trans.position.z) * (waypoints[posVector].get()->getTransformData().position.z - trans.position.z);

        
            if((distaneActualWay <= (radius*radius)) || (waypoints[posVector].get()->getTransformData().position.x - pos.x == 0 && waypoints[posVector].get()->getTransformData().position.z - pos.z == 0))
            {
                if(posVector < waypoints.size()-1)
                {
                    posVector++;
                    getGameObject().getComponent<PathPlanningComponent>()->setLastPosVector(posVector);
                }
                else if(posVector == waypoints.size()-1)
                {
                    getGameObject().getComponent<PathPlanningComponent>()->setLastPosVector(0);
                }
                posVector = getGameObject().getComponent<PathPlanningComponent>()->getLastPosVector();
            }
             
            distination = waypoints[posVector].get()->getTransformData().position;
            nextPos = ((distCover/10000) * (distination - trans.position)) + trans.position;
            
        }
        trans.position = nextPos;

        getGameObject().setNewTransformData(trans);
        RenderManager::getInstance().getRenderFacade()->updateObjectTransform(getGameObject().getId(), trans);
        

    }
    //Accelerate and brake
    moveComponent->isMoving(true);
    moveComponent->changeAcc(1);
    
}

void ItemRedShellComponent::close()
{
    
} 
 
 
