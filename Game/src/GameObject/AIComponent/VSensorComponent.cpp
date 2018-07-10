#include "VSensorComponent.h"
#include <iostream>

//Constructors
VSensorComponent::VSensorComponent(GameObject& newGameObject) : 
ISensorComponent(newGameObject)
{
    angleInitial=newGameObject.getTransformData().rotation.y * 3.141592653589f / 180.f;
    angleVision=55.0 * 3.141592653589f / 180.f;
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));
    maxDistance = 100.f;
    maxLength = 20.f;
}       

VSensorComponent::VSensorComponent(GameObject& newGameObject, float angV, float angI, float md, float ml) :
ISensorComponent(newGameObject)
{
    angleInitial=angI;
    angleVision=angV * 3.141592653589f / 180.f;

    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));

    maxDistance = md;
    maxLength = ml;
}

//Checks the objects seen and stores the ones seen in the seenObjects vector
void VSensorComponent::updateSeenObjects(std::vector<GameObject> objects)
{
    //initial variables
    size_t i;                                  //Counter
    VObject::Pointer pvo;                      //VPointer included in the end result
    VObject::Pointer ramp; 
    VObject::Pointer box; 
    float distanceBox = -1, distanceRamp = -1;
    float a = 0.f, b = 0.f;                    //initial a and b (left and right sensor, respectively)


    //Clear seen objects
    seenObjects.clear();

    //Iterate through all available objects
    for(i=0; i<objects.size(); i++)
    {
        glm::vec3 myPos = getGameObject().getTransformData().position;
        float distance = (objects[i].getTransformData().position.x - myPos.x) * (objects[i].getTransformData().position.x - myPos.x) +
						(objects[i].getTransformData().position.y - myPos.y) * (objects[i].getTransformData().position.y - myPos.y) +
						(objects[i].getTransformData().position.z - myPos.z) * (objects[i].getTransformData().position.z - myPos.z);
        

        float distY = (objects[i].getTransformData().position.y - myPos.y) * (objects[i].getTransformData().position.y - myPos.y);

        float rad = objects[i].getComponent<CollisionComponent>()->getRadius();
        float length = objects[i].getComponent<CollisionComponent>()->getRadius();

        if(rad != -1.f)
        {
            distance -= rad;
        }

        if((distance < maxDistance*maxDistance || maxDistance == 0) && (distY < maxLength*maxLength || maxLength == 0))
        {
            calculateAB(objects[i].getTransformData().position, a, b);       //Do the math

            //if both are inside the cone contained by A and B
            if(a > 0 && b > 0)  
            {
                if(objects[i].getComponent<CollisionComponent>()->getType() == CollisionComponent::Type::Default)
                {
                    pvo = std::make_shared<VObject>(objects[i].getTransformData().position, a, b, rad, 0, length); //generate VObject with the data
                    seenObjects.push_back(pvo);                                                     //Add to seen objects
                }
                else if(objects[i].getComponent<CollisionComponent>()->getType() == CollisionComponent::Type::ItemBox)
                {
                    if((distanceBox == -1 || distance < distanceBox) && objects[i].getComponent<ItemBoxComponent>()->getActive() == 1)
                    {
                        distanceBox = distance;
                        box = std::make_shared<VObject>(objects[i].getTransformData().position, a, b, rad, 1, length); //generate VObject with the data
                    } 
                }
                else if(objects[i].getComponent<CollisionComponent>()->getType() == CollisionComponent::Type::Ramp)
                {
                    if(distanceRamp == -1 || distance < distanceRamp)
                    {
                        distanceRamp = distance;
                        ramp = std::make_shared<VObject>(objects[i].getTransformData().position, a, b, rad, 2, length); //generate VObject with the data
                    } 
                }
            }
        }

        //clean A and B for the next object
        a = 0;
        b = 0;
    }
    if(box != nullptr)
    {
        seenObjects.push_back(box);                                     //Add box to seen objects
    }
    if(ramp != nullptr)
    {
        seenObjects.push_back(ramp);                                    //Add ramp to seen objects
    }
}

//Auxiliar function to calculate A and B of given objective
void VSensorComponent::calculateAB(glm::vec3 objective, float& a, float& b){
    glm::vec3 relativeP;
    
    //update sensors
    sensorLeft = glm::vec3(cos(angleVision+angleInitial), 0.f, sin(angleVision+angleInitial));
    sensorRight = glm::vec3(cos(-angleVision+angleInitial), 0.f, sin(-angleVision+angleInitial));

    //Do the math
    relativeP = objective - this->getGameObject().getTransformData().position;
    LAPAL::calculateAB(relativeP, sensorLeft, sensorRight, a, b);
}