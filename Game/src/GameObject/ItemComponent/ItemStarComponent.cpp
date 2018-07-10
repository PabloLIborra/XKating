#include "ItemStarComponent.h"



ItemStarComponent::ItemStarComponent(GameObject& newGameObject, GameObject& obj) : IItemComponent(newGameObject), player(obj)
{
    speed = 650.f;
    consTime = 5.f;
    decTime = 0.5f;
}

ItemStarComponent::~ItemStarComponent()
{

}

void ItemStarComponent::init()
{
    player.getComponent<MoveComponent>()->changeMaxSpeedOverTime(speed, consTime, decTime);
    player.getComponent<MoveComponent>()->changeInvul(true);
}

void ItemStarComponent::update(float dTime)
{
    
}

void ItemStarComponent::close()
{
    
} 
 
