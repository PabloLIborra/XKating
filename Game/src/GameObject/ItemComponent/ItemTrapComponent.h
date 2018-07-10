 #pragma once

#include "IItemComponent.h"

class ItemTrapComponent : public IItemComponent
{

    private:

        float speed;
        float consTime;
        float decTime;

    public:

        ItemTrapComponent(GameObject& newGameObject);

        virtual ~ItemTrapComponent();

        virtual void init();

        virtual void update(float dTime);

        virtual void close();

        //Getters
        float getSpeed()        {       return speed;       };
        float getConsTime()     {       return consTime;    };
        float getDecTime()      {       return decTime;     };

};
 
