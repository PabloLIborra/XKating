#include "TResourceOBJ.h"
#include <string>
#include <iostream>

//Auxiliar method used to split strings. Obtained from the internet
std::vector<std::string> TResourceOBJ::split(const std::string& s, const char& c) {
	std::string buff{""};
	std::vector<std::string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

//Destructor
TResourceOBJ::~TResourceOBJ()
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
    meshes.clear();
}

//Loads only the meshes in the scene, ignoring the materials and textures. This is used for the animations, to avoid loading one different material for every frame
bool TResourceOBJ::loadOnlyMeshes()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name, aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

    if(scene)
    {
        for(unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            TResourceMesh* mesh = new TResourceMesh();
            aiMesh* m = scene->mMeshes[i];
            mesh->loadMesh(m);
            meshes.push_back(mesh);
        }

        generateBoundingBox();
        
        return true;
    }
    return false;
}

//Sets a texture for a specified mesh. This is used for the animations.
void TResourceOBJ::setTexture(unsigned int i, TResourceTexture* t)
{
    if(i>=0 && i<meshes.size())
    {
        meshes[i]->setTextActive(true);
        meshes[i]->setTexture(t);
    }
}

//Sets a material for a specified mesh. This is used for the animations
void TResourceOBJ::setMaterial(unsigned int i, TResourceMaterial* m)
{
    if(i >= 0 && i < meshes.size())
    {
        meshes[i]->setMaterial(m);
    }
}

//Loads the OBJ contained in the name variable.
bool TResourceOBJ::loadResource()
{
    Assimp::Importer importer;
    //First we attempt to load the obj
    const aiScene* scene = importer.ReadFile(name, aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);


    //If loaded succesfully, we proceed to get all his data
    if(scene)
    {
        //We get the directory path to load properly the textures
        std::string s(name);
        std::vector<std::string> v = split(s, '/');
        std::string route;

        if(v[0].compare("home") == 0)
            route = "/";

        for(unsigned int i = 0; i < v.size()-1; i++)
        {
            route+=v[i] + "/";
        }

        //For each mesh in the obj, we create a new resourceMesh and storage the mesh data on it
        for(unsigned int i = 0; i<scene->mNumMeshes; i++)
        {
            TResourceMesh* mesh = new TResourceMesh();
            aiMesh* m = scene->mMeshes[i];
            mesh->loadMesh(m);
            meshes.push_back(mesh);

            TResourceMaterial* mat = new TResourceMaterial();
            mat->loadResource(scene->mMaterials[m->mMaterialIndex]);
            mesh->setMaterial(mat);
            aiString path;
            
            //Now we load the diffuse texture asociated to this mesh
            if(scene->mMaterials[m->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                TResourceTexture* texture = new TResourceTexture();
                
                //First we combine the path we just got with the directory path of the obj, and then we just load the texture
                std::string completePath = route + path.data;
                texture->setName(completePath.c_str());
                texture->loadResource();
                mesh->setTexture(texture);
                mesh->setTextActive(true);
            }

            //And now we load the normal texture, identified by assimp as height texture, asociated to this mesh
            if(scene->mMaterials[m->mMaterialIndex]->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
            {
                TResourceNormalTexture* normalTexture = new TResourceNormalTexture();

                //First we combine the path we just got with the directory path of the obj, and then we just load the texture
                std::string completePath = route + path.data;
                normalTexture->setName(completePath.c_str());
                normalTexture->loadResource();
                mesh->setNormalTexture(normalTexture);
                mesh->setNormalActive(true);
            }
        }
        
        //Finally we generate the bounding box of the model, in case it's needed for the frustum culling
        generateBoundingBox();


        return true;
    }
    return false;
}

//Draws every mesh asociated to this OBJ
void TResourceOBJ::draw()
{
    if((TEntity::getFrustumCulling() && checkBoundingBox()) || !TEntity::getFrustumCulling())
    {
        //The textures, materials and meshes are loaded, suposedly, in a way that they should just correspond, so we draw one of each
        for(unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i]->draw();
        }
    }
}

void TResourceOBJ::generateBoundingBox()
{

    //First we initialize the variables of the bounding box
    maxX = meshes[0]->getMaxX();
    minX = meshes[0]->getMinX();
    maxY = meshes[0]->getMaxY();
    minY = meshes[0]->getMinY();
    maxZ = meshes[0]->getMaxZ();
    minZ = meshes[0]->getMinZ();

    for(unsigned int i = 1; i < meshes.size(); i++)
    {
        if(meshes[i]->getMaxX() > maxX)
            maxX = meshes[i]->getMaxX();
        if(meshes[i]->getMinX() < minX)
            minX = meshes[i]->getMinX();
        if(meshes[i]->getMaxY() > maxY)
            maxY = meshes[i]->getMaxY();
        if(meshes[i]->getMinY() < minY)
            minY = meshes[i]->getMinY();
        if(meshes[i]->getMaxZ() > maxZ)
            maxZ = meshes[i]->getMaxZ();
        if(meshes[i]->getMinZ() < minZ)
            minZ = meshes[i]->getMinZ();
    }

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

//Function used only for debug purposes. Draws a cube (not polygonal, only the edges) surrounding the OBJ
void TResourceOBJ::drawBoundingBox()
{
    glm::mat4 m = TEntity::modelMatrix() * bbTransform;

    glUniformMatrix4fv(TEntity::getModelID(), 1, GL_FALSE, &m[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, boxVBOVertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxIBOIndices);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(TEntity::getModelID(), 1, GL_FALSE, &TEntity::modelMatrix()[0][0]);
}

//Checks if the bounding box is in the frustum, for frustum culling purposes
bool TResourceOBJ::checkBoundingBox()
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