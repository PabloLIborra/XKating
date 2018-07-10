#pragma once

#include "LAPALData.h"

namespace LAPAL {

    //--------------------------------------
    //-------------MOVEMENT-----------------
    //--------------------------------------
    //Updates all linear variables 
    void updateLinearVelocity(LAPAL::movementData& mData, const float dTime, const LAPAL::plane3f& terrain);

    //Updates spin velocity and spin
    void updateSpin(LAPAL::movementData& mData, const float dTime);

    //Updates vector velocity
    void updateVelocity(LAPAL::movementData& mData, LAPAL::plane3f& terrain);

    //Updates object rotation based on terrain
    void updateRotation(LAPAL::movementData& mData, LAPAL::plane3f& terrain, const float dTime);

    //function that moves the vehicle elliptically given its internal radius ratio rotation
    void updateEllipticMovement( LAPAL::movementData& mData, const float dTime);

    //Updates the deviation in velocity caused by a collision
    void updateCollisionMovement ( LAPAL::movementData& mData, const float dTime);

    //--------------------------------------
    //-------------COLLISIONS---------------
    //--------------------------------------
    //Checks colision between circles
    bool checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const float& length1, const LAPAL::vec3f& pos2,const float& radius2, const float& length2);

    //Assuming there's collision, changes velocity of every object after collision
    void calculateElasticCollision(LAPAL::vec3f& vel1, const LAPAL::vec3f& pos1, const float& mass1, LAPAL::vec3f& vel2, const LAPAL::vec3f& pos2, const float& mass2);

    //Checks if terrain is horizontal o it's a pendent 
    bool checkTerrain(const LAPAL::plane3f& terrain); //done

    //Calculates rotation in X and Z of the plane, assigning the value to rotZ and rotX
    void calculateRotationsXZ(LAPAL::plane3f& terrain);

    //Calculates expected Y for the object given its position
    float calculateExpectedY(const LAPAL::plane3f& terrain, const LAPAL::vec3f& position );

    //Corrects position of the object on Y
    void correctYPosition(LAPAL::movementData& mData, const float dTime, LAPAL::plane3f& terrain, LAPAL::vec3f& position);

    //Reflects the velocity given a line
    void calculateReflectedVector(LAPAL::vec3f& vel, const LAPAL::vec3f& p1, const LAPAL::vec3f& p2);

    //--------------------------------------
    //-------------MATHEMATICS--------------
    //--------------------------------------

    //Calculates values A and B which are the scalars that multiply vector A and B to compose the point C in 2D (X-Z plane)
    void calculateAB(const LAPAL::vec3f& vecC, const LAPAL::vec3f& vec1, const LAPAL::vec3f& vec2,  float& a, float& b);

    //Calculates values A and B in given terrain, using Cross vectors (from p1 to p3 and from p2 to p4 OR down-left to up-right and up-left to down-right)
    void calculateTerrainAB(const LAPAL::plane3f& terrain, const LAPAL::vec3f& position, float& a, float& b);

    //Calculates the distance between a line defined by two points (l1,l2) and a point (p1)
    float distance2DLinePoint(const LAPAL::vec3f& l1, const LAPAL::vec3f& l2, const LAPAL::vec3f& p1);

    //Calculates if a point (p1) is left o right of a line defined by two points (l1,l2)
    bool position2DLinePoint(const LAPAL::vec3f& l1, const LAPAL::vec3f& l2, const LAPAL::vec3f& p1);

    //Calculates if a circle is inside a rectangle
    bool checkCircleRectangleCollision(const LAPAL::plane3f& terrain, const LAPAL::vec3f& nextPosition, const float length, const float length2);

    //calculate angle a-b and a-c
    float calculateAngleVector(glm::vec3 a, glm::vec3 b, glm::vec3 c);
}