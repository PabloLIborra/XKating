#pragma once
#include "TEntity.h"
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <glm/ext.hpp>

class TCamera : public TEntity {
    public:
        TCamera(float z);
        ~TCamera() {}


        //////////////////////////////////////////////////////////
        //////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////
        bool isParallel()                                          {   return parallel;    };
        void setParallel(bool b)                                   {   parallel=b;         };

        void setCameraParameters(float l, float r, float t, float b, float n, float f);

        void beginDraw();
        void endDraw() {}


    private:
        // Definition of the camera
        float left, right, top, bottom, nnear, ffar;
        //Parallel or perspective; False == persective, true == parallel
        bool parallel;
        GLfloat zoom;

};