#include "VObject.h"
#include "VSensorComponent.h"


//Constructors
VObject::VObject()
{
    a=0;
    b=0;
    radius=0;
    type=0;
    length=0;
}

VObject::VObject(glm::vec3 p, float x, float y, float r, int t, float l)
{
    position=p;
    a=x;
    b=y;
    radius=r;
    type=t;
    length=l;
}

//Destructor
VObject::~VObject()
{

}

//Methods
void VObject::nearAB(float* a, float* b, GameObject& myPos)
{
    glm::vec3 myPosition = myPos.getTransformData().position;
    float distance = (position.x - myPosition.x) * (position.x - myPosition.x) +
                    (position.y - myPosition.y) * (position.y - myPosition.y) +
                    (position.z - myPosition.z) * (position.z - myPosition.z);

    glm::vec3 nearPoint = (glm::sqrt((radius*radius)/distance)) * (position - myPosition) + position;

    myPos.getComponent<VSensorComponent>()->calculateAB(nearPoint, *a, *b);

}