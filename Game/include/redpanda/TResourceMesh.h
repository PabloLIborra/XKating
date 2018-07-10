#pragma once

#include "TEntity.h"
#include "TResource.h"
#include "TResourceMaterial.h"
#include "TResourceTexture.h"
#include "TResourceNormalTexture.h"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/ext.hpp>


class TResourceMesh : public TResource {
    public:
        //Constructor and destructor
        TResourceMesh() {};
        virtual ~TResourceMesh();

        //Copies the data needed from an already loaded in memory mesh (loaded with assimp)
        bool loadMesh(aiMesh* m);

        //Load the resource specified in the route provided
        bool loadResource();

        //Draws the mesh
        void draw();

        //////////////////////////////////////////////////////////////////////////////////////////
        //////////  GETTERS && SETTERS
        //////////////////////////////////////////////////////////////////////////////////////////

        void setVertex(GLfloat* v)                                  {       vertex=v;                     }
        void setNormals(GLfloat* n)                                 {       normals=n;                    }
        void setTextures(GLfloat* t)                                {       textures=t;                   }
        void setIndices(GLuint* i)                                  {       vertexIndices=i;              }
        void setNTriangles(long n)                                  {       nTriangles=n;                 }
        void setNVertex(long n)                                     {       nVertex=n;                    }
        void setTexture(TResourceTexture* t)                        {       texture=t;                    }
        void setTextActive(bool b)                                  {       textActive=b;                 }
        void setNormalTexture(TResourceNormalTexture* t)            {       normalTexture = t;            }
        void setNormalActive(bool b)                                {       normalActive = b;             }
        void setMaterial(TResourceMaterial* m)                      {       material=m;                   }

        GLfloat  getMaxX()                                          {       return maxX;                  }
        GLfloat  getMinX()                                          {       return minX;                  }
        GLfloat  getMaxY()                                          {       return maxY;                  }
        GLfloat  getMinY()                                          {       return minY;                  }
        GLfloat  getMaxZ()                                          {       return maxZ;                  }
        GLfloat  getMinZ()                                          {       return minZ;                  }
        GLfloat* getVertex()                                        {       return vertex;                }
        long     getNVertex()                                       {       return nVertex;               }
        GLuint*  getVertexIndices()                                 {       return vertexIndices;         }

    private:
        //Vertex info
        GLfloat* vertex, *normals, *textures, *tangents, *bitangents;
        //Vertex indices
        GLuint* vertexIndices;
        //Number of faces (assuming faces are triangles, what we currently are forcing)
        long nTriangles = 0;
        //Number of vertex
        long nVertex;
        //Texture asociated to this mesh
        TResourceTexture* texture = NULL;
        //Control variable to see if the texture is active
        bool textActive=0;
        //Normal texture asociated to this mesh
        TResourceNormalTexture* normalTexture = NULL;
        //Control variable to see if the normal texture is active
        bool normalActive = 0;
        //Material asociated to this mesh
        TResourceMaterial* material = NULL;
        /*
        //Buffer handles
        GLuint* vboHandles;
        */
        //======================
        //Buffer handles
        GLuint* vboHandles, vaoHandles;
        //======================

        ///////////////////////////////////////
        //// Bounding box asociated data //////
        //Maximum X, Y and Z values of the mesh. They define the first of the two points needed
        GLfloat maxX, maxY, maxZ;
        //Minimum X, Y and Z values of the mesh. They define the second of the points needed
        GLfloat minX, minY, minZ;
        //VBO Buffer handler for the bounding box's vertex
        GLuint boxVBOVertices;
        //IBO Buffer handler for the bounding box's vertex
        GLuint boxIBOIndices;
        //Transform of the bounding box
        glm::mat4 bbTransform;

        //Auxiliar functions for generating bounding box and calculating frustum culling
        void generateBoundingBox();
        void drawBoundingBox();
        bool checkBoundingBox();

        //Auxiliar function to generate the vertex triangle adjacency vertex index buffer
        unsigned int getAdjacentIndex(aiMesh* m, const unsigned int index1, const unsigned int index2, const unsigned int index3);
};  