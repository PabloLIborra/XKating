#include "TTransform.h"

//Loads a identity matrix in the class matrix
bool TTransform::identity()
{
    matrix = glm::mat4(1.0);
    return true;
}

//Transposes the class matrix
bool TTransform::transpose()
{
    matrix = glm::transpose(matrix);
    return true;
}

//Translates the class matrix by a x y z vector
bool TTransform::translate(float x, float y, float z)
{
    matrix = glm::translate(matrix, glm::vec3(x,y,z));
    return true;
} 

//Rotates the matrix around the axis provided with the x y z vector
bool TTransform::rotate(float x, float y, float z, float w)
{
    glm::mat4 rotateY = glm::rotate(matrix, w, glm::vec3(x, y, z));
    matrix = rotateY;
    return true;
} 

//Scales the matrix by x y z values
bool TTransform::scale(float x, float y, float z) 
{
    matrix = glm::scale(matrix, glm::vec3(x,y,z));
    return true;
}

//Inverses the matrix
bool TTransform::inverse()
{
    matrix = glm::inverse(matrix);
    return true;
}

//Returns the multiplication of the matrix by a specific vector
glm::vec4 TTransform::multiplyVector(glm::vec4 v)
{
    return (matrix * v);
} 

//Returns the multiplication of the matrix by a specific matrix
glm::mat4 TTransform::multiplyMatrix(glm::mat4 m)
{
    return (matrix * m);
}

//Begin draw of the transform. Pushes the previous matrix to the stack
//and multiplies the class matrix by it's current value
void TTransform::beginDraw()
{
    glm::mat4& mM= modelMatrix();
    matrixStack().push(mM);
    mM = matrix * mM;
}

//Restores the previous matrix by popping the matrix stack
void TTransform::endDraw()
{
    glm::mat4& mM= modelMatrix();
    mM = matrixStack().top();
    matrixStack().pop();
}