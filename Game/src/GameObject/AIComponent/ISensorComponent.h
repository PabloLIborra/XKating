#pragma once

#include "../IComponent.h"

class ISensorComponent : public IComponent {

public:

    ISensorComponent(GameObject& newGameObject) : IComponent(newGameObject) {}

};