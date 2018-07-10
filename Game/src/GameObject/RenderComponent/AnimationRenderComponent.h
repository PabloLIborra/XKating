#pragma once

#include <string>
#include "IRenderComponent.h"
#include <iostream>
class AnimationRenderComponent : public IRenderComponent {

public:

	enum Poly {
		High,
		Medium,
		Low
	};
	
    //Constructor
	AnimationRenderComponent(GameObject& newGameObject, const char* newStr, int newFrames, int newPlayer, const char* newTex) : 
        IRenderComponent(newGameObject), frames(newFrames), player(newPlayer), polyMesh(Poly::High) {

		path = "media/anim/" + std::string(newStr);
		tex = "media/anim/" + std::string(newTex);

	}

	//Destructor
	virtual ~AnimationRenderComponent() {};

	//Initilizer
	virtual void init();

	//Update
	virtual void update(float dTime);

	//Closer
	virtual void close();

    //Drawer
	virtual void draw();

    //Pause animation
    void pause();

    //Play animation once
    void playOnce();

    //Start animation 
    void playLoop();

	std::string getPath() {
		return path;
	}

	std::string getTex() {
		return tex;
	}

	int getState() 		{ return state; }
	int getFrames()		{ return frames; }
	int getPlayer()		{ return player; }



	void setPolyMesh(Poly p){
		polyMesh = p;
	}


	Poly getPolyMesh() {
		return polyMesh;
	}

private:

	std::string path;
	std::string tex;
    int frames;
    int state = 2; // 0 = paused, 1 = play once, 2 = play loop
	int player = 0; // 0 - punk, 1 - croco, 2 - cyborg, 3 - witch
	Poly polyMesh;

};