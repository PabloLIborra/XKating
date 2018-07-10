#include "TResourceMesh.h"

//Destructor of the mesh
TResourceMesh::~TResourceMesh()
{
    glDeleteBuffers(6, vboHandles);
    glDeleteVertexArrays(1, &vaoHandles);
    glDeleteBuffers(1, &boxIBOIndices);
    glDeleteBuffers(1, &boxVBOVertices);
}

//Loads a mesh from an already loaded by assimp mesh (usually given by TResourceOBJ)
bool TResourceMesh::loadMesh(aiMesh* m)
{
    int nFaces = m->mNumFaces;
    nTriangles = nFaces;
    nVertex = m->mNumVertices;

    //Generate an array of 6 buffer identifiers
    vboHandles = (unsigned int *)malloc(sizeof(unsigned int) * 6);
    glGenBuffers(6, vboHandles);
    
    vertex = new float[(sizeof(float) * nVertex * 3)];
    memcpy(&vertex[0], m->mVertices, 3 * sizeof(float) * nVertex);

    //Bind and pass to OpenGL the first array (vertex coordinates)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), vertex, GL_STATIC_DRAW);

    if(m->HasNormals())
    {
        //normals = (float *)malloc(sizeof(float) * nVertex * 3);
        memcpy(&vertex[0], m->mNormals, 3 * sizeof(float) * nVertex);

        //Bind and pass to OpenGL the second array (vertex normals)
        glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
        glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), vertex, GL_STATIC_DRAW);
    }

    //We assume we are always working with triangles
    vertexIndices = (unsigned int *)malloc(sizeof(unsigned int) * nFaces * 3);
    unsigned int faceIndex = 0;


    for(int j = 0; j<nFaces; j++, faceIndex += 3)
    {
        vertexIndices[0+faceIndex] = m->mFaces[j].mIndices[0];
        vertexIndices[1+faceIndex] = m->mFaces[j].mIndices[1];
        vertexIndices[2+faceIndex] = m->mFaces[j].mIndices[2];
    }

    if(m->HasTextureCoords(0))
    {
        //textures=(float *)malloc(sizeof(float)*2*nVertex);
        for(unsigned int k = 0; k<nVertex;k++)
        {
            vertex[k*2] = m->mTextureCoords[0][k].x;
            vertex[k*2+1] = m->mTextureCoords[0][k].y;
        }
        //Bind and pass to OpenGL the third array (vertex texture coordinates)
        glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
        glBufferData(GL_ARRAY_BUFFER, nVertex*2*sizeof(float), vertex, GL_STATIC_DRAW);
    }

    if(m->HasTangentsAndBitangents())
    {
        //tangents = (float *)malloc(sizeof(float)*3*nVertex);
        //bitangents = (float *)malloc(sizeof(float)*3*nVertex);
        memcpy(&vertex[0], m->mTangents, 3 * sizeof(float) * nVertex);

        //Bind and pass to OpenGL the fourth array (vertex tangent)
        glBindBuffer(GL_ARRAY_BUFFER, vboHandles[3]);
        glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), vertex, GL_STATIC_DRAW);

        memcpy(&vertex[0], m->mBitangents, 3 * sizeof(float) * nVertex);

        //Bind and pass to OpenGL the fifth array (vertex bitangent)
        glBindBuffer(GL_ARRAY_BUFFER, vboHandles[4]);
        glBufferData(GL_ARRAY_BUFFER, nVertex*3*sizeof(float), vertex, GL_STATIC_DRAW);
    }
    
    //=============================================================================
    //Generate an array of a vertex array identifier
    glGenVertexArrays(1, &vaoHandles);
    glBindVertexArray(vaoHandles);

    //==============================================
    //BIND VAO
    glBindVertexArray(vaoHandles);
    //==============================================  


    //Detach elements
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //=============================================================================

    //Generates the two points needed for a parallel-to-edges bounding box
    generateBoundingBox();

    //Bind and pass to OpenGL the fourth array (vertex indices)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles*3*sizeof(unsigned int), vertexIndices, GL_STATIC_DRAW);


    delete[] vertex;
    free(vertexIndices);

    return true;

}

