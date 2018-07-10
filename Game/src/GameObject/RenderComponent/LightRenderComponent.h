#pragma once

#include "IRenderComponent.h"

class LightRenderComponent : public IRenderComponent {

public:

	enum Type {
		Point,
        Directional
	};

    //Constructor
	LightRenderComponent(GameObject& newGameObject, Type newType, float newRadius) : IRenderComponent(newGameObject), lightType(newType), radius(newRadius) {}

	//Destructor
	virtual ~LightRenderComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Drawer
	virtual void draw();

	Type getLightType() {
		return lightType;
	}

    float getLightRadius() {
		return radius;
	}


private:

	Type lightType;
    float radius;

};