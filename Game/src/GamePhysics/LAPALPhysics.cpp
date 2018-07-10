#include "LAPALPhysics.h"
#include <math.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>


//--------------------------------------
//-------------MOVEMENT-----------------
//--------------------------------------

//Updates all linear variables 
void LAPAL::updateLinearVelocity(LAPAL::movementData& mData, const float dTime, const LAPAL::plane3f& terrain) {

    //The approach is that we define a base acceleration for every object and it does not change when it reaches the max acceleration
    //then, we calculate how the different forces affect the acceleration and, therefore, the velocity

    //Initial check of destination: prevents acceleration from pushing same direction when chaning forces.
    if((mData.acc <0 && mData.dAcc>0) || (mData.acc >0 && mData.dAcc <0)){
        mData.acc = 0.f;
    }

    //BASE ACCELERATION
    mData.acc += mData.dAcc*dTime; //increment of acceleration * increment of time

    //Aply friction
    if(!mData.mov) {
        if(mData.acc>0) {
            mData.acc -= (mData.brake_acc)*dTime;  
        }else {
            mData.acc += (mData.brake_acc)*dTime; 
        }
    }
    
    //If we aren't accelerating
    if(abs(mData.acc)<0.5  && !mData.mov)
        mData.acc = 0;

    //Check acceleration limits
    if(abs(mData.acc)>abs(mData.max_acc)){
        mData.acc = copysign(mData.max_acc, mData.acc);
    }

    //Update velocity
    mData.vel += mData.acc*dTime*(1-terrain.fric); 

    //Aply friction
    if(!mData.mov) {
        if(mData.vel>0) {
            mData.vel -= (mData.brake_vel + terrain.fric)*dTime;  
        }else {
            mData.vel += (mData.brake_vel + terrain.fric)*dTime; 
        }
    }

    //If we aren't accelerating
    if(abs(mData.vel)<0.5 && !mData.mov)
        mData.vel = 0;

    //Check velocity limits 
    if(abs(mData.vel)>abs(mData.max_vel*(1-terrain.fric))){
        mData.vel = copysign(mData.max_vel*(1-terrain.fric), mData.vel);
    }
    
    //Check if we are braking
    if(mData.braking){
        mData.vel = mData.vel * 0.99f;
        mData.acc = mData.acc * 0.99f;
    }

    if(mData.colVel.x > 0 || mData.colVel.y > 0 || mData.colVel.z > 0 || mData.coll)
        mData.vel = 0;
}

//Updates vector velocity
void LAPAL::updateVelocity(LAPAL::movementData& mData, LAPAL::plane3f& terrain){
    mData.velocity.x = mData.vel*cos(mData.angle);
    mData.velocity.y = 0;
    mData.velocity.z = mData.vel*sin(mData.angle);

    //Applying reduction of speed based on plane angle
    if(!checkTerrain(terrain)){
        //If we are going upwards in X axis, then lower the speed
        if((mData.velocity.x > 0 && terrain.rotX > 0) || (mData.velocity.x < 0 && terrain.rotX < 0)){
            mData.velocity.x *= cos(terrain.rotX);
        }else{
            //if we are going downwards, raise it a bit
            mData.velocity.x *= (2-cos(terrain.rotX));
        }

        //If we are going upwards in Z axis, then lower the speed
        if((mData.velocity.z > 0 && terrain.rotZ > 0) || (mData.velocity.z < 0 && terrain.rotZ < 0)){
            mData.velocity.z *= cos(terrain.rotZ);
        }else{
            //if we are going downwards, raise it a bit
            mData.velocity.z *= (2-cos(terrain.rotZ));
        }
    }
    if(mData.coll)
        mData.velocity = glm::vec3(0,0,0);
}

//Updates all spin related variables
void LAPAL::updateSpin(LAPAL::movementData& mData, const float dTime){
    //if spinning
    float rel = 1 - (mData.vel / (mData.max_vel*1.5));
    mData.spin += mData.spin_inc * dTime * rel; //Spin depends on vel and spin_inc

    //if not
    if(!mData.spi) {
        if(abs(mData.spin) < 0.001) {
            mData.spin = 0;
         
        //Comment if no friction
        }else {
            mData.spin -= mData.spin*(mData.brake_spin);
        }
    }

    if(abs(mData.spin)>abs(mData.max_spin * rel)){
        mData.spin = copysign(mData.max_spin * rel, mData.spin);
    }

    if(!mData.drift)
    {
        mData.angle += mData.spin * dTime * 50;
    }
}

