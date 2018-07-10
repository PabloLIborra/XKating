#pragma once

#include "IComponent.h"

class StartLineComponent : public IComponent{
    public:

        //Define ::Pointer
        typedef std::shared_ptr<StartLineComponent> Pointer;

        //Constructor
        StartLineComponent(GameObject& newGameObject);

        //Destructor
        ~StartLineComponent() {}

        //Initilizer
        virtual void init() {};

        //Update
        virtual void update(float dTime) {};

        //Closer
        virtual void close() {};

        //Getters
        bool getActive()                        {       return active;      }

        //Setters
        void setActive(bool act)                {       active = act;       }


    private:
        bool active;
};