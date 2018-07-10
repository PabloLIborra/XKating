#pragma once

#include "AIComponent/PathPlanningComponent.h"

class ScoreComponent : public IComponent{
    public:
        //Define ::Pointer
        typedef std::shared_ptr<ScoreComponent> Pointer;

        //Constructor
        ScoreComponent(GameObject& newGameObject);

        //Destructor
        ~ScoreComponent() {}

        //Initilizer
        virtual void init() {};

        //Update
        virtual void update(float dTime) {};

        //Closer
        virtual void close() {};

        //Returns a score based on lap and waypoint on the lap, in a way that being on the first waypoint of a higher lap
        //has more score than being in the highest waypoint of a previous lap
        //Maximum number of waypoints possible is set in the variable maxWaypoints
        int getScore();

        //Gets the distance to it's current waypoint
        float getActualDistance();

        //Getters
        int getLap() {return lap;}
        int getPosition() {return position;}
        int getMaxWaypoints() {return maxWaypoints;}

        //Setters
        void setLap(int i) {lap=i;}
        void setPosition(int i) {position=i;}
        void setMaxWaypoints(int i) {maxWaypoints=i;}
    private:
        int lap;
        int position;
        int maxWaypoints; //Maximum number of waypoints: Initially 300
};