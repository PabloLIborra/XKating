#pragma once

#include "TResource.h"
#include "TEntity.h"
#include <GL/glew.h>
#include <assimp/scene.h>

class TResourceMaterial : public TResource {
    public:
        //Constructor and destructor
        TResourceMaterial() {};
        ~TResourceMaterial() {};

        //Load the material from an already loaded material provided in assimp format
        bool loadResource(aiMaterial* m);

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the material
        void draw();

    private:
        //Diffuse, ambient and specular component of the material
        GLfloat* kd, *ka, *ks;

        //Glossiness of the material (specular exponent)
        GLfloat ns=0.0f;

};