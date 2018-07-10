#pragma once

#include "../IComponent.h"
#include "../../GameEvent/EventManager.h"
#include "ItemHolderComponent.h"
#include "../ScoreComponent.h"

class ItemBoxComponent : public IComponent{

    public:

        //Constructor
        ItemBoxComponent(GameObject& newGameObject);

        //Destructor
        virtual ~ItemBoxComponent() {};

        //Initializer
        virtual void init();

        //Closer 
        virtual void close();

        //Update
        virtual void update(float dTime);


        int getActive()             {       return active;      }

    
    //==========================================
    // BOX RELATED FUNCTIONS
    //==========================================
    void deactivateBox();
    void asignItem(GameObject& obj);

    private:
        int active = 1;
        float boxTime;
        float actTime;
        glm::vec3 scale;

};