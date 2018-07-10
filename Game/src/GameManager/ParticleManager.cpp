#include "../GameEvent/EventManager.h"

#include "ParticleManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"

//==============================================
// PARTICLE MANAGER FUNCTIONS
//============================================== 
ParticleManager& ParticleManager::getInstance() {
    static ParticleManager instance;
    return instance;
}

void ParticleManager::init(){
    createSmokeParticleSystem(glm::vec3(-1746, -278,525));
    createSmokeParticleSystem(glm::vec3(-2146, -278,192));
    createDustParticleSystem(glm::vec3(375, -40, 670));
}

void ParticleManager::close(){

    for(unsigned int i = 0; i < emitters.size(); i++) {
        RenderManager::getInstance().getRenderFacade()->deleteObject(emitters[i].id);
    }
    emitters.clear();

}

void ParticleManager::update(){

    for(unsigned int i = 0; i < emitters.size(); i++) {
        if(emitters[i].id != 0) {
            GameObject::TransformationData td = (ObjectManager::getInstance().getObject(emitters[i].idToFollow)).get()->getTransformData();
            td.position += emitters[i].offset;
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(emitters[i].id, td);
        }
        else {
            GameObject::TransformationData td;
            td.position = emitters[i].offset;
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(emitters[i].id, td);
        }
    }

}

void ParticleManager::createSmokeParticleSystem(uint16_t idToFollow, glm::vec3 offset) {

    GameObject::TransformationData td = (ObjectManager::getInstance().getObject(idToFollow)).get()->getTransformData();
    td.position += offset;

    RenderManager::getInstance().getRenderFacade()->createParticleSystem(lastId, "media/mesh/smoke/smoke.obj", td.position, 2, 10, 5,
                                                                        glm::vec3(0,0.1,0), glm::vec3(0,0.07,0), 0.3,        //Direction
                                                                        0.5, 4, 0.5,                                          //Size
                                                                        glm::vec4(0,0,0,1), glm::vec4(1,1,1,1), 0.05);    //Color

    emitters.push_back(particleSystem{lastId, idToFollow, offset});

    lastId++;

}

void ParticleManager::createSmokeParticleSystem(glm::vec3 position) {

    RenderManager::getInstance().getRenderFacade()->createParticleSystem(lastId, "media/mesh/smoke/smoke.obj", position, 2, 10, 5,
                                                                        glm::vec3(0,0.2,0), glm::vec3(0,0.1,0), 0.3,        //Direction
                                                                        0.5, 2, 0.5,                                          //Size
                                                                        glm::vec4(0,0,0,1), glm::vec4(1,1,1,1), 0.05);    //Color

    emitters.push_back(particleSystem{lastId, 0, position});

    lastId++;

}

void ParticleManager::createDustParticleSystem(glm::vec3 position) {

    RenderManager::getInstance().getRenderFacade()->createParticleSystem(lastId, "media/mesh/part/part.obj", position, 100, 50, 10,
                                                                        glm::vec3(0,0.2,0), glm::vec3(0,0.2,0), 0.4,        //Direction
                                                                        2, 2, 0.5,                                          //Size
                                                                        glm::vec4(0.5,0,0.5,1), glm::vec4(0.5,0,0.5,1), 0.4);    //Color

    emitters.push_back(particleSystem{lastId, 0, position});

    lastId++;

}

void ParticleManager::deleteParticleSystem(uint16_t idToFollow) {

    unsigned int j = -1;

    for(unsigned int i = 0; i < emitters.size(); i++) {
        if(emitters[i].idToFollow == idToFollow) {
            RenderManager::getInstance().getRenderFacade()->deleteObject(emitters[i].id);
            j = i;
        }
    }

    emitters.erase(emitters.begin()+j);

}