#pragma once

#include "TEntity.h"
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <vector>

//////////////////////////////////////////POSITION CORRESPONDENCY/////////////////////////////////////////
//This class loads a set of six pictures and draws the skybox. The order of the pictures is the following:
// 0 -> Front
// 1 -> Back
// 2 -> Up
// 3 -> Down
// 4 -> Right
// 5 -> Left

//This could be improved with an enumerator

/////////////////////////////////////////////////////////////////////////////////////////////////////////


//All this class is based on the skybox example provided in the superbible of OpenGL, and also in the example code provided in the book
class TResourceSkybox {
    public:
        //Constructor and destructor
        TResourceSkybox();
        ~TResourceSkybox() {};

        //Loads the texture given in the route and stores it in the position specified (check position correspondency for more info)
        bool loadResource(const char* route, int i);

        //Loads all the textures in OpenGL (doesn't check if they are already loaded, error prone)
        bool initSkybox();

        //Draws the skybox
        void draw();

        /////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////// GETTERS && SETTERS ////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////
        void setView(GLuint v)              { view=v;   }

    private:
        //Vector of the 6 textures that compose the skybox, in the order showed in position correspondency
        std::vector<sf::Image*> textures;

        //Identifier of the texture of the skybox
        GLuint texture;

        //Identifier of the viewMatrix of the shader
        GLuint view;

};