//Updates rotation of the vehicle to match the terrain's. Do it smoothly.
void LAPAL::updateRotation(LAPAL::movementData& mData, LAPAL::plane3f& terrain, const float dTime){

    //Increasing rotation axis in mData until it matches the terrain. Incremental turn.
    //X axis.
    if(glm::abs(mData.angX - terrain.rotX)<0.05f){
        mData.angX = terrain.rotX;
        mData.rotateX = 0.f;
    }else{
        //increment rotate speed
        mData.rotateX += mData.rotate_inc*dTime;

        if(abs(mData.rotateX)>abs(mData.max_rotate)){
            mData.rotateX = copysign(mData.max_rotate, mData.rotateX);
        }

        //if rotation is higher or lower
        if(mData.angX < terrain.rotX){
            mData.angX += mData.rotateX*dTime;
        }else{
            mData.angX -= mData.rotateX*dTime;
        }
    }

    //Z axis
    if(glm::abs(mData.angZ - terrain.rotZ)<0.05f){
        mData.angZ = terrain.rotZ;
        mData.rotateZ = 0.f;
    }else{
        //increment rotate speed
        mData.rotateZ += mData.rotate_inc*dTime;

        if(abs(mData.rotateZ)>abs(mData.max_rotate)){
            mData.rotateZ = copysign(mData.max_rotate, mData.rotateZ);
        }

        //if rotation is higher or lower
        if(mData.angZ < terrain.rotZ){
            mData.angZ += mData.rotateZ*dTime;
        }else{
            mData.angZ -= mData.rotateZ*dTime;
        }
    }

}

//Function that moves the vehicle elliptically given its internal radius ratio rotation
void LAPAL::updateEllipticMovement( LAPAL::movementData& mData, const float dTime){
    //Check if drifting is pressed
    if(mData.drift){ 
        //Initial variables
        mData.driftAngleIncr   += dTime;                    //Augment drifting angle increment
        if(mData.driftAngleIncr > mData.driftAngleIncrMax)
            mData.driftAngleIncr = mData.driftAngleIncrMax;

        mData.driftTimeCounter += dTime;                    //Time drifting

        //if true, drift going left and NPC was right
        if(mData.driftDir == 1.0f){
            //if spin is going in the same direction as when it began (right)
            if(mData.spin_inc < 0){
                mData.angle  -= dTime*mData.driftAngleIncr;
            }

            //Transitionate 1/8 when not turning
            if(mData.spin_inc == 0)
            {
                //mData.angle += dTime*mData.driftAngleIncr*0.125;
                mData.driftWallAngle = mData.angle;
            }

            //When turning to initial angle position, turn at a 1/4 of the turning speed
            if(mData.spin_inc > 0){
                mData.angle  += dTime*mData.driftAngleIncr*0.25;
                if(mData.angle > mData.driftWallAngle)
                    mData.angle = mData.driftWallAngle;
            }

            //Update velocity with 90º vector (if time drifting is less than 1)
            if(mData.driftTimeCounter <= 1.0f)
            {
                mData.velocity.x += mData.vel*cos(mData.angle + mData.driftDesplaceAngle) * mData.driftTimeCounter;
                mData.velocity.z += mData.vel*sin(mData.angle + mData.driftDesplaceAngle)/2 * mData.driftTimeCounter;
            }
            else
            {
                mData.velocity.x += mData.vel*cos(mData.angle + mData.driftDesplaceAngle);
                mData.velocity.z += mData.vel*sin(mData.angle + mData.driftDesplaceAngle)/2;
            }

        }

        //if true drift is going right and NPC was left
        if(mData.driftDir == -1.f){
            //if spin is positive, it is going in the same direction as when it began ( left)
            if(mData.spin_inc > 0){
                mData.angle  += dTime*mData.driftAngleIncr;
            }
            
            //Transitionate 1/8 when not turning
            if(mData.spin_inc == 0)
            {
                //mData.angle -= dTime*mData.driftAngleIncr*0.125;
                mData.driftWallAngle = mData.angle;
            }

            //When turning to initial angle position, turn at a 1/4 of the turning speed
            if(mData.spin_inc < 0){
                mData.angle  -= dTime*mData.driftAngleIncr*0.25;
                if(mData.angle < mData.driftWallAngle)
                    mData.angle = mData.driftWallAngle;
            }

            //Update velocity with 90º vector
            if(mData.driftTimeCounter <= 1.0f)
            {
                mData.velocity.x += mData.vel*cos(mData.angle - mData.driftDesplaceAngle) * mData.driftTimeCounter;
                mData.velocity.z += mData.vel*sin(mData.angle - mData.driftDesplaceAngle)/2 * mData.driftTimeCounter;
            }
            else
            {
                mData.velocity.x += mData.vel*cos(mData.angle - mData.driftDesplaceAngle);
                mData.velocity.z += mData.vel*sin(mData.angle - mData.driftDesplaceAngle)/2;
            }
        }
    }

    //if collided, stop it
    if(mData.coll || !mData.drift)
    {
        mData.driftTimeCounter = 0.f;
        mData.driftDir         = 0.f;
        mData.driftWallAngle   = 0.f;
        mData.driftAngleIncr   = 0.3f;
        mData.drift            = false;
    }
}

