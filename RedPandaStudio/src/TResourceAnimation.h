#pragma once

#include "TResource.h"
#include "TResourceOBJ.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

//This class is only intended to load frame by frame animations, defining each frame in it's own OBJ file.
//The purpose of this class is not loading skeletical or any other type of animations

//At the moment, the route provided should direct to the file, except the final three numbers and the .obj extension. For example:
//Animation000001.obj ... Animation000999.obj -> Animation000
//Only supports a maximum of 999 frames
//Warning: This class doesn't check the existence of all the frames, if you try to load more frames than those that exist, you will cause a segmentation fault
class TResourceAnimation : public TResource {
    public:
        TResourceAnimation(){};
        virtual ~TResourceAnimation();

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the first mesh of the animation
        void draw();

        //Draws the frame given by parameter, if possible
        void draw(unsigned int i);

        //////////////////////////////////////////////////////////////////////
        /////////////////////////// GETTERS && SETTERS ///////////////////////
        //////////////////////////////////////////////////////////////////////
        int getFrames()                                     {   return frames;      };
        void setFrames(int i)                               {   frames = i;         };
        void setTexture(TResourceTexture* t)                {   texture = t;        };

    private:
        //Maximum number of frames (initially set to 60)
        int frames = 60;

        //Array of frames
        std::vector<TResourceOBJ*> objs;

        //Texture of the animation meshes
        TResourceTexture* texture = NULL;

        //Auxiliar function to split strings
        std::vector<std::string> split(const std::string& s, const char& c);

        //Function to populate textures to all the frames, without needing to load them multiple times
        void populateTextures();
};