//Loads a mesh from the route provided. Currently, this method is not in use, and it's deprecated
bool TResourceMesh::loadResource()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_FlipUVs );

    nTriangles = 0;

    if(scene)
    {
        for(unsigned int i = 0; i<scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];
            int nFaces = mesh->mNumFaces;
            //We get the vertex, normals and textures arrays and prepare them for OpenGL calls
            
            nTriangles += nFaces;
            nVertex = mesh->mNumVertices;

            vertex = (float *)malloc(sizeof(float) * nVertex * 3);
            memcpy(&vertex[0], mesh->mVertices, 3 * sizeof(float) * nVertex);

            if(mesh->HasNormals())
            {
                normals = (float *)malloc(sizeof(float) * nVertex * 3);
                memcpy(&normals[0], mesh->mNormals, 3 * sizeof(float) * nVertex);
            }

            //We assume we are always working with triangles
            vertexIndices = (unsigned int *)malloc(sizeof(unsigned int) * nFaces * 3);
            unsigned int faceIndex = 0;

            for(int j = 0; j<nFaces; j++)
            {
                memcpy(&vertexIndices[faceIndex], mesh->mFaces[j].mIndices, 3 * sizeof(unsigned int));
                faceIndex += 3;
            }
            if(mesh->HasTextureCoords(0))
            {
                textures=(float *)malloc(sizeof(float)*2*nVertex);
                for(unsigned int k = 0; k<nVertex;k++)
                {
                    textures[k*2] = mesh->mTextureCoords[0][k].x;
                    textures[k*2+1] = mesh->mTextureCoords[0][k].y;
                }
            }

        }
        return true;
    }
    return false;
}

//Draw of the mesh. It also checks the frustum culling if it's activated
void TResourceMesh::draw()
{
    if((TEntity::getFrustumCulling() && checkBoundingBox()) || !TEntity::getFrustumCulling())
    {
        GLuint id = glGetUniformLocation(TEntity::getProgramID(), "textActive");
        glUniform1i(id, textActive);

        GLuint normalID = glGetUniformLocation(TEntity::getProgramID(), "normalActive");
        glUniform1i(normalID, normalActive);

        //First we draw the texture of our mesh
        if(texture!=NULL && textActive)
        {
            texture->draw();
        }
        
        if(material!=NULL)
        {
            material->draw();
        }

        if(normalTexture!=NULL && normalActive)
        {
            normalTexture->draw();
        }

    //==============================================
    //BIND VAO
    glBindVertexArray(vaoHandles);
    //==============================================  
    
    //Bind and pass to OpenGL the first array (vertex coordinates)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    //Bind and pass to OpenGL the second array (vertex normals)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    //Bind and pass to OpenGL the third array (vertex texture coordinates)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    //Bind and pass to OpenGL the fourth array (vertex tangent)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[3]);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    //Bind and pass to OpenGL the fifth array (vertex bitangent)
    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[4]);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    //Bind and pass to OpenGL the fourth array (vertex indices)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[5]);

    //We order to draw here
    glDrawElements(GL_TRIANGLES, nTriangles*3, GL_UNSIGNED_INT, 0);


    //==============================================    
    //Detach elements
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //==============================================

    //drawBoundingBox();

    }
}

//Generates a bounding box for the mesh
void TResourceMesh::generateBoundingBox()
{
    //In first case, we initialize all the variables to the first vertex coordinates
    minX = vertex[0];
    minY = vertex[1];
    minZ = vertex[2];
    maxX = vertex[0];
    maxY = vertex[1];
    maxZ = vertex[2];

    //After that, we look for the highest and lowest values in each axis
    for(int i = 0; i < nVertex * 3; i+=3)
    {
        if(vertex[i] > maxX)
            maxX=vertex[i];
        if(vertex[i] < minX)
            minX=vertex[i];
        if(vertex[i+1] > maxY)
            maxY = vertex[i+1];
        if(vertex[i+1] < minY)
            minY = vertex[i+1];
        if(vertex[i+2] > maxZ)
            maxZ = vertex[i+2];
        if(vertex[i+2] < minZ)
            minZ = vertex[i+2];
    }

    //std::cout << "maxX: " << maxX << " minX: " << minX << std::endl;
    //std::cout << "maxY: " << maxY << " minY: " << minY << std::endl;
    //std::cout << "maxZ: " << maxZ << " minZ: " << minZ << std::endl;

    glm::vec3 size = glm::vec3(maxX-minX, maxY-minY, maxZ-minZ);
    glm::vec3 center = glm::vec3((minX+maxX)/2, (minY+maxY)/2, (minZ+maxZ)/2);
    bbTransform = glm::translate(glm::mat4(1), center) *  glm::scale(glm::mat4(1), size);
    
    //1x1x1 box centered on origin
    GLfloat boxVertices[] = {
    -0.5, -0.5, -0.5, 1.0,
     0.5, -0.5, -0.5, 1.0,
     0.5,  0.5, -0.5, 1.0,
    -0.5,  0.5, -0.5, 1.0,
    -0.5, -0.5,  0.5, 1.0,
     0.5, -0.5,  0.5, 1.0,
     0.5,  0.5,  0.5, 1.0,
    -0.5,  0.5,  0.5, 1.0,
  };

  glGenBuffers(1, &boxVBOVertices);
  glBindBuffer(GL_ARRAY_BUFFER, boxVBOVertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLushort boxIndices[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 1, 5, 2, 6, 3, 7
  };

  glGenBuffers(1, &boxIBOIndices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxIBOIndices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

//Draws the bounding box of the mesh. This is only used for debug purposes, and should be disabled in release versions of the game
void TResourceMesh::drawBoundingBox()
{
    glm::mat4 m = TEntity::modelMatrix() * bbTransform;

    glUniformMatrix4fv(TEntity::getModelID(), 1, GL_FALSE, &m[0][0]);

    //========================================
    //Bind VAO
    glBindVertexArray(vaoHandles);
    //========================================
    glBindBuffer(GL_ARRAY_BUFFER, boxVBOVertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxIBOIndices);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    //========================================
    //Closing VAO binding
    glBindVertexArray(0);
    //========================================
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(TEntity::getModelID(), 1, GL_FALSE, &TEntity::modelMatrix()[0][0]);

}

//Checks the frustum culling with the bounding box, and also checks some special cases if the first test doesn't pass
bool TResourceMesh::checkBoundingBox()
{
    //First we set the bounding box's points in the scene
    glm::mat4 m = TEntity::projectionMatrix() * TEntity::viewMatrix() * TEntity::modelMatrix() * bbTransform;
    glm::vec4 p1 = m * glm::vec4(-0.5, -0.5, -0.5, 1.0);
    glm::vec4 p2 = m * glm::vec4(0.5, -0.5, -0.5, 1.0);
    glm::vec4 p3 = m * glm::vec4(0.5, 0.5, -0.5, 1.0);
    glm::vec4 p4 = m * glm::vec4(-0.5, 0.5, -0.5, 1.0);
    glm::vec4 p5 = m * glm::vec4(-0.5, -0.5, 0.5, 1.0);
    glm::vec4 p6 = m * glm::vec4(0.5, -0.5, 0.5, 1.0);
    glm::vec4 p7 = m * glm::vec4(0.5, 0.5, 0.5, 1.0);
    glm::vec4 p8 = m * glm::vec4(-0.5, 0.5, 0.5, 1.0);


    //Then we check if atleast one point is inside the view frustum
    if(p1.x >= -p1.w && p1.x <= p1.w && p1.y >= -p1.w && p1.y <= p1.w && p1.z >= -p1.w && p1.z <= p1.w)
    {
        return true;
    }
    if(p2.x >= -p2.w && p2.x <= p2.w && p2.y >= -p2.w && p2.y <= p2.w && p2.z >= -p2.w && p2.z <= p2.w)
    {
        return true;
    }
    if(p3.x >= -p3.w && p3.x <= p3.w && p3.y >= -p3.w && p3.y <= p3.w && p3.z >= -p3.w && p3.z <= p3.w)
    {
        return true;
    }
    if(p4.x >= -p4.w && p4.x <= p4.w && p4.y >= -p4.w && p4.y <= p4.w && p4.z >= -p4.w && p4.z <= p4.w)
    {
        return true;
    }
    if(p5.x >= -p5.w && p5.x <= p5.w && p5.y >= -p5.w && p5.y <= p5.w && p5.z >= -p5.w && p5.z <= p5.w)
    {
        return true;
    }
    if(p6.x >= -p6.w && p6.x <= p6.w && p6.y >= -p6.w && p6.y <= p6.w && p6.z >= -p6.w && p6.z <= p6.w)
    {
        return true;
    }
    if(p7.x >= -p7.w && p7.x <= p7.w && p7.y >= -p7.w && p7.y <= p7.w && p7.z >= -p7.w && p7.z <= p7.w)
    {
        return true;
    }
    if(p8.x >= -p8.w && p8.x <= p8.w && p8.y >= -p8.w && p8.y <= p8.w && p8.z >= -p8.w && p8.z <= p8.w)
    {
        return true;
    }


    //If not a single point is in the view frustum, we check that all the points are outside at the same time from atleast one
    //of the planes. This is to fix fake negatives in cases where the bounding volume is higher than the frustum camera (which happens with our map)
    if(p1.x < -p1.w && p2.x < -p2.w && p3.x < -p3.w && p4.x < -p4.w && p5.x < -p5.w && p6.x < -p6.w && p7.x < -p7.w && p8.x < -p8.w)
    {
        return false;
    }
    if(p1.x > p1.w && p2.x > p2.w && p3.x > p3.w && p4.x > p4.w && p5.x > p5.w && p6.x > p6.w && p7.x > p7.w && p8.x > p8.w)
    {
        return false;
    }
    if(p1.y < -p1.w && p2.y < -p2.w && p3.y < -p3.w && p4.y < -p4.w && p5.y < -p5.w && p6.y < -p6.w && p7.y < -p7.w && p8.y < -p8.w)
    {
        return false;
    }
    if(p1.y > p1.w && p2.y > p2.w && p3.y > p3.w && p4.y > p4.w && p5.y > p5.w && p6.y > p6.w && p7.y > p7.w && p8.y > p8.w)
    {
        return false;
    }
    if(p1.z < -p1.w && p2.z < -p2.w && p3.z < -p3.w && p4.z < -p4.w && p5.z < -p5.w && p6.z < -p6.w && p7.z < -p7.w && p8.z < -p8.w)
    {
        return false;
    }
    if(p1.z > p1.w && p2.z > p2.w && p3.z > p3.w && p4.z > p4.w && p5.z > p5.w && p6.z > p6.w && p7.z > p7.w && p8.z > p8.w)
    {
        return false;
    }


    return true;
}

//This functions looks for a specific adjacent vertex for the vertex indices. Due to the computational cost, this should be improved
//using the half-edge structure, to avoid a massive number of searchs through the mesh
//In the last version of the engine, silhouette rendering is not done with a geometry shader anymore, so the use of this function is
//deprecated.
unsigned int TResourceMesh::getAdjacentIndex(aiMesh* m, const unsigned int index1, const unsigned int index2, const unsigned int index3)
{
    for(unsigned int i=0; i < m->mNumFaces; i++)
    {
        for(int edge = 0; edge < 3; edge++)
        {
            unsigned int v1 = m->mFaces[i].mIndices[edge];
            unsigned int v2 = m->mFaces[i].mIndices[(edge+1)%3];
            unsigned int v3 = m->mFaces[i].mIndices[(edge+2)%3];

            if( ((m->mVertices[v1].x == m->mVertices[index1].x && m->mVertices[v1].y == m->mVertices[index1].y && m->mVertices[v1].z == m->mVertices[index1].z && 
                m->mVertices[v2].x == m->mVertices[index2].x && m->mVertices[v2].y == m->mVertices[index2].y && m->mVertices[v2].z == m->mVertices[index2].z) ||
                (m->mVertices[v2].x == m->mVertices[index1].x && m->mVertices[v2].y == m->mVertices[index1].y && m->mVertices[v2].z == m->mVertices[index1].z && 
                m->mVertices[v1].x == m->mVertices[index2].x && m->mVertices[v1].y == m->mVertices[index2].y && m->mVertices[v1].z == m->mVertices[index2].z)) &&
                (m->mVertices[v3].x != m->mVertices[index3].x || m->mVertices[v3].y != m->mVertices[index3].y || m->mVertices[v3].z != m->mVertices[index3].z))
            {
                return v3;
            }
        }
    }
    return -1;
}