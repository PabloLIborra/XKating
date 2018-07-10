#pragma once

#include <glm/ext.hpp>
#include "../GameObject.h"

class WaypointComponent : public IComponent{
private:
        float radius;
        int level;
public:

        //Constructor
        WaypointComponent(GameObject& newGameObject, float rad, int lvl);

        //Destructor
        ~WaypointComponent();

        //Initilizer
	virtual void init() {}

	//Update
	virtual void update(float dTime) {}

	//Closer
	virtual void close() {}

        //Getters and setters
        int getLevel()                  {   return level;   };
        float getRadius()               {   return radius;  };     
        void setRadius(float rad)       {   radius = rad;   };
        void setLevel(int lvl)          {   level  = lvl;   };

};