#include "ItemMushroomComponent.h"



ItemMushroomComponent::ItemMushroomComponent(GameObject& newGameObject, GameObject& obj) : IItemComponent(newGameObject), player(obj)
{
    speed = 800.f;
    consTime = 0.1f;
    decTime = 4.f;
}

ItemMushroomComponent::~ItemMushroomComponent()
{

}

void ItemMushroomComponent::init()
{
    player.getComponent<MoveComponent>()->changeMaxSpeedOverTime(speed, consTime, decTime);
}

void ItemMushroomComponent::update(float dTime)
{
    
}

void ItemMushroomComponent::close()
{
    
} 
 
