#include "PathPlanningComponent.h"
 

PathPlanningComponent::PathPlanningComponent(GameObject& newGameObject, std::vector<GameObject::Pointer>& list) : IComponent(newGameObject) 
{
	seconds = 1;
    distLastWay = -1;
    lastVector = 0;
	listNodes = list;
}

void PathPlanningComponent::update(float dTime)
{

	auto pos = this->getGameObject().getTransformData().position;

	auto modSpeed = this->getGameObject().getComponent<MoveComponent>()->getMovemententData().vel;

	float distaneActualWay = (listNodes[lastVector].get()->getTransformData().position.x - pos.x) * (listNodes[lastVector].get()->getTransformData().position.x - pos.x) +
						(listNodes[lastVector].get()->getTransformData().position.y - pos.y) * (listNodes[lastVector].get()->getTransformData().position.y - pos.y) +
						(listNodes[lastVector].get()->getTransformData().position.z - pos.z) * (listNodes[lastVector].get()->getTransformData().position.z - pos.z);
	
	float distaneNextWay;

	if(lastVector == listNodes.size()-1)
	{
		distaneNextWay = (listNodes[0].get()->getTransformData().position.x - pos.x) * (listNodes[0].get()->getTransformData().position.x - pos.x) +
						(listNodes[0].get()->getTransformData().position.y - pos.y) * (listNodes[0].get()->getTransformData().position.y - pos.y) +
						(listNodes[0].get()->getTransformData().position.z - pos.z) * (listNodes[0].get()->getTransformData().position.z - pos.z);
	
	}
	else
	{
		distaneNextWay = (listNodes[lastVector+1].get()->getTransformData().position.x - pos.x) * (listNodes[lastVector+1].get()->getTransformData().position.x - pos.x) +
						(listNodes[lastVector+1].get()->getTransformData().position.y - pos.y) * (listNodes[lastVector+1].get()->getTransformData().position.y - pos.y) +
						(listNodes[lastVector+1].get()->getTransformData().position.z - pos.z) * (listNodes[lastVector+1].get()->getTransformData().position.z - pos.z);
	
	}
	
	
	float radius = listNodes[lastVector].get()->getComponent<WaypointComponent>()->getRadius();

	if(this->getGameObject().getComponent<AIDrivingComponent>() != nullptr)
	{
		if((distaneActualWay <= (radius*radius)/2) || distaneNextWay < distaneActualWay)
		{
			if(lastVector < listNodes.size()-1)
			{
				lastVector++;
			}
			else if(lastVector == listNodes.size()-1)
			{
				getGameObject().getComponent<StartLineComponent>()->setActive(true);
				lastVector = 0;
			}
		}
	}
	else
	{
		if((distaneActualWay <= radius*radius) || distaneNextWay < distaneActualWay)
		{
			if(lastVector < listNodes.size()-1)
			{
				lastVector++;
			}
			else if(lastVector == listNodes.size()-1)
			{
				getGameObject().getComponent<StartLineComponent>()->setActive(true);
				lastVector = 0;
			}
		}
	}
	

    float tour = (modSpeed * seconds) * (modSpeed * seconds);
	int posVector;
	int lastPosVector = lastVector;
	float distanceNextNode;
    float distNode;
	float dist;

    int lvl;

    for (size_t i = lastPosVector; i < listNodes.size(); i++)
	{
		lvl = listNodes[lastPosVector].get()->getComponent<WaypointComponent>()->getLevel();
		if(listNodes[i].get()->getComponent<WaypointComponent>()->getLevel() >= lvl)
		{
			setDistLastWay(listNodes[lastPosVector], pos);
			distNode = (listNodes[i].get()->getTransformData().position.x - pos.x) * (listNodes[i].get()->getTransformData().position.x - pos.x) +
					(listNodes[i].get()->getTransformData().position.y - pos.y) * (listNodes[i].get()->getTransformData().position.y - pos.y) +
					(listNodes[i].get()->getTransformData().position.z - pos.z) * (listNodes[i].get()->getTransformData().position.z - pos.z);

			
			if((lvl+1) == listNodes[i].get()->getComponent<WaypointComponent>()->getLevel())
			{
				if(tour-distLastWay < 0)
				{
					nextPos = listNodes[lastPosVector].get()->getTransformData().position;
					return;
				}
				else
				{
					if(distNode <= tour)
					{
						lastPosVector = i;
					}                        
				}
			}
		}
	}
		posVector = lastPosVector;
        distanceNextNode  = -1;

        for(size_t i = lastPosVector; i < listNodes.size(); i++)
        {  
			if(listNodes[i]->getComponent<WaypointComponent>()->getLevel() == listNodes[lastPosVector]->getComponent<WaypointComponent>()->getLevel()+1)
			{
				distNode = (listNodes[i].get()->getTransformData().position.x - pos.x) * (listNodes[i].get()->getTransformData().position.x - pos.x) +
						(listNodes[i].get()->getTransformData().position.y - pos.y) * (listNodes[i].get()->getTransformData().position.y - pos.y) +
						(listNodes[i].get()->getTransformData().position.z - pos.z) * (listNodes[i].get()->getTransformData().position.z - pos.z);

				if(distanceNextNode == -1)
				{
					distanceNextNode = distNode;
					lastPosVector = i;
				}
			}
        }

        distNode = (pos.x - listNodes[posVector].get()->getTransformData().position.x) * (pos.x - listNodes[posVector]->getTransformData().position.x) +
                (pos.y - listNodes[posVector].get()->getTransformData().position.y) * (pos.y - listNodes[posVector]->getTransformData().position.y) +
                (pos.z - listNodes[posVector].get()->getTransformData().position.z) * (pos.z - listNodes[posVector]->getTransformData().position.z);
        
		dist = ( pos.x - listNodes[lastPosVector].get()->getTransformData().position.x) * ( pos.x - listNodes[lastPosVector].get()->getTransformData().position.x) +
				( pos.y - listNodes[lastPosVector].get()->getTransformData().position.y) * ( pos.y - listNodes[lastPosVector].get()->getTransformData().position.y) +
				(pos.z - listNodes[lastPosVector].get()->getTransformData().position.z) * ( pos.z - listNodes[lastPosVector].get()->getTransformData().position.z);
		
		tour -= distNode;
		

        nextPos = ((tour/dist) * (listNodes[lastPosVector].get()->getTransformData().position - listNodes[posVector].get()->getTransformData().position) + listNodes[posVector].get()->getTransformData().position);
		
        
    return;
}