#include "ScoreManager.h"
#include "ObjectManager.h"
#include "../Game.h"

void objectDeleteScore(EventData);


//==============================================
// MAIN FUNCTIONS
//==============================================
ScoreManager::ScoreManager()
{
    //Maximum number of laps
    maxLaps = 3;
}

ScoreManager::~ScoreManager()
{

}

void ScoreManager::init()
{
    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteScore});
}

void ScoreManager::close()
{
    players.clear();
}

//Static getter
ScoreManager& ScoreManager::getInstance() {
    static ScoreManager instance;
    return instance;
}

//Component creator
IComponent::Pointer ScoreManager::createScoreComponent(GameObject& newGameObject){
    //Make shared pointer of the score component
    ScoreComponent::Pointer component = std::make_shared<ScoreComponent>(newGameObject);

    //Attach to game object
    newGameObject.addComponent(component);

    //Push it into the list of players
    players.push_back(component);

    return component;
}

IComponent::Pointer ScoreManager::createStartLineComponent(GameObject& newGameObject){
    //Make shared pointer of the start line component
    StartLineComponent::Pointer component = std::make_shared<StartLineComponent>(newGameObject);

    //Attach to game object
    newGameObject.addComponent(component);

    //Push it into the list of start lines
    startLines.push_back(component);

    return component;
}

//Thirty programmers have died during the development of this method
void ScoreManager::update()
{
    //Initial variables
    std::vector<ScoreComponent::Pointer> ordered;
    std::vector<ScoreComponent::Pointer> auxiliar;
    uint32_t j, pos;
    int score;
    bool found;

    pos=1;
    //If there is more than one player
    if(players.size()>0)
    {
        for(unsigned int i=0; i<players.size(); i++)
        {
            if(i==0)
            {
                ordered.push_back(players[i]);
            }
            else
            {
                found=false;
                score = players[i].get()->getScore();
                for(j=0; j<ordered.size() && found==false; j++)
                {
                    if((score>ordered[j].get()->getScore()) || (score == ordered[j].get()->getScore() && players[i].get()->getActualDistance() < ordered[j].get()->getActualDistance()))
                    {
                        ordered.insert(ordered.begin()+j, players[i]);
                        found=true;
                    }
                }
                if(found==false)
                {
                    ordered.push_back(players[i]);
                }
            }
        }
        for(unsigned int i=0; i<ordered.size(); i++)
        {
            ordered[i].get()->setPosition(pos);
            pos++;
        }
        players=ordered;
    }

    //Get player for the update, and there is one guardian
    GameObject* player = GlobalVariables::getInstance().getPlayer();
    if(player == nullptr)
        return;
    
    //Score component and there is one guardian
    ScoreComponent* scoreC = player->getComponent<ScoreComponent>().get();
    if(scoreC == nullptr)
        return;
    
    int position = scoreC->getPosition();
    int lap = scoreC->getLap();

    if(position > playerPosition){

        EventData data;
        data.Component      = std::static_pointer_cast<IComponent>(player->getComponent<MoveComponent>());

        EventManager::getInstance().addEvent(Event {EventType::Score_OnOvertaken, data});

        playerPosition = position;
    } 
    if(position < playerPosition){

        EventData data;
        data.Component      = std::static_pointer_cast<IComponent>(player->getComponent<MoveComponent>());

        EventManager::getInstance().addEvent(Event {EventType::Score_OnOvertake, data});

        playerPosition = position;
    }
    if(lap > playerLap){

        uint16_t mID = 0;

        if(lap == 2)
            mID = 0;
        else if ( lap == 3 )
            mID = 4;
        else if(lap > 3 && playerPosition < 4)
            mID = 1;
        else
            mID = 2;

        //Send event of new lap
        EventData data;
        data.Component      = std::static_pointer_cast<IComponent>(player->getComponent<MoveComponent>());
        data.Id             = mID;
        EventManager::getInstance().addEvent(Event {EventType::Score_OnNewLap, data});

        //Assign player lap to the player
        playerLap = lap;
        
        //If final lap, send victory event
        if(lap == 4) {
            Game::getInstance().setState(IGameState::stateType::POSTMATCH);
        }
    } 


}


//==============================================
// DELEGATES
//==============================================
void objectDeleteScore(EventData eData) {

    auto& scoreComponentList = ScoreManager::getInstance().getPlayers();

    for(unsigned int i = 0; i<scoreComponentList.size(); ++i) {
        if(eData.Id == scoreComponentList[i].get()->getGameObject().getId()) {
            scoreComponentList.erase(scoreComponentList.begin() + i);
            return;
        }
    }
}