//Updates the deviation in velocity caused by a collision
void LAPAL::updateCollisionMovement ( LAPAL::movementData& mData, const float dTime) {

    const float cons = 5; //factor of vel reduction over time
    const float min = 0.03; //minimum velocity of colVel

    if(abs(mData.colVel.x) > min || abs(mData.colVel.z) > min) {

        mData.velocity.x += mData.colVel.x*2;
        mData.velocity.z += mData.colVel.z*2;

        mData.colVel.x -= mData.colVel.x*dTime*cons;
        mData.colVel.z -= mData.colVel.z*dTime*cons;
        mData.invul = false;
    }
    else{
        mData.colVel = { 0, 0, 0};
    }

}


//--------------------------------------
//-------------COLLISIONS---------------
//--------------------------------------

//Checks 3D collision between circles
bool LAPAL::checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const float& length1, const LAPAL::vec3f& pos2,const float& radius2, const float& length2) {
    if ( ( pos2.x-pos1.x ) * ( pos2.x-pos1.x )  + ( pos2.z-pos1.z ) * ( pos2.z-pos1.z ) < ( radius1 + radius2 ) * ( radius1 + radius2) ) // = (x² + z²)<tRadius²
        if( abs(pos2.y-pos1.y) < (length1 + length2) )
            return true;
    return false;
} 


//Assuming there's collision, changes velocity of every object after collision
void LAPAL::calculateElasticCollision(LAPAL::vec3f& vel1, const LAPAL::vec3f& pos1, const float& mass1, 
                                        LAPAL::vec3f& vel2, const LAPAL::vec3f& pos2, const float& mass2) {

    //We apply the physical elastic collision formula
    /*
                2⋅m2     ⟨v1−v2,x1−x2⟩                           2⋅m1    ⟨v2−v1,x2−x1⟩
    v′1 = v1 − ------ ⋅ -------------  ⋅(x1−x2)    v'2 = v2 −  ------ ⋅ ------------- ⋅ (x2−x1)
                m1+m2       ∥x1−x2∥^2                           m1+m2     ∥x2−x1∥^2
    */
    
    LAPAL::vec3f auxPos = pos1 - pos2;
    LAPAL::vec3f auxVel = vel1 - vel2;
    auxPos.y = 0;
    vel1 = vel1 - ((2 * mass2) / (mass1 + mass2)) * ( (auxPos.x*auxVel.x + auxPos.z*auxVel.z) / (auxPos.x * auxPos.x + auxPos.z + auxPos.z)) * auxPos;

    auxPos = pos2 - pos1;
    auxVel = vel2 - vel1;
    auxPos.y = 0;
    vel2 = vel2 - ((2 * mass1) / (mass1 + mass2)) * ( (auxPos.x*auxVel.x + auxPos.z*auxVel.z) / (auxPos.x * auxPos.x + auxPos.z + auxPos.z)) * auxPos;

}

//Checks if terrain is horizontal o it's a pendent 
bool LAPAL::checkTerrain(const LAPAL::plane3f& terrain){

    bool state=false;

   if(terrain.p1.y == terrain.p2.y && terrain.p1.y == terrain.p3.y && terrain.p1.y == terrain.p4.y){ 
        state=true; //the plane is horizontal
    }

    return state;
}

//Calculates rotation in X and Z of the plane, assigning the value to rotZ and rotX
void LAPAL::calculateRotationsXZ(LAPAL::plane3f& terrain){
    
    //Calculate terrain normal
    LAPAL::vec3f v1 = terrain.p2 - terrain.p1;
    LAPAL::vec3f v2 = terrain.p3 - terrain.p1;

    //Plane equation Ax + By + Cz + D = 0
    float A = v1.y * v2.z - v1.z * v2.y;
    float B = v1.z * v2.x - v1.x * v2.z;
    float C = v1.x * v2.y - v1.y * v2.x;
    float D = -( terrain.p1.x * A + terrain.p1.y * B + terrain.p1.z * C );

    //Calculate three points from the plane (0,y,0), (1,y,0) and (0,y,1)
    auto p0 = glm::vec3( 0, -D / B, 0);
    auto pX = glm::vec3( 1, (-A-D) / B, 0);
    auto pZ = glm::vec3( 0, (-C-D) / B, 1);

    //Calculate line in x and angle with x axis (1,0,0)
    auto lX = pX-p0;
    float cosX = lX.x / sqrt(lX.x*lX.x + lX.y*lX.y + lX.z*lX.z);
    terrain.rotZ = acos(cosX);

    //Calculate line in z and angle with z axis (0,0,1)
    auto lZ = pZ-p0;
    float cosZ = lZ.z / sqrt(lZ.x*lZ.x + lZ.y*lZ.y + lZ.z*lZ.z);
    terrain.rotX = acos(cosZ);

}

//Calculates expected Y for the object given its position
float LAPAL::calculateExpectedY(const LAPAL::plane3f& terrain, const LAPAL::vec3f& position ){
    if(checkTerrain(terrain)){
        return terrain.p1.y;
    }else{
        //Scalars that determine the position Y of the object given the vectors needed to compose its position in X and Z
        float a = 0.f, b = 0.f;

        //Using auxiliar function to calculate them
        calculateTerrainAB(terrain, position, a, b);

        //Returns the Y value correspondent
        return a * (terrain.p3 - terrain.p1).y + b  * (terrain.p4 - terrain.p2).y + terrain.p1.y;
    }
}

//Corrects position of the object on Y
void LAPAL::correctYPosition(LAPAL::movementData& mData, const float dTime, LAPAL::plane3f& terrain, LAPAL::vec3f& position){
    float y = calculateExpectedY(terrain, position);
    
    if(!mData.asc){
        //check if we are not touching the ground
        if(position.y>y){
             
            //update vertical speed with gravity
            mData.velY += gravity*dTime*mData.velY*0.05;
        
            //update falling position
            position.y -= mData.velY* dTime;
            if(position.y < y)  //Correct position if we get under it
                position.y = y; 
        
            //if terrain is plane, reset gravity
            if(position.y == y)
                mData.velY = gravity;
        
            //if gravity is too high, cap gravity
            if(mData.velY > gravity * 20)
                mData.velY = gravity * 20;
        
        }else{
            position.y = y;
        }
    }
    mData.jump = false;
}

//Reflects the velocity given a line
void LAPAL::calculateReflectedVector(LAPAL::vec3f& vel, const LAPAL::vec3f& p1, const LAPAL::vec3f& p2) {

    LAPAL::vec3f lineVector = p2-p1;
    LAPAL::vec3f normal = glm::vec3(-lineVector.z, 0, lineVector.x);    //Get normal from the two points of the line

    float nMod = sqrt(normal.x*normal.x + normal.z*normal.z);

    normal = glm::vec3(normal.x/nMod, 0, normal.z/nMod);    //Normalize normal

    float dotVelNormal = vel.x*normal.x + vel.z*normal.z;   //Obtain product between vel and normal

    vel.x = vel.x - 2 * dotVelNormal * normal.x;
    vel.z = vel.z - 2 * dotVelNormal * normal.z;

}

//--------------------------------------
//---------------MATHS------------------
//--------------------------------------

//Calculates values A and B which are the scalars that multiply vector 1 and 2 to compose the point C in 2D (X-Z plane) inside the terrain givenç
void LAPAL::calculateAB(const LAPAL::vec3f& vecC, const LAPAL::vec3f& vec1, const LAPAL::vec3f& vec2,  float& a, float& b){
    double a_x = vec2.x;
    double a_z = vec2.z;
    double b_x = vec1.x;
    double b_z = vec1.z;
    double c_x = vecC.x;
    double c_z = vecC.z;
    double aux_a = 0;
    double aux_b = 0;
    
 
    //Composing vectors a * vec1 +  b * vec2 = vecC
    if(b_x * a_z != b_z * a_x){
        aux_a = (c_z * a_x - c_x * a_z) / (b_z * a_x - b_x * a_z);
    }
    if(a_z != 0.0){
        aux_b = (c_z - aux_a * b_z) / a_z;
    }
    if(a_x != 0.0){
        aux_b = (c_x - aux_a * b_x) / a_x;
    }

    a = aux_a;
    b = aux_b;
}


//Calculates values A and B in given terrain, using Cross vectors (from p1 to p3 and from p2 to p4 OR down-left to up-right and up-left to down-right)
void LAPAL::calculateTerrainAB(const LAPAL::plane3f& terrain, const LAPAL::vec3f& position, float& a, float& b){

    //Vector that will compose the position inside the terrain
    glm::vec3 vec_a     =   terrain.p3 - terrain.p1;
    glm::vec3 vec_b     =   terrain.p4 - terrain.p2;
    glm::vec3 relativeP = position - terrain.p1;

    //Composing the scalars
    calculateAB(relativeP, vec_a, vec_b, a, b);
}

//Calculates the distance between a line defined by two points (l1,l2) and a point (p1)
float LAPAL::distance2DLinePoint(const LAPAL::vec3f& l1, const LAPAL::vec3f& l2, const LAPAL::vec3f& p1) {

    LAPAL::vec3f lineVec = l2-l1;
    LAPAL::vec3f circVec = l2-p1;

    float lineVecMod = sqrt(lineVec.x*lineVec.x + lineVec.z*lineVec.z);
    float circVecMod = sqrt(circVec.x*circVec.x + circVec.z*circVec.z);

    float vectorCos = (lineVec.x*circVec.x + lineVec.z*circVec.z)/(lineVecMod*circVecMod);

    float distance = vectorCos*circVecMod;

    return distance;

}

//Calculates if a point (p1) is left o right of a line defined by two points (l1,l2)
bool LAPAL::position2DLinePoint(const LAPAL::vec3f& l1, const LAPAL::vec3f& l2, const LAPAL::vec3f& p1) {

    //We use a formula infered from the equation of the line (A * x + B * y + C = 0)
    float A = -( l2.z - l1.z );
    float B = l2.x - l1.x;
    float C = -( A * l1.x + B * l1.z );

    float D = A * p1.x + B * p1.z + C;

    if (D > 0)
        return false;   // p1 is left of the line
    else 
        return true;    // p1 is right of the line (or in the line)

}

//Calculates if a circle is inside a rectangle
bool LAPAL::checkCircleRectangleCollision(const LAPAL::plane3f& terrain, const LAPAL::vec3f& nextPosition, const float length, const float length2) {

    if( abs(LAPAL::calculateExpectedY(terrain, nextPosition)-nextPosition.y)-((length + length2)/2) > 1)
        return false;

    //Check if the object is to the right side (clockwise) to the rectangle for every border, failing in one case means being outside the area
    if ( !LAPAL::position2DLinePoint(terrain.p1, terrain.p2, nextPosition) ) 
        return false;
    if ( !LAPAL::position2DLinePoint(terrain.p2, terrain.p3, nextPosition) ) 
        return false;
    if ( !LAPAL::position2DLinePoint(terrain.p3, terrain.p4, nextPosition) ) 
        return false;
    if ( !LAPAL::position2DLinePoint(terrain.p4, terrain.p1, nextPosition) ) 
        return false;

    return true;

}

//calculate angle a-b and a-c
float LAPAL::calculateAngleVector(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    glm::vec3 ab = b-a;
    ab = glm::normalize(ab);

    glm::vec3 ac = c-a;
    ac = glm::normalize(ac);

    float dot = glm::dot(ab, ac);
    float angleCos = glm::acos(dot);
    
    if(dot > 0)
    {
        ab = glm::rotateY(ab, (float)-M_PI/2);
        if(glm::dot(ab, ac) > 0)
        {
            angleCos = 2*M_PI - angleCos;
        }
    }
    else if(dot < 0)
    {
        ab = glm::rotateY(ab, (float)M_PI/2);
        if(glm::dot(ab, ac) < 0)
        {
            angleCos = 2*M_PI - angleCos;
        }
    }

    return angleCos;
}

