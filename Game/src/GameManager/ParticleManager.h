#pragma once

#include <vector>
#include <iostream>

#include <glm/ext.hpp>

class ParticleManager {
    public:

        ParticleManager() {}

        ~ParticleManager() {};

        void init();

        void update();

        void close();

        static ParticleManager& getInstance();

        //====================================================
        //      PARTICLE MANAGERS
        //====================================================
        void createSmokeParticleSystem(uint16_t idToFollow, glm::vec3 offset);
        void createSmokeParticleSystem(glm::vec3 position);
        void createDustParticleSystem(glm::vec3 position);
        void deleteParticleSystem(uint16_t idToFollow);

    private:

        struct particleSystem {
            uint16_t id;
            uint16_t idToFollow;
            glm::vec3 offset;
        };

        std::vector<particleSystem> emitters;

        uint16_t lastId = 35000;

 };
