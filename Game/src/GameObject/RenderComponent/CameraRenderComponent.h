#pragma once

#include <iostream>
#include "IRenderComponent.h"
#include "../PhysicsComponent/TerrainComponent.h"


class CameraRenderComponent : public IRenderComponent {

public:

    //Constructor
	CameraRenderComponent(GameObject& newGameObject);

	//Destructor
	virtual ~CameraRenderComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Drawer
	virtual void draw();

	//Getters
	std::shared_ptr<TerrainComponent> 	getTerrain() 		{	return terrain;		}
	float 								getDistance()		{	return distance;	}
	float 								getMaxDistance()	{	return maxDistance;	}
	float 								getOldDistance()	{	return oldDistance; }
	float								getHeight()			{	return mheight;		}
	float								getOldHeight()		{	return moldHeight;	}
	bool								getBlurActivation() {	return blur;	    }
	float								getBlurFactor()     {	return blurFactor;	}

	//Setters
	void setTerrain(std::shared_ptr<TerrainComponent> t) 	{	terrain = t; 		}
	void setDistance(float d) 								{ 	distance = d; 		}
	void setMaxDistance(float d) 							{ 	maxDistance = d;	}
	void setOldDistance(float d) 							{ 	oldDistance = d; 	}

private:

	std::shared_ptr<TerrainComponent> terrain;
	float distance;
	float minDistanceCP;
	float maxDistance;
	float sumDistanceCP;
	float oldDistance;
	float height[5] = {0,0,0,0,0};
	float oldHeight[5] = {0,0,0,0,0};
	float mheight;
	float moldHeight;
	int count;
	int spinDir;
	float cameraMaxVel;

	//Activate blur effect or not
	bool blur;

	//Indicated the blur factor (proportion of activation) from 0 to 1
	float blurFactor;

	//Distance needed to activate the blur
	float blurActivationDistance;
};