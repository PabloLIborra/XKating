#include "WaypointManager.h"

//==============================================
// DELEGATES DECLARATIONS
//==============================================

void objectDeletePathPlanning(EventData eData);

//==============================================
// MAIN FUNCTIONS
//==============================================

WaypointManager::WaypointManager()
{

}

WaypointManager::~WaypointManager()
{
    delete listSubNodes;
}

void WaypointManager::init() {
    listSubNodes = new std::vector<GameObject::Pointer>;

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeletePathPlanning});
}

void WaypointManager::update(float dTime) {
    auto player = GlobalVariables::getInstance().getPlayer();
    auto posPlayer = player->getTransformData().position;

    //CALCULATE LOD PATHPLANNING
    for(unsigned int i=0; i < pathPlanningComponentList.size(); i++)
    {
        auto AIObject = pathPlanningComponentList[i]->getGameObject();
        auto posAI = AIObject.getTransformData().position; 


        auto distPlayerAI = (posPlayer.x - posAI.x) * (posPlayer.x - posAI.x) +
                            (posPlayer.y - posAI.y) * (posPlayer.y - posAI.y) +
                            (posPlayer.z - posAI.z) * (posPlayer.z - posAI.z);

        //IF DISTANCE PLAYER-AI IS BIGER THAN DISTANCELOD, NOT UPDATE
        float distanceLoD = GlobalVariables::getInstance().getDistanceLoD();
        if(distPlayerAI <= distanceLoD*distanceLoD || distanceLoD == 0)
        {
            //auto pathPlanning = std::dynamic_pointer_cast<PathPlanningComponent>(pathPlanningComponentList[i]).get();
            updatePathPlanning(pathPlanningComponentList[i], dTime);
        }
    }
}

void WaypointManager::close() {
    waypointComponentList.clear();
    listSubNodes->clear();
}

IComponent::Pointer WaypointManager::createWaypointComponent(GameObject::Pointer newGameObject, float r, int lvl)
{
    //Create shared pointer
    IComponent::Pointer component = std::make_shared<WaypointComponent>(*newGameObject.get(), r, lvl);

    //Add component to the object
    newGameObject.get()->addComponent(component);

    //Add to list of waypoints
    listSubNodes->push_back(newGameObject);

    //Search for its place on the list of waypoints
    for(unsigned int i=0;i<listSubNodes->size();i++){
        auto radius1 = listSubNodes->at(i).get()->getComponent<WaypointComponent>()->getLevel();
        for(unsigned int x=i+1;x<listSubNodes->size()-1;x++){
            //bubble sort
            auto radius2 = listSubNodes->at(x).get()->getComponent<WaypointComponent>()->getLevel();
            if(radius1>radius2){
                auto aux=listSubNodes->at(i);
                listSubNodes->at(i)=listSubNodes->at(x);
                listSubNodes->at(x)=aux;
            }
        }
    }

    return component;
}

IComponent::Pointer WaypointManager::createPathPlanningComponent(GameObject::Pointer newGameObject, std::vector<GameObject::Pointer>& list)
{
    //Make shared pointer of path plannign component
    IComponent::Pointer component = std::make_shared<PathPlanningComponent>(*newGameObject.get(), list);

    //Add component to the object
    newGameObject.get()->addComponent(component);

    //add to the list of components
    pathPlanningComponentList.push_back(component);

    return component;
}

////////////////////////////////////////////
//
//      UPDATE PATHPLANNING
//
////////////////////////////////////////////

void WaypointManager::updatePathPlanning(IComponent::Pointer pathPlanning, float dTime)
{
    auto iItemComponent = pathPlanning->getGameObject().getComponent<IItemComponent>().get();
    if(iItemComponent == nullptr)
    {
        pathPlanning->update(dTime); 
    }
}

//==============================================
//Constructor and Destructor
//==============================================
void objectDeletePathPlanning(EventData eData) {

    auto& PathPlanningComponentList = WaypointManager::getInstance().getPathPlanningList();
    //Seach for component if given ID
    for(unsigned int i = 0; i<PathPlanningComponentList.size(); ++i) {
        if(eData.Id == PathPlanningComponentList[i].get()->getGameObject().getId()) {
            //and erase it
            PathPlanningComponentList.erase(PathPlanningComponentList.begin() + i);
            return;
        }
    }
} 
