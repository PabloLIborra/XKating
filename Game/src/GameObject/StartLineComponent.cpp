#include "StartLineComponent.h"

StartLineComponent::StartLineComponent(GameObject& newGameObject) : IComponent(newGameObject)
{
    active = false;
}