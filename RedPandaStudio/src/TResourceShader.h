#pragma once

#include "TResource.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <vector>

class TResourceShader : public TResource {
    public:
        //Constructor and destructor
        TResourceShader(){};
        ~TResourceShader(){};

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the shader (in this case, activates it)
        void draw();

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////  GETTERS && SETTERS
        //////////////////////////////////////////////////////////////////////////////////////////

        void setShaderType(GLenum e)            {   shaderType=e;       };
        GLuint getShaderID()                    {   return shaderID;    };
        GLenum getShaderType()                  {   return shaderType;  };

    private:
        //ID of the shader
        GLuint shaderID;
        //Defines the type of shader
        GLenum shaderType;


};