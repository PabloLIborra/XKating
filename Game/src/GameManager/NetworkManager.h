#pragma once

//C++ includes
#include <iostream>
#include <memory>
#include <string>

//Raknet includes
#include <raknet/RakPeerInterface.h> 
#include <raknet/MessageIdentifiers.h>
#include <raknet/BitStream.h>

//Basic info includes
#include "../GameServer/CustomIdentifiers.h"

//Component && extra includes
#include "../GameObject/ItemComponent/IItemComponent.h"
#include "../GameObject/NetworkComponent/RemotePlayerComponent.h"
#include "../GameObject/NetworkComponent/RemoteItemComponent.h"

//Manager includes
#include "PhysicsManager.h"
#include "ItemManager.h"
#include "ObjectManager.h"

class ItemManager;
class ObjectManager;

class NetworkManager{

public: 

    //Constructor
    NetworkManager() {}

    //Destructor
    ~NetworkManager() {}

    //Initialization
    void init();

    //Update
    void update();

    //Shutdown
    void close();

    //Static class getter
    static NetworkManager& getInstance();

    //Initializes lobby
    void initLobby();

    //Updates the lobby
    void updateLobby();

    //==============================================================
    // Create component
    //==============================================================
    //Create remote player component
    IComponent::Pointer createRemotePlayerComponent(GameObject& newGameObject);

    //Create remote player component
    IComponent::Pointer createRemoteItemComponent(GameObject& newGameObject, int type);

    //==============================================================
    // Player functions
    //==============================================================

    //Sends input from the player to the server so it executes actions
    void sendInput(EventType event);

    //Collocates the player in the map
    void createPlayer(RakNet::Packet* packet);

    //Creates a remote player in our game
    void createRemotePlayer(RakNet::Packet* packet);

    //Update players position
    void updatePlayersPosition(RakNet::Packet* packet);

    //==============================================================
    //  MOVE STUFF
    //==============================================================

    //Broadcasts the player position to the rest of players
    void broadcastPosition();

    //Receives a remote player position
    void moveRemotePlayer(RakNet::Packet* packet);

    //Broadcasts all the red shells position to the rest of players
    void broadcastPositionRedShell();

    //Receives a remote red shell position
    void moveRemoteRedShell(RakNet::Packet* packet);

    //Broadcasts the blue shells position to the rest of players
    void broadcastPositionBlueShell();

    //Receives a remote blue shell position
    void moveRemoteBlueShell(RakNet::Packet* packet);

    //==============================================================
    //  Create Objects
    //==============================================================

    //--------------------------------------------------------     
    //send signal to create a trap
    void createTrap(EventData eData);
 
    //send signal to destroy a trap
    void destroyTrap(EventData eData);

    //creates a trap item where it should be on the map
    void remoteCreateTrap(RakNet::Packet* packet);

    //destroy a trap item where it should be on the map
    void remoteDestroyTrap(RakNet::Packet* packet);
    //--------------------------------------------------------


    //--------------------------------------------------------
    //send signal to create a red shell
    void createRedShell(EventData eData);

    //send signal to destroy a red shell
    void destroyRedShell(EventData eData);

    //creates a red shell item where it should be on the map
    void remoteCreateRedShell(RakNet::Packet* packet);

    //destroy a red shell item where it should be on the map
    void remoteDestroyRedShell(RakNet::Packet* packet);
    //--------------------------------------------------------


    //--------------------------------------------------------
    //send signal to create a blue shell
    void createBlueShell(EventData eData);

    //send signal to destroy a blue shell
    void destroyBlueShell(EventData eData);

    //creates a blue shell item where it should be on the map
    void remoteCreateBlueShell(RakNet::Packet* packet);

    //destroy a blue shell item where it should be on the map
    void remoteDestroyBlueShell(RakNet::Packet* packet);
    //--------------------------------------------------------


    //Broadcasts the collision with a box
    void itemBoxCollision(EventData eData);

    //Receives the collision of a remote player with a box
    void remoteItemBoxCollision(RakNet::Packet* packet);

    //Broadcasts the end of the game, because the player has crossed the start line in the last lap
    void endGame();

    //Receives the event of the game finishing
    void remoteEndGame(RakNet::Packet* packet);

    //==============================================================
    // Getters and setters
    //==============================================================
    std::vector<IComponent::Pointer>& getRemotePlayerComponentList()   {  return remotePlayerComponentList;   } //Remote player component list getter
    std::vector<IComponent::Pointer>& getRemoteTrapComponentList()     {  return remoteTrapComponentList;     } //Remote player component list getter
    std::vector<IComponent::Pointer>& getRemoteRedShellComponentList() {  return remoteRedShellComponentList; } //Remote player component list getter
    std::vector<IComponent::Pointer>& getRemoteBlueSHellComponentList(){  return remoteBlueShellComponentList;} //Remote player component list getter
    void setPlayer(GameObject::Pointer p)                              {  player = p;                         };
    void setStarted(bool s)                                            {  started = s;                        };
    void setConnected(bool s)                                          {  connected = s;                      };
    void setDebugNetworkState(bool s)                                  {  debugNetworkState = s;              };
    GameObject::Pointer getPlayer()                                    {  return player;                      };
    bool getStarted()                                                  {  return started;                     };
    bool getConnected()                                                {  return connected;                   };
    std::list<customMessages>* getLastPackets()                        {  return &lastPackets;                }; //Returns last packets received
    std::list<int>*            getLastSenders()                        {  return &lastSenders;                }; //Returns last packets' Senders
    std::list<std::string>* getLastData()                              {  return &lastData;                   }; //Returns last packets' data

    //IP identifier
    void setServerIP(std::string s)                                    { serverIP = s;                        }; //Set server IP
    std::string getServerIP()                                          { return serverIP;                     }; //Return server IP
    
private:
    //==============================================================
    // Private data
    //==============================================================
    //RakPeerInterface, manages the connection
    RakNet::RakPeerInterface* peer;

    //MatchState started
    bool started;
    bool connected;

    //Own player
    GameObject::Pointer player;

    //Server Info
    int server_id;
    std::string serverIP;

    //Debug info
    bool debugNetworkState = false;
    std::list<customMessages> lastPackets;
    std::list<std::string>    lastData;
    std::list<int>            lastSenders;

    //List of remotePlayerComponent
    std::vector<IComponent::Pointer> remotePlayerComponentList;

    //List of remotePlayerComponent
    std::vector<IComponent::Pointer> remoteTrapComponentList;

    //List of remotePlayerComponent
    std::vector<IComponent::Pointer> remoteRedShellComponentList;

    //List of remotePlayerComponent
    std::vector<IComponent::Pointer> remoteBlueShellComponentList;
};