#pragma once

#include <iostream>
#include "../PhysicsComponent/MoveComponent.h"
#include "../StartLineComponent.h"
#include "AIDrivingComponent.h"
#include "WaypointComponent.h"

class PathPlanningComponent : public IComponent{
public:

    //Constructor
    PathPlanningComponent(GameObject& newGameObject, std::vector<GameObject::Pointer>& list);

    //Destructor
    ~PathPlanningComponent() {};

    //Initilizer
	virtual void init() {}

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close() {}

    //==============================================
    // Setters & Getters
    //==============================================
    void setSeconds(float sec)    { seconds = sec;   }; //Set seconds
    void setLastPosVector(int lvl){ lastVector = lvl; } //Set last level of the last vector
    void setDistLastWay(GameObject::Pointer n, glm::vec3 pos)
    {
        distLastWay = (n.get()->getTransformData().position.x - pos.x) * (n.get()->getTransformData().position.x - pos.x) +
                    (n.get()->getTransformData().position.y - pos.y) * (n.get()->getTransformData().position.y - pos.y) +
                    (n.get()->getTransformData().position.z - pos.z) * (n.get()->getTransformData().position.z - pos.z);
    }

    glm::vec3 getNextPoint(){ return nextPos;     }
    float getDistLastWay()  { return distLastWay; }
    int getLastPosVector()  { return lastVector;  }
    int getActualLevel()
    {
        return listNodes[lastVector].get()->getComponent<WaypointComponent>()->getLevel();
    }
    float getActualDistance()
    {
        glm::vec3 pos = getGameObject().getTransformData().position;

        float distanceActualWay = (listNodes[lastVector].get()->getTransformData().position.x - pos.x) * (listNodes[lastVector].get()->getTransformData().position.x - pos.x) +
                            (listNodes[lastVector].get()->getTransformData().position.y - pos.y) * (listNodes[lastVector].get()->getTransformData().position.y - pos.y) +
                            (listNodes[lastVector].get()->getTransformData().position.z - pos.z) * (listNodes[lastVector].get()->getTransformData().position.z - pos.z);

        return distanceActualWay;
    }

private:
    //==============================================
    // PRIVATE DATA
    //==============================================
    std::vector<GameObject::Pointer> listNodes;
    float seconds;
    float distLastWay;
    unsigned int lastVector;
    glm::vec3 nextPos;
}; 
