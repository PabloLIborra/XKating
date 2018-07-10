#pragma once

#include "TResource.h"
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

//The functioning of this class should be very similar to the funcioning of TResourceTexture, with the
//difference that this texture should be drawn in the spot 1 and not the 0, to avoid texture collisions
//between diffuse maps and normal maps
class TResourceNormalTexture : public TResource {
    public:
        //Constructor and destructor
        TResourceNormalTexture() {};
        ~TResourceNormalTexture();

        //Load the resource specified in the route provided (stored in the father class)
        bool loadResource();

        //Draws the texture
        void draw();

    private:
        //OpenGL ID of the texture
        GLuint textureID;
        //Defines if we have a loaded texture ready to be drawn
        bool active = false;

};