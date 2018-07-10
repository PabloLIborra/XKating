#include "TMesh.h"

//Begin Draw of the mesh
void TMesh::beginDraw()
{
    if(mesh)
    {
        //First we pass the model matrix to the shader
        glUniformMatrix4fv(TEntity::modelID, 1, GL_FALSE, &modelMatrix()[0][0]);
        glm::mat4 m = viewMatrix() * modelMatrix();
        //Then, the modelView matrix
        glUniformMatrix4fv(TEntity::modelViewID, 1, GL_FALSE, &m[0][0]);
        m = projectionMatrix() * m;
        //And finally, the MVP Matrix
        glUniformMatrix4fv(TEntity::mvpID, 1, GL_FALSE, &m[0][0]);
        //And then, at last, we draw the mesh
        mesh->draw();
    }
}