#pragma once

#include "IItemComponent.h"
#include "../../GameManager/ScoreManager.h"
#include "../../GameManager/WaypointManager.h"
#include "../AIComponent/VSensorComponent.h"

class ItemRedShellComponent : public IItemComponent
{


    public:

        //Constructor
        ItemRedShellComponent(GameObject& newGameObject, GameObject& obj, IItemComponent::InstanceType m);

        //Destructor
        ~ItemRedShellComponent();

        //Initializer
        void init();

        //Update
        void update(float dTime);

        //Close function
        void close();

        //==============================================================
        // GETTERS & SETTERS
        //==============================================================
        float getSpeed()        {       return speed;       };
        float getConsTime()     {       return consTime;    };
        float getDecTime()      {       return decTime;     };

    private:
        //==============================================================
        // PRIVATE DATA
        //==============================================================
        GameObject& player;                //Victim player
        float speed;                       //Speed of the red shell
        float consTime;                    
        float decTime;                     
        unsigned int lastVector;           
        int myPos;                        
        glm::vec3 objective;               //vec3 indicating the position to which to go
        ScoreComponent::Pointer enemy;     //Pointer to the score of the victim player
        IItemComponent::InstanceType mode; //How the item was instanciated
        float valueY;                      //Max value to increase Y
        std::vector<GameObject::Pointer> waypoints;

};
 

 
 
