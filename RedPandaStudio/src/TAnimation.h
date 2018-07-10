#pragma once

#include "TEntity.h"
#include "TResourceAnimation.h"
#include <iostream>

class TAnimation : public TEntity{
    public:
        //Constructor and destructor
        TAnimation() {}
        ~TAnimation() {}

        //Draw Methods
        void beginDraw();
        void endDraw() {}

        //Animation control methods

        //Plays a no-loop animation that is currently paused
        void playNoLoop();
        void reset();       //Resets animation state

        //Updates the animation
        void update(double eTime);

        //////////////////////////////////////////////////////////////////////
        ////////////////////// GETTERS & SETTERS /////////////////////////////
        //////////////////////////////////////////////////////////////////////

        TResourceAnimation* getAnimation()                      {   return animation;               };
        int getFrames()                                         {   return frames;                  };
        bool getPauseState()                                    {   return playing;                 };
        bool getLoopState()                                     {   return loop;                    };
        double getFramerate()                                   {   return framerate;               };
        void setAnimation(TResourceAnimation* a)                {   animation = a;                  };
        void setFrames(int i)                                   {   frames = i;                     };
        void setPause(bool b)                                   {   playing = b;                    };
        void setLoop(bool b)                                    {   loop = b;                       };
        void setFramerate(double s)                             {   framerate = s;                  };
        void setRender(bool r)                                  {   render = r;                     };
        

    private:
        //Pointer to the resource of the animation
        TResourceAnimation* animation;

        //Maximum number of frames (initially set to 60)
        int frames = 60;
        //Pointer to current frame
        int pointer = 0;
        //Framerate of the animation
        double framerate = 1/24;
        //Elapsed time since the previous frame
        double elapsedTime = 0;

        //Pauses or plays the animation
        bool playing = true;
        //Controls if the animation is played in loop or not
        bool loop = true;
        //Controls if animation is drawn
        bool render = false;

};