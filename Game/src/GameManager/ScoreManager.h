#pragma once

#include <vector>
#include <cstdint>
#include "../GlobalVariables.h"
#include "../GameObject/ScoreComponent.h"
#include "../GameObject/StartLineComponent.h"

class ScoreManager {
    public:
        //Constructor
        ScoreManager();

        //Destructor
        ~ScoreManager();

        //Initializer
        void init();

        //Update : This method  updates the position of every player
        void update();

        //Close
        void close();

        //Static class getter
        static ScoreManager& getInstance();

        //Component creator
        IComponent::Pointer createScoreComponent(GameObject& newGameObject);

        IComponent::Pointer createStartLineComponent(GameObject& newGameObject);


        //Getters
        std::vector<ScoreComponent::Pointer>& getPlayers() {return players;}
        int getMaxLaps() {return maxLaps;}

        //Setters
        void setMaxLaps(int i) {maxLaps=i;}


    private:
        std::vector<ScoreComponent::Pointer> players;
        std::vector<StartLineComponent::Pointer> startLines;
        int maxLaps;
        int playerLap = 1, playerPosition = 1;
};