#pragma once

#include <vector>
#include <string.h>
#include "TResourceMesh.h"
#include "TResourceMaterial.h"
#include "TResourceTexture.h"
#include "TResourceShader.h"
#include "TResourceOBJ.h"
#include "TResourceLoD.h"
#include "TResourceAnimation.h"
#include <GL/glew.h>

class ResourceManager {
    public:
        ResourceManager() {};
        ~ResourceManager() {};

        //////////////////////////////////////////////////////////////////////
        /////////////// GETTERS & SETTERS
        //////////////////////////////////////////////////////////////////////

        //The following set of functions gets the resource from the vector, and if it's not loaded, 
        //loads it to the vector and returns him. One function specific for each type of resource
        TResourceMesh* getResourceMesh(const char* n);
        TResourceMaterial* getResourceMaterial(const char* n);
        TResourceTexture* getResourceTexture(const char* n);
        TResourceShader* getResourceShader(const char* n, GLenum e);
        TResourceOBJ* getResourceOBJ(const char* n);
        TResourceAnimation* getResourceAnimation(const char* n, int frames, const char* t);
        TResourceLoD* getResourceLoD(const char* n);

        //The following set of functions erases the resource specified in the route n from
        //the vector of resources and then deletes the object, freeing the memory asociated
        void deleteResourceOBJ(const char* n);
        void deleteResourceAnimation(const char* n);


    private:
        std::vector<TResourceMesh*> meshes;
        std::vector<TResourceMaterial*> materials;
        std::vector<TResourceTexture*> textures;
        std::vector<TResourceShader*> shaders;
        std::vector<TResourceOBJ*> objs;
        std::vector<TResourceAnimation*> animations;
        std::vector<TResourceLoD*> lods;

};