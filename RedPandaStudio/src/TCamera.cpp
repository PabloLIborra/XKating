#include "TCamera.h"

TCamera::TCamera(float z)
{
    parallel=false;

    //Camera parameters default initialization. Those values are currently not in use in our game (with the exception of nnear, ffar and zoom)
    left=1.f;
    right=2.f;
    top=1.f;
    bottom=2.f;
    nnear=0.1f;
    ffar=10000.f;
    zoom = z;

    projectionMatrix() = glm::perspective(glm::radians(zoom), (float)16 / (float)9, nnear, ffar);
}

void TCamera::setCameraParameters(float l, float r, float t, float b, float n, float f)
{
    left=l;
    right=r;
    top=t;
    bottom=b;
    nnear=n;
    ffar=f;

    projectionMatrix() = glm::perspective(zoom, (float)16 / (float)9, nnear, ffar);
}

void TCamera::beginDraw()
{
    
}