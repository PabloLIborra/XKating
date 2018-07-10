#pragma once

#include "TEntity.h"
#include "TResource.h"
#include "TResourceMesh.h"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <glm/ext.hpp>

class TResourceOBJ : public TResource {
    public:
        TResourceOBJ() {};
        virtual ~TResourceOBJ();

        //Only loads the meshes in the OBJ provided
        bool loadOnlyMeshes();

        //Sets the provided texture in the mesh placed in position i, if possible
        void setTexture(unsigned int i, TResourceTexture* t);

        //Sets the provided material in the mesh placed in position i, if possible
        void setMaterial(unsigned int i, TResourceMaterial* m);

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the mesh
        void draw();

        ///////////////////////////////////////////////////////////////////
        //////////////////////// GETTERS && SETTERS ///////////////////////
        ///////////////////////////////////////////////////////////////////
        
        std::vector<TResourceMesh*> getMeshes() {       return meshes;      }
        

    private:
        //Array of meshes contained in this OBJ
        std::vector<TResourceMesh*> meshes;

        //Maximum X, Y and Z values of the OBJ. They define the first of the two points needed
        GLfloat maxX, maxY, maxZ;
        //Minimum X, Y and Z values of the OBJ. They define the second of the points needed
        GLfloat minX, minY, minZ;
        //VBO Buffer handler for the bounding box's vertex
        GLuint boxVBOVertices;
        //IBO Buffer handler for the bounding box's vertex
        GLuint boxIBOIndices;
        //Transform of the bounding box
        glm::mat4 bbTransform;

        //Auxiliar function to split strings
        std::vector<std::string> split(const std::string& s, const char& c);

        //Generates bounding box. Must be called after generating the bounding boxes of all the meshes contained in the obj
        void generateBoundingBox();
        void drawBoundingBox();
        bool checkBoundingBox();
};
