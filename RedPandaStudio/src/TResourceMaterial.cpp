#include "TResourceMaterial.h"

//Since the material is usually loaded from an obj, we are never loading it from a route by itself
bool TResourceMaterial::loadResource()
{
    return false;
}

//Loads a material data from an assimp already loaded material
bool TResourceMaterial::loadResource(aiMaterial* m)
{
    aiColor3D v;

    //First we get space for the tree arrays
    kd = (float *)malloc(sizeof(float) * 3);
    ka = (float *)malloc(sizeof(float) * 3);
    ks = (float *)malloc(sizeof(float) * 3);

    //We store the material difuse
    m->Get(AI_MATKEY_COLOR_DIFFUSE, v);
    kd[0]=v.r;
    kd[1]=v.g;
    kd[2]=v.b;
    
    //We store the material ambient
    m->Get(AI_MATKEY_COLOR_AMBIENT, v);
    ka[0]=v.r;
    ka[1]=v.g;
    ka[2]=v.b;
    
    //We store the material specular
    m->Get(AI_MATKEY_COLOR_SPECULAR, v);
    ks[0]=v.r;
    ks[1]=v.g;
    ks[2]=v.b;

    //Finally, we store the material shininess
    m->Get(AI_MATKEY_SHININESS, ns);

    return true;
    
}

//Draw of the material
void TResourceMaterial::draw()
{
    //First we get all the uniform locations
    GLuint diffuseID = glGetUniformLocation(TEntity::getProgramID(), "material.kd");
    GLuint ambientID = glGetUniformLocation(TEntity::getProgramID(), "material.ka");
    GLuint specularID = glGetUniformLocation(TEntity::getProgramID(), "material.ks");
    GLuint shininessID = glGetUniformLocation(TEntity::getProgramID(), "material.ns");

    //And then we give the shader the material information
    glUniform3fv(diffuseID, 1, kd);
    glUniform3fv(ambientID, 1, ka);
    glUniform3fv(specularID, 1, ks);
    glUniform1f(shininessID, ns);
}