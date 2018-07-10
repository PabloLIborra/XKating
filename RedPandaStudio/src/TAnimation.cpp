#include "TAnimation.h"
#include <iostream>

//Begin draw method of the animation. Used to transfer uniform matrix to the shader
void TAnimation::beginDraw()
{
    if(animation && render)
    {
        glUniformMatrix4fv(TEntity::modelID, 1, GL_FALSE, &modelMatrix()[0][0]);
        glm::mat4 m = viewMatrix() * modelMatrix();
        glUniformMatrix4fv(TEntity::modelViewID, 1, GL_FALSE, &m[0][0]);
        m = projectionMatrix() * m;
        glUniformMatrix4fv(TEntity::mvpID, 1, GL_FALSE, &m[0][0]);
        
        animation->draw(pointer);
    }
}

//Plays an animation that is set in a no loop state only one time, from the beginning
void TAnimation::playNoLoop()
{
    if(!playing)
    {
        pointer = 0;
        playing = true;
    }
}

//Updates the animation pointer to frame
void TAnimation::update(double eTime)
{
    //First we check that the animation is playing
    if(playing)
    {
        //Then we advance the animation if the elapsed time is enough, based on the animation framerate
        elapsedTime += eTime;
        if(elapsedTime > framerate)
        {
            pointer++;
            elapsedTime = 0;
        }
        //If its a no-loop animation and has played completely, it stops
        if(pointer == frames - 2  && loop == false)
        {
            playing = false;
        }
        //If it's a loop animation, we reset it
        if(pointer>=frames-1)
        {
            pointer = 0;
        }
    }
}

//Auxiliar method to reset an animation to the beginning, withouth checking anything more
void TAnimation::reset() {
    pointer = 0;
}