#include "WaypointComponent.h"



//==============================================
//Contructor and Destructor
//==============================================

WaypointComponent::WaypointComponent(GameObject& newGameObject, float rad, int lvl) : IComponent(newGameObject)
{
    radius = rad;
    level = lvl;
}

WaypointComponent::~WaypointComponent()
{

}