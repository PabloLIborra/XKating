#pragma once

#include "TResource.h"
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

class TResourceTexture : public TResource {
    public:
        //Constructor and destructor
        TResourceTexture() {};
        ~TResourceTexture();

        //Load the resource specified in the route provided (stored in the father class)
        bool loadResource();

        //Draws the texture
        void draw();
        
        //Deactivates the current texture
        void endDraw();

        //////////////////////////////////////////////////////////////////////////
        ////////////////////////// GETTERS && SETTERS ////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        GLuint getTextureId() { return textureID; }


    private:
        //OpenGL ID of the texture
        GLuint textureID;
        //Defines if we have a loaded texture ready to be drawn
        bool active = false;
};