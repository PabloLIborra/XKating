#include "Clock.h"

//Init
void Clock::init() {
    //Initialize clock
    initTime = std::chrono::high_resolution_clock::now();
    lastTime = initTime;
}

//Restart the clock counting
void Clock::restart(){
    //Get current time
    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();

    //Assign new last time
    lastTime = currentTime;
}

//Update
double Clock::getElapsedTime() {
    //Get current time
    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();

    //Get elapsed time
    elapsedTime = currentTime - lastTime;

    //Return value
    return elapsedTime.count();
}

double Clock::getInitTime() {
    //Return value
    double r = initTime.time_since_epoch().count();
    return r;
}