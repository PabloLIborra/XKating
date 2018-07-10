#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "../GameObject.h"
#include "../../GameManager/RenderManager.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

//Initilizer
void MoveComponent::init() {
}

//Update
void MoveComponent::update(float dTime) {
    //get transform data
    auto position = getGameObject().getTransformData().position;

    //Update all movement conditions and visuals
    updateMaxSpeedOverTime(dTime);                      //Update speed difference over time (for powerups)
    LAPAL::updateRotation(mData, terrain, dTime);       //Updates rotation of object (axis and visual)

    //Update horizontal movement
    LAPAL::updateLinearVelocity(mData, dTime, terrain); //Update linear velocity
    LAPAL::updateSpin(mData, dTime);                    //Update spin (turning)
    LAPAL::updateVelocity(mData, terrain);              //Update velocity (vector)
    LAPAL::updateEllipticMovement(mData, dTime);        //Update elliptic movement (if object is drifting)
    LAPAL::updateCollisionMovement(mData, dTime);       //Update deviation in velocity after a collision

    //Correct vertical movement
    updateJump(mData, position, terrain);
    LAPAL::correctYPosition(mData, dTime, terrain, position);

    bool animPlaying = RenderManager::getInstance().getRenderFacade()->isAnimationPLaying(getGameObject().getId());
    bool movement = (abs(mData.vel) > 2.5);

    //Update run animation velocity
    if(movement && moving && !colliding && !drifting && !itemThrown) {
        if(mData.vel > 0) {
            float ratio = mData.vel / mData.max_vel;
            ratio = 1/(10 + 30 * ratio * ratio);
            RenderManager::getInstance().getRenderFacade()->setFramerate(getGameObject().getId(), ratio);
        }
    }
    //Update animation depending on speed
    if(movement && !moving && !colliding && !drifting && !itemThrown) {
        moving = true;
        RenderManager::getInstance().getRenderFacade()->changeAnimation(getGameObject().getId(), 1);
    }
    else if(!movement && moving && !colliding && !drifting && !itemThrown) {
        moving = false;
        RenderManager::getInstance().getRenderFacade()->changeAnimation(getGameObject().getId(), 0);
    }
    //Update animation depending on collisions
    if(mData.coll) {
        RenderManager::getInstance().getRenderFacade()->changeAnimation(getGameObject().getId(), 2);
        RenderManager::getInstance().getRenderFacade()->loopOnceAnimation(getGameObject().getId());
        colliding = true;
        movingOnCollision = (movement);
    }
    else if(colliding && !animPlaying) {
        colliding = false;
        animPlaying = true;
        if(movingOnCollision == mData.mov && !itemThrown)
            moving = !moving;
    }
    //Update animation depending on item
    auto cmp = getGameObject().getComponent<ItemHolderComponent>();
    int itemType = -1;

    //Update animation depending on item
    if(getGameObject().getComponent<ItemHolderComponent>() != nullptr)
    {
        int itemType = (getGameObject().getComponent<ItemHolderComponent>()).get()->getItemType();
        if(itemType == -1 && (item == 0 || item == 1)){
            itemThrown = true;
            if(mData.mov){
                RenderManager::getInstance().getRenderFacade()->changeAnimation(getGameObject().getId(), 4);
                RenderManager::getInstance().getRenderFacade()->loopOnceAnimation(getGameObject().getId());
            }
            else{
                RenderManager::getInstance().getRenderFacade()->changeAnimation(getGameObject().getId(), 3);
                RenderManager::getInstance().getRenderFacade()->loopOnceAnimation(getGameObject().getId());
            }
                
            movingOnItem = mData.mov;
        }
        else if(itemThrown && !animPlaying) {
            itemThrown = false;
            animPlaying = true;
            if(movingOnItem == mData.mov && !colliding)
                moving = !moving;
        }

        item = itemType;
    }
        
    //Update animation depending on drift
    if(mData.drift && !drifting) {
        drifting = true;
        if(mData.spin <= 0)
            RenderManager::getInstance().getRenderFacade()->changeAnimation(getGameObject().getId(), 5);
        else 
            RenderManager::getInstance().getRenderFacade()->changeAnimation(getGameObject().getId(), 6);
    }
    else if(!mData.drift && drifting){
        drifting = false;
        if(!colliding)
            moving = false;
    }
    

    //MUSIC EVENTS RELATED TO PLAYER
    if(getGameObject().getId() == 25000 && mData.vel > 20 && !mData.drift) {
        float vel = mData.vel / mData.max_vel;
        EventData eD;
        eD.Component = getGameObject().getComponent<MoveComponent>();
        eD.grade = vel;
        EventManager::getInstance().addEvent(Event {EventType::Player_Speed, eD});
    }
    else if(getGameObject().getId() == 25000 && mData.drift) {
        EventData eD;
        eD.Component = getGameObject().getComponent<MoveComponent>();
        EventManager::getInstance().addEvent(Event {EventType::Player_Slide, eD});
    }

    if(getGameObject().getId() == 25000 && isAscending && !mData.asc) {
        EventData eD;
        eD.Component = getGameObject().getComponent<MoveComponent>();
        EventManager::getInstance().addEvent(Event {EventType::Player_Jump, eD});
    }

    isAscending = mData.asc;

    //Set collision value to false (if it was true it has already been processed)
    mData.coll = false;

    //Get transform data
    auto trans = getGameObject().getTransformData(); 
    
    //Change position
    trans.position = position;
    trans.position += mData.velocity*dTime;

    //Change rotation
    trans.rotation.y = -mData.angle;
    trans.rotation.z = mData.angZ;
    trans.rotation.x = mData.angX;

    //Set final transform of position
    getGameObject().setTransformData(trans);
    
    //===========================================================================================
    // DEBUG
    //if(getGameObject().getId() == 25000){
    //    system("clear");
    //    //std::cout << " GIRO: "<<mData.angX<<","<<mData.angZ<<std::endl;
    //    std::cout << " POS X " << trans.position.x << " POS Z " << trans.position.z <<  " POS Y " << trans.position.y <<std::endl;
    //    //std::cout << " ANG X " << trans.rotation.x << " ANG Y " << trans.rotation.y << " ANG Z " << trans.rotation.z << std::endl;
    //    //std::cout << " POS Y " << trans.position.y << std::endl;
    //    std::cout << " VEL X " << mData.velocity.x << " VEL Z " << mData.velocity.z << std::endl;
    //    std::cout << " INCR ANGLE: " << mData.spin_inc << std::endl;
    //    std::cout << " ANGULO VELOCIDAD: " << mData.spin << std::endl;
    //    std::cout << " ANGULO GIRO: " << mData.angle << std::endl;
    //    std::cout << " Velocidad " << mData.vel << std::endl;
    //    //std::cout << " Terrain angles. X: " << terrain.rotX <<", Z: "<<terrain.rotZ << std::endl;
    //    //std::cout << " VEL Y " << mData.velY << std::endl;
    //
    //
    //    /*if (mData.jump == false){
    //        std::cout << " No estoy saltando " << std::endl;
    //    }
    //    else{
    //        std::cout << " Sí estoy saltando " << std::endl;
    //    }*/
    //}
    
    //=========================================================================================*/
}

//Closer
void MoveComponent::close() {

}

//Physics related functions
void MoveComponent::changeAccInc(float n) {
    mData.dAcc     = n;
}

void MoveComponent::changeSpinIncrement(float n) {
    //Reset spin speed in case of change of direction
    mData.spin_inc = n;
}

void MoveComponent::isMoving(bool m){
    mData.mov      = m;
}

void MoveComponent::isJumping(bool j){
    mData.jump = j;
}
void MoveComponent::isSpinning(bool s){
    mData.spi      = s;
}

void MoveComponent::changeSpin(float n) {
    mData.spin     = mData.max_spin * n;
}

void MoveComponent::changeAcc(float a){
    mData.acc      = mData.max_acc  * a;
}

void MoveComponent::isDrifting(bool d){
    mData.drift          = mData.spin_inc != 0 ? d : false;              //Drifting is true
    mData.driftWallAngle = d ? mData.angle : 0.f;                        //Lock the maximum angle of turn back
    mData.spin           = d ? mData.spin  : mData.spin / 3.f;           //Set turning to a third of the speed to avoid fast rotation after stopping the drift

    //Drift turn preservation
    if(d)
    {
        if(mData.spin_inc < 0)
            mData.driftDir = 1.f;

        if(mData.spin_inc > 0 )
            mData.driftDir = -1.f;

        if(mData.spin_inc == 0)
            mData.driftDir = 0;
    }

    //Speed boost
    if(!d && mData.driftTimeCounter > mData.driftBoostTime)
    {
        mData.driftTimeCounter = 0.f;
        changeMaxSpeedOverTime(mData.driftSpeedBoost, mData.driftConstTime, mData.driftDecTime );   //Apply boost
    }
}

void MoveComponent::isBraking(bool b) {
    mData.braking  = b;
}

void MoveComponent::changeVel(float v){
    mData.vel      = v;
}

void MoveComponent::changeMaxVel(float v){
    mData.max_vel      = v;
}

void MoveComponent::changeInvul(bool i){
    mData.invul      = i;
}

//=================================================
//Functions related with temporal data changes
//=================================================

//Activate temporal speed change
void MoveComponent::changeMaxSpeedOverTime(float maxSpeed, float constTime, float decTime) {

    auto animationRender = this->getGameObject().getComponent<AnimationRenderComponent>();

    if(animationRender!=nullptr && animationRender->getPolyMesh() == ObjectRenderComponent::Poly::High)
    {
        if(mData.max_vel != maxSpeed){
            auxData.max_vel         = mData.max_vel;
            mData.max_vel           = maxSpeed;      
        }
        constantAlteredTime     = constTime;
        decrementalAlteredTime  = decTime;
        maxDecrementalAT        = decTime;
        mData.boost             = true;
    }

}

//Update and interpolate temporal speed change
void MoveComponent::updateMaxSpeedOverTime(const float dTime) {

    auto animationRender = this->getGameObject().getComponent<AnimationRenderComponent>();

    if(animationRender!=nullptr && animationRender->getPolyMesh() == ObjectRenderComponent::Poly::High)
    {
        if(mData.boost && !mData.coll) {
            if(constantAlteredTime > 0) {
                //While time is constant, velocity is constant and maximum
                mData.vel = mData.max_vel;
                constantAlteredTime -= dTime;
            }
            else if (decrementalAlteredTime > 0) {
                //Calculate velocity decrease depending on dTime
                float vel_diff = mData.max_vel - auxData.max_vel;
                float vel      = (dTime*vel_diff)/maxDecrementalAT;

                mData.vel     -= vel; 

                decrementalAlteredTime -= dTime;

                if(decrementalAlteredTime < 0) {
                    mData.max_vel = auxData.max_vel;
                    mData.boost   = false;
                    mData.invul = false;
                }
                    
            }
        }
        else {
            constantAlteredTime = 0;
            decrementalAlteredTime = 0;
            mData.max_vel = auxData.max_vel;

            if(mData.coll)
            {
                mData.boost = false;
            }
        }
    }
}

//Control and update jump
void MoveComponent::updateJump(LAPAL::movementData& mData, glm::vec3& pos, LAPAL::plane3f t){

    float maxJump = 15.0;
    float velJump = 50.0;

    if(mData.jump == true){
        if(abs(LAPAL::calculateExpectedY(t, pos)-pos.y) < 1) { 
            mData.posY = pos.y + maxJump;
            mData.asc = true;
        }
    }
    if(mData.asc == true){
        if(pos.y < mData.posY){
            mData.velocity.y = (velJump*(mData.posY-pos.y)/maxJump)+10; //Interpolate velocity 
                                                                            //(the higher we get, the slower we go up)
        }
        else{
            mData.asc = false;
            mData.velocity.y = 0;
        }
    }
}