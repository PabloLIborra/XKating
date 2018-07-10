#pragma once

#include "IItemComponent.h"
#include "../GameObject.h"
#include "../PhysicsComponent/MoveComponent.h"
#include "../PhysicsComponent/CollisionComponent.h"

class ItemMushroomComponent : public IItemComponent
{

    private:

        GameObject& player;
        float speed;
        float consTime;
        float decTime;

    public:

        ItemMushroomComponent(GameObject& newGameObject, GameObject& obj);

        virtual ~ItemMushroomComponent();

        virtual void init();

        virtual void update(float dTime);

        virtual void close();

};
 

 
