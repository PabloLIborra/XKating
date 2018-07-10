#pragma once
#include "TEntity.h"

class TTransform : public TEntity{

private:
    
    glm::mat4 matrix;

public:
    //Constructor and destructor
    TTransform() {}
    ~TTransform() {}

    //Basic transformation methods for the matrix
    bool translate(float x, float y, float z);
    bool rotate(float x, float y, float z, float w);
    bool scale(float x, float y, float z);

    //Advanced transformation methods for the matrix
    bool inverse();
    bool identity();
    bool transpose();

    //Methods used to multiply the matrix by different data
    glm::vec4 multiplyVector(glm::vec4 v);
    glm::mat4 multiplyMatrix(glm::mat4 m);

    //Setters
    void setMatrix(glm::mat4 m)         {       matrix = m;         }

    //Getters 
    glm::mat4 getMatrix()               {       return matrix;      }

    //Draw Methods
    void beginDraw();
    void endDraw();

};