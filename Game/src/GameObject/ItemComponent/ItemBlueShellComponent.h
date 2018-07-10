#pragma once

#include "IItemComponent.h"
#include "../../GameManager/ScoreManager.h"
#include "../../GameManager/WaypointManager.h"
#include "../AIComponent/VSensorComponent.h"

class ItemBlueShellComponent : public IItemComponent
{
    public:

        ItemBlueShellComponent(GameObject& newGameObject, GameObject& obj, IItemComponent::InstanceType m);

        ~ItemBlueShellComponent();

        void init() {};
        void init(float actualVector);

        void update(float dTime);

        void close();

        //Getters
        float getSpeed()        {       return speed;       };
        float getConsTime()     {       return consTime;    };
        float getDecTime()      {       return decTime;     };

    private:

        GameObject& player;                //Player who've thrown it
        float speed;                       //Speed of the blue shell
        float consTime;                    
        float decTime;                    
        unsigned int lastVector;           
        glm::vec3 objective;               //Objective position of the enemy
        ScoreComponent::Pointer enemy;     //Score component of the guy you're aiming at
        IItemComponent::InstanceType mode; //How the object was created
        float valueY;                      //Max value to increase Y
        float lastPosVector;
        std::vector<GameObject::Pointer> waypoints;
        bool go;
};
 

