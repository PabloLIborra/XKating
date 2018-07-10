#pragma once

#include <stdint.h>
#include <chrono>

class Clock {

public:
	//===========================================
	// BASIC FUNCTIONS
	//===========================================

	//Constructor
	Clock() {}

	//Destructor
	~Clock() {}

	//===========================================
	// SPECIFIC FUNCTIONS
	//===========================================

	//Init
	void init();

    //Restart the counter
	void restart();

	//Get time since the beginning of the clock
	double getInitTime();

	//Get elapsed time since last time the clock was restarted
	double getElapsedTime();

private:
    
    std::chrono::time_point<std::chrono::high_resolution_clock> initTime;    //Initial time
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;    //Last time elapsed time was consulted
    std::chrono::duration<double>                               elapsedTime; //Elapsed time since lastTime
};