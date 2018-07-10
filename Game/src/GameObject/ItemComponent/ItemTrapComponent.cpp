#include "ItemTrapComponent.h"



ItemTrapComponent::ItemTrapComponent(GameObject& newGameObject) : IItemComponent(newGameObject)
{
    speed = 5.f;
    consTime = 0.1f;
    decTime = 1.f;
}

ItemTrapComponent::~ItemTrapComponent()
{

}

void ItemTrapComponent::init()
{

}

void ItemTrapComponent::update(float dTime)
{
    
}

void ItemTrapComponent::close()
{
    
}
