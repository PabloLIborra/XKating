 #pragma once

#include "../GameObject.h"

class IItemComponent : public IComponent {



    public:

        enum ItemType{
            
            none        = -1,
            redShell    = 0,      
            blueShell   = 1,      
            trap        = 2,      
            mushroom    = 3,      
            star        = 4
        
        };

        enum InstanceType{
            
            LOCAL       = 0,
            REMOTE      = 1
        
        };

    IItemComponent(GameObject& newGameObject) : IComponent(newGameObject) {};

    virtual ~IItemComponent() {};

    virtual void init() = 0;

    virtual void update(float dTime) = 0;

    virtual void close() = 0;

};
