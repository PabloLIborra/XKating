#pragma once

#include "IItemComponent.h"
#include "../PhysicsComponent/MoveComponent.h"
#include "../PhysicsComponent/CollisionComponent.h"

class ItemStarComponent : public IItemComponent
{

    private:

        GameObject& player;
        float speed;
        float consTime;
        float decTime;

    public:

        ItemStarComponent(GameObject& newGameObject, GameObject& obj);

        ~ItemStarComponent();

        void init();

        void update(float dTime);

        void close();

};
 

 
 
