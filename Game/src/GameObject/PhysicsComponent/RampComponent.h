#pragma once

#include "../IComponent.h"

class RampComponent : public IComponent {

public:


    //Constructor
	RampComponent(GameObject& newGameObject, float speed, float cTime, float dTime) : IComponent(newGameObject), 
        maxSpeed(speed), constTime(cTime), decTime(dTime) {}

	//Destructor
	virtual ~RampComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

	//Getters
    const float getSpeed()      { return maxSpeed;  }
    const float getConstTime()  { return constTime; }
    const float getDecTime()    { return decTime;   }


private:

    float maxSpeed;
    float constTime;
    float decTime;

};