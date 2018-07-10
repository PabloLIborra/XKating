#include "NetworkManager.h"
#include "../GlobalVariables.h"


//////////////////////////////////////////////
//            Possible Upgrades
//////////////////////////////////////////////
/*
+>Add time stamp from server
>Add Debug callers
*/
//////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////// DELEGATE FUNCTIONS
//////////////////////////////////////////////////////
void itemBoxCollisionEvent(EventData eData);
void startLineCollisionEvent(EventData eData);
void createTrapEvent(EventData eData);
void destroyTrapEvent(EventData eData);
void createRedShellEvent(EventData eData);
void destroyRedShellEvent(EventData eData);
void createBlueShellEvent(EventData eData);
void destroyBlueShellEvent(EventData eData); 
//=============================================

//==============================================
// PLAYER FUNCTIONS
//============================================== 

void NetworkManager::createPlayer(RakNet::Packet* packet)
{
    //Initial variables
    float x, y, z;
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    //Server ID
    parser.Read(server_id);
    //Position
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);

    //If player exists take its transform data, if not create it
    if(GlobalVariables::getInstance().getPlayer() == nullptr)
    {
        //Set position
        GameObject::TransformationData t;
        t.position.x = x;
        t.position.y = y;
        t.position.z = z;

        //Creation function
        ObjectManager::getInstance().createPlayer(t, GlobalVariables::getInstance().getSelectedPlayer(), 0, 25000, 
                                                PhysicsManager::getInstance().getTerrainFromPos(t.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(t.position));
    }

    auto trans = player.get()->getTransformData();

    trans.position.x = x;
    trans.position.y = y;
    trans.position.z = z;
    player.get()->setNewTransformData(trans);
    player.get()->setOldTransformData(trans);
    player.get()->setTransformData(trans);

}

void NetworkManager::createRemotePlayer(RakNet::Packet* packet)
{
    int id;
    float x, y, z;
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(id);
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);

    GameObject::TransformationData trans;
    trans.position.x = x;
    trans.position.y = y;
    trans.position.z = z;

    trans.rotation.x = 0;
    trans.rotation.y = 90;
    trans.rotation.z = 0;

    trans.scale.x    = 1;
    trans.scale.y    = 1;
    trans.scale.z    = 1;

    ObjectManager::getInstance().createPlayer(trans, 0, 2, 25001+id, 
                                                PhysicsManager::getInstance().getTerrainFromPos(trans.position).get()->getTerrain(), 
                                                PhysicsManager::getInstance().getTerrainFromPos(trans.position));

    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == -1)
        {
            found = true;
            rPlayer.get()->setServerId(id);
        }
    }

}

//=============================================
// CREATE ITEMS
//=============================================

////////////
//CREATE TRAP
////////////

//Send message to the server to create trap
void NetworkManager::createTrap(EventData eData)
{
    //Stream of raknet bits
    RakNet::BitStream stream;

    stream.Write((unsigned char)ID_CREATE_TRAP);  //Send message create trap to server
    stream.Write((int)server_id);                 //Send Id of the player that created it

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //Send the message
}

void NetworkManager::destroyTrap(EventData eData)
{
    //Stream of raknet bits
    RakNet::BitStream stream;

    int s_id = ObjectManager::getInstance().getObject(eData.Id).get()->getComponent<RemoteItemComponent>()->getServerId();//get server id of the object

    stream.Write((unsigned char)ID_DESTROY_TRAP); //Send type of event (destroy trap)
    stream.Write((int) s_id);                     //Send server id of the trap
    stream.Write((int) server_id);                //Send Id of the player that collided with it

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true); //Send the message
}

void NetworkManager::remoteCreateTrap(RakNet::Packet* packet){
    int s_id; //server player creator id
    int o_id; //object server id

    //Read that from the packet
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(o_id);

    //Search for the player that created the trap to create it in its position
    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == s_id)    //find the player creator of the trap
        {
            found = true;
            auto object = ItemManager::getInstance().createTrap(rPlayer.get()->getGameObject(), IItemComponent::InstanceType::REMOTE);

            //set the server id of the trap
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);
        }
    }
     //If not found by other players, it was our player who did it
    if(found == false)
    {
        auto object = ItemManager::getInstance().createTrap(*player.get(), IItemComponent::InstanceType::LOCAL);
        object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Player who created the trap
        lastSenders.push_back(s_id);

        //Read from packet and transform info
        std::string data = std::to_string(o_id);
        data += " created";

        //Push all information
        lastSenders.push_back(s_id);            //last sender
        lastPackets.push_back(ID_CREATE_TRAP);  //type of packet
        lastData.push_back(data);               //data
    }
}

void NetworkManager::remoteDestroyTrap(RakNet::Packet* packet){
    int s_id;      //server object id
    int sender_id; //Server id of the player that collided with the trap

    //Read data
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(sender_id);

    //Search trap inside the remote component list (external traps)
    bool found = false;
    std::shared_ptr<RemoteItemComponent> rItem;
    for(unsigned int i = 0; i<remoteTrapComponentList.size() && found == false; i++)
    {
        rItem = std::dynamic_pointer_cast<RemoteItemComponent>(remoteTrapComponentList[i]);
        if(rItem.get()->getServerId() == s_id)    //find the Id of the item
        {
            found = true;
            //Remove from list
            remoteTrapComponentList.erase(remoteTrapComponentList.begin()+i);

            //Send erase object event
            EventData data;
            data.Id = rItem.get()->getGameObject().getId();

            EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
        }
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Read from packet and transform info
        std::string data = std::to_string(s_id);
        data += " destroyed";

        //Push all information
        lastSenders.push_back(sender_id);             //last sender
        lastPackets.push_back(ID_DESTROY_TRAP);       //type of packet
        lastData.push_back(data);                     //data
    }

}


////////////
//CREATE TIRE
////////////

 //Send message to the server to create red shell
 void NetworkManager::createRedShell(EventData eData)
 {
     //Stream of raknet bits
     RakNet::BitStream stream;
 
     stream.Write((unsigned char)ID_CREATE_RED_SHELL);  //Send message create red shell to server
     stream.Write((int)server_id);                      //Send Id of the player that created it
 
     peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //Send the message
 }
 
 void NetworkManager::destroyRedShell(EventData eData)
 {
     RakNet::BitStream stream;
 
     int s_id = ObjectManager::getInstance().getObject(eData.Id).get()->getComponent<RemoteItemComponent>()->getServerId();//get server id of the object
 
     stream.Write((unsigned char)ID_DESTROY_RED_SHELL);
     stream.Write((int) s_id);                          //Send server id of the red shell
     stream.Write((int) server_id);                     //Send Id of the player that collided with it or managed its destruction
 
     peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::remoteCreateRedShell(RakNet::Packet* packet){
    int s_id; //server player creator id
    int o_id; //object server id

    //Read data from the packet
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(o_id);

    //Search for the player to create the object in its position
    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == s_id)    //find the player creator of the red shell
        {
            found = true;
            auto object = ItemManager::getInstance().createRedShell(rPlayer.get()->getGameObject(), IItemComponent::InstanceType::REMOTE); //Create object
            std::dynamic_pointer_cast<ItemRedShellComponent>(object)->init();                           //Initialize object
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);       //Set server id
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setParentId(s_id);       //Set parent id
        }
    }

     //If not found by other players, it was our player who did it
    if(found == false)
    {
        auto object = ItemManager::getInstance().createRedShell(*player.get(), IItemComponent::InstanceType::LOCAL);  //Create object
        std::dynamic_pointer_cast<ItemRedShellComponent>(object)->init();                                             //Initialize object
      
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Read from packet and transform info
        std::string data = std::to_string(o_id);
        data += " created";

        //Push all information
        lastSenders.push_back(s_id);                  //last sender
        lastPackets.push_back(ID_CREATE_RED_SHELL);   //type of packet
        lastData.push_back(data);                     //data
    }

}

void NetworkManager::remoteDestroyRedShell(RakNet::Packet* packet){
    int s_id;   //server object id
    int sender_id; //Server id of the player that collided with the trap

    //Read data from packet
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(sender_id);

    //Search for the item to destroy it
    bool found = false;
    std::shared_ptr<RemoteItemComponent> rItem;
    for(unsigned int i = 0; i<remoteRedShellComponentList.size() && found == false; i++)
    {
        rItem = std::dynamic_pointer_cast<RemoteItemComponent>(remoteRedShellComponentList[i]);
        if(rItem.get()->getServerId() == s_id)    //find the Id of the item
        {
            found = true;
            //Remove from list
            remoteRedShellComponentList.erase(remoteRedShellComponentList.begin()+i);

            //Send erase object event
            EventData data;
            data.Id = rItem.get()->getGameObject().getId();

            EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
        }
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Read from packet and transform info
        std::string data = std::to_string(s_id);
        data += " destroyed";

        //Push all information
        lastSenders.push_back(sender_id);             //last sender
        lastPackets.push_back(ID_DESTROY_RED_SHELL);  //type of packet
        lastData.push_back(data);                     //data
    }
}


////////////
//CREATE BOMB
////////////
//Send message to the server to create blue shell
void NetworkManager::createBlueShell(EventData eData)
{
    //Stream of raknet bits
    RakNet::BitStream stream;

    stream.Write((unsigned char)ID_CREATE_BLUE_SHELL);  //Send message create blue shell to server
    stream.Write((int)server_id);                   //Send Id of the player that created it

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //Send the message
}

void NetworkManager::destroyBlueShell(EventData eData)
{
    RakNet::BitStream stream;

    int s_id = ObjectManager::getInstance().getObject(eData.Id).get()->getComponent<RemoteItemComponent>()->getServerId();//get server id of the object

    stream.Write((unsigned char)ID_DESTROY_BLUE_SHELL);
    stream.Write((int) s_id);   //Send server id of the blue shell
    stream.Write((int) server_id);                     //Send Id of the player that collided with it or managed its destruction

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::remoteCreateBlueShell(RakNet::Packet* packet){
    int s_id; //server player creator id
    int o_id; //object server id

    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(o_id);

    //Search for the player to create the object in its position
    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == s_id)    //find the player creator of the blue shell
        {
            found = true;
            auto object = ItemManager::getInstance().createBlueShell(rPlayer.get()->getGameObject(), IItemComponent::InstanceType::REMOTE); //Create object
            std::dynamic_pointer_cast<ItemBlueShellComponent>(object)->init();                      //Initialize object
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setServerId(o_id);   //Set server id
            object.get()->getGameObject().getComponent<RemoteItemComponent>()->setParentId(s_id);   //Set parent id
        }
    }

    //If not found by other players, it was our player who did it
    if(found == false)
    {
        auto object = ItemManager::getInstance().createBlueShell(*player.get(), IItemComponent::InstanceType::LOCAL); //Create object
        std::dynamic_pointer_cast<ItemBlueShellComponent>(object)->init();                        //Initialize object
    }

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Read from packet and transform info
        std::string data = std::to_string(o_id);
        data += " created";

        //Push all information
        lastSenders.push_back(s_id);                  //last sender
        lastPackets.push_back(ID_CREATE_BLUE_SHELL);  //type of packet
        lastData.push_back(data);                     //data
    }
}

void NetworkManager::remoteDestroyBlueShell(RakNet::Packet* packet){
    int s_id;      //Server object id
    int sender_id; //Server id of the sender

    //Read packet data
    RakNet::BitStream parser(packet->data, packet->length, false);
    parser.IgnoreBytes(1);
    parser.Read(s_id);
    parser.Read(sender_id);  

    //Search for the item to destroy it
    bool found = false;
    std::shared_ptr<RemoteItemComponent> rItem;
    for(unsigned int i = 0; i<remoteBlueShellComponentList.size() && found == false; i++)
    {
        rItem = std::dynamic_pointer_cast<RemoteItemComponent>(remoteBlueShellComponentList[i]);
        if(rItem.get()->getServerId() == s_id)    //find the Id of the item
        {
            found = true;
            //Remove from list
            remoteBlueShellComponentList.erase(remoteBlueShellComponentList.begin()+i);

            //Send erase object event
            EventData data;
            data.Id = rItem.get()->getGameObject().getId();
            
            EventManager::getInstance().addEvent(Event {EventType::GameObject_Delete, data});
        }
    }
        
    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Read from packet and transform info
        std::string data = std::to_string(s_id);
        data += " destroyed";

        //Push all information
        lastSenders.push_back(sender_id);             //last sender
        lastPackets.push_back(ID_DESTROY_BLUE_SHELL); //type of packet
        lastData.push_back(data);                     //data
    }
}

//=============================================
// ITEM BOX
//=============================================

void NetworkManager::itemBoxCollision(EventData eData)
{
    RakNet::BitStream stream;

    //Send packet id
    stream.Write((unsigned char)ID_BOX_COLLISION);
    //Send box object id
    stream.Write(eData.CollComponent.get()->getGameObject().getId());
    //Send who sent it
    stream.Write(server_id);

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::remoteItemBoxCollision(RakNet::Packet* packet)
{
    //Parse data
    RakNet::BitStream parser(packet->data, packet->length, false);
    uint16_t id;
    int sender_id;

    //Ignore packet time
    parser.IgnoreBytes(1);
    //Read data and generate collision
    parser.Read(id);
    ObjectManager::getInstance().getObject(id).get()->getComponent<ItemBoxComponent>().get()->deactivateBox();

    ////////////
    //DEBUG AREA
    ////////////
    //If debug state is on
    if(debugNetworkState){
        //Read from packet and transform info
        parser.Read(sender_id);
        std::string data = std::to_string(id);
        data += " collided";

        //Push all information
        lastSenders.push_back(sender_id);          //last sender
        lastPackets.push_back(ID_BOX_COLLISION);   //type of packet
        lastData.push_back(data);                  //data
    }

}

//=============================================
// MOVING STUFF
//=============================================

void NetworkManager::broadcastPosition()
{
    //Stream variable
    RakNet::BitStream stream;

    //Player has to exist to send the position
    if(player.get() == nullptr)
        return;

    auto trans = player.get()->getTransformData();

    //Send our position to all the other players
    stream.Write((unsigned char)ID_REMOTE_PLAYER_MOVEMENT);
    stream.Write((int)server_id);
    stream.Write((float)trans.position.x);
    stream.Write((float)trans.position.y);
    stream.Write((float)trans.position.z);
    stream.Write((float)trans.rotation.x);
    stream.Write((float)trans.rotation.y);
    stream.Write((float)trans.rotation.z);
    stream.Write((int) server_id);

    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void NetworkManager::moveRemotePlayer(RakNet::Packet* packet)
{
    RakNet::BitStream parser(packet->data, packet->length, false);
    float x, y, z, rx, ry, rz;
    int id;

    parser.IgnoreBytes(1);
    parser.Read(id);
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);
    parser.Read(rx);
    parser.Read(ry);
    parser.Read(rz);
    
    bool found = false;
    std::shared_ptr<RemotePlayerComponent> rPlayer;
    for(unsigned int i = 0; i<remotePlayerComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemotePlayerComponent>(remotePlayerComponentList[i]);
        if(rPlayer.get()->getServerId() == id)
        {
            found = true;
            auto trans = rPlayer.get()->getGameObject().getTransformData();

            trans.position.x = x;
            trans.position.y = y;
            trans.position.z = z;

            trans.rotation.x = rx;
            trans.rotation.y = ry;
            trans.rotation.z = rz;
            
            rPlayer.get()->getGameObject().setNewTransformData(trans);
            RenderManager::getInstance().getRenderFacade()->updateObjectTransform(rPlayer.get()->getGameObject().getId(), trans);

        }
    }
}

void NetworkManager::broadcastPositionRedShell()
{
    RakNet::BitStream stream;
    
    for(unsigned int i = 0; i<remoteRedShellComponentList.size(); i++){
        auto red_shell = std::dynamic_pointer_cast<RemoteItemComponent>(remoteRedShellComponentList[i]);
        stream.Reset();
        if(red_shell.get()->getParentId() == server_id){
            int s_id = red_shell.get()->getServerId();
            auto trans = red_shell.get()->getGameObject().getTransformData();
            stream.Write((unsigned char)ID_REMOTE_RED_SHELL_MOVEMENT);
            stream.Write((int)s_id);
            stream.Write((float)trans.position.x);
            stream.Write((float)trans.position.y);
            stream.Write((float)trans.position.z);
            stream.Write((float)trans.rotation.x);
            stream.Write((float)trans.rotation.y);
            stream.Write((float)trans.rotation.z);

            peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
        }
    }
}

void NetworkManager::moveRemoteRedShell(RakNet::Packet* packet)
{
    RakNet::BitStream parser(packet->data, packet->length, false);
    float x, y, z, rx, ry, rz;
    int id;

    parser.IgnoreBytes(1);
    parser.Read(id);
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);
    parser.Read(rx);
    parser.Read(ry);
    parser.Read(rz);
   
    std::shared_ptr<RemoteItemComponent> rPlayer;
    for(unsigned int i = 0; i<remoteRedShellComponentList.size(); i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemoteItemComponent>(remoteRedShellComponentList[i]);
        if(rPlayer.get()->getServerId() == id)
        {
            auto trans = rPlayer.get()->getGameObject().getTransformData();
            trans.position.x = x;
            trans.position.y = y;
            trans.position.z = z;

            trans.rotation.x = rx;
            trans.rotation.y = ry;
            trans.rotation.z = rz;

            rPlayer.get()->getGameObject().setNewTransformData(trans);
            
        }
    }
}

void NetworkManager::broadcastPositionBlueShell()
{
    RakNet::BitStream stream;
    for(unsigned int i = 0; i<remoteBlueShellComponentList.size(); i++){
        auto blue_shell = std::dynamic_pointer_cast<RemoteItemComponent>(remoteBlueShellComponentList[i]);
        if( blue_shell.get()->getParentId() == server_id ){
            int s_id = blue_shell.get()->getServerId();
            auto trans = blue_shell.get()->getGameObject().getTransformData();
            stream.Reset();
            stream.Write((int)s_id);
            stream.Write((float)trans.position.x);
            stream.Write((float)trans.position.y);
            stream.Write((float)trans.position.z);
            stream.Write((float)trans.rotation.x);
            stream.Write((float)trans.rotation.y);
            stream.Write((float)trans.rotation.z);

            peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
        }
    }
}

void NetworkManager::moveRemoteBlueShell(RakNet::Packet* packet)
{
    RakNet::BitStream parser(packet->data, packet->length, false);
    float x, y, z, rx, ry, rz;
    int id;

    parser.IgnoreBytes(1);
    parser.Read(id);
    parser.Read(x);
    parser.Read(y);
    parser.Read(z);
    parser.Read(rx);
    parser.Read(ry);
    parser.Read(rz);
   
    bool found = false;
    std::shared_ptr<RemoteItemComponent> rPlayer;
    for(unsigned int i = 0; i<remoteBlueShellComponentList.size() && found == false; i++)
    {
        rPlayer = std::dynamic_pointer_cast<RemoteItemComponent>(remoteBlueShellComponentList[i]);
        if(rPlayer.get()->getServerId() == id)
        {
            found = true;
            auto trans = rPlayer.get()->getGameObject().getTransformData();
            trans.position.x = x;
            trans.position.y = y;
            trans.position.z = z;

            trans.rotation.x = rx;
            trans.rotation.y = ry;
            trans.rotation.z = rz;

            rPlayer.get()->getGameObject().setNewTransformData(trans);
        }
    }
}

//=============================================
// END GAME
//=============================================

void NetworkManager::endGame()
{
    //Set online game as finished
    setStarted(false);

    //Send same information to other players
    RakNet::BitStream stream;
    stream.Write((unsigned char)ID_GAME_ENDED);
    peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);

    //Send change state event
    EventData data;
    data.Id = IGameState::stateType::CLIENTLOBBY;
    EventManager::getInstance().addEvent(Event {EventType::State_Change, data});

}

void NetworkManager::remoteEndGame(RakNet::Packet* packet)
{
    //Set online game as finished
    setStarted(false);
    
    //Send change state event
    EventData data;
    data.Id = IGameState::stateType::CLIENTLOBBY;
    EventManager::getInstance().addEvent(Event {EventType::State_Change, data});

}


//=============================================
// PRINCIPAL FUNCTIONS
//=============================================

NetworkManager& NetworkManager::getInstance() {
    static NetworkManager instance;
    return instance;
}

void NetworkManager::init() {
    //Get instance of the peer
    peer = RakNet::RakPeerInterface::GetInstance();

    //Match not started
    started = false;

    //Conexion has not started
    connected = false;

    //Inital value for server IP
    serverIP = std::string("192.168.0.1");

    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::ItemBoxComponent_Collision,itemBoxCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::StartLineComponent_Collision, startLineCollisionEvent});
    EventManager::getInstance().addListener(EventListener {EventType::Trap_Create,createTrapEvent});
    EventManager::getInstance().addListener(EventListener {EventType::TrapComponent_Collision,destroyTrapEvent});
    EventManager::getInstance().addListener(EventListener {EventType::RedShell_Create,createRedShellEvent});
    EventManager::getInstance().addListener(EventListener {EventType::RedShellComponent_Collision,destroyRedShellEvent});
    EventManager::getInstance().addListener(EventListener {EventType::BlueShell_Create,createBlueShellEvent});
    EventManager::getInstance().addListener(EventListener {EventType::BlueShellComponent_Collision,destroyBlueShellEvent});
}

void NetworkManager::update() 
{
    //Received packet
    RakNet::Packet* packet = nullptr;

    //Receive from the RakPeerInterface till there are none
    packet = peer->Receive();

    //If a packet exists
    while(packet)
    {
        //Based on the first byte, determine which packet type is it, then execute accordingly
        switch(packet->data[0])
        {
            case ID_GAME_ENDED:
                remoteEndGame(packet);
                break;

            case ID_CREATE_PLAYER:
                createPlayer(packet);
                break;
                
            case ID_CREATE_REMOTE_PLAYER:
                createRemotePlayer(packet);
                break;
                
            case ID_REMOTE_PLAYER_MOVEMENT:
                moveRemotePlayer(packet);
                break;

            case ID_BOX_COLLISION:
                remoteItemBoxCollision(packet);
                break;

            case ID_CREATE_TRAP:
                remoteCreateTrap(packet);
                break;

            case ID_DESTROY_TRAP:
                remoteDestroyTrap(packet);
                break;

            case ID_CREATE_RED_SHELL:
                remoteCreateRedShell(packet);
                break;

            case ID_DESTROY_RED_SHELL:
                remoteDestroyRedShell(packet);
                break;

            case ID_RED_SHELLS_POSITION:
                moveRemoteRedShell(packet);
                break;

            case ID_CREATE_BLUE_SHELL:
                remoteCreateBlueShell(packet);
                break;

            case ID_DESTROY_BLUE_SHELL:
                remoteDestroyBlueShell(packet);
                break;

            case ID_BLUE_SHELLS_POSITION:
                moveRemoteBlueShell(packet);
                break;

            default:
                std::cout << "Mensaje recibido" << std::endl;
                break;
        }

        //Free packet
        peer->DeallocatePacket(packet);

        //Pick next one if there are more
        packet = peer->Receive();
    }

    //Always broadcast position of items in each iteration
    broadcastPosition();
    broadcastPositionRedShell();
    broadcastPositionBlueShell();
}

void NetworkManager::close() {

}


//==============================================
// LOBBY FUNCTIONS
//============================================== 
void NetworkManager::initLobby(){
    RakNet::SocketDescriptor socket(0, 	0);
	socket.socketFamily = AF_INET;
    peer->Startup(1, &socket, 1);
    RakNet::ConnectionAttemptResult result;
    result = peer->Connect(serverIP.c_str(), 39017, 0, 0);

    if(result == RakNet::CONNECTION_ATTEMPT_STARTED)
    {
        std::cout << "Connection Attempt Started Correctly" << std::endl;
    }
}

void NetworkManager::updateLobby(){
    RakNet::Packet* result;
    RakNet::BitStream stream;
    result = peer->Receive();
    if(result)
    {
        switch(result->data[0])
        {
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << "Connection Accepted" << std::endl;
                connected = true;
                stream.Write((unsigned char)ID_GAME_START);
                peer->Send(&stream, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
                break;
            case ID_CONNECTION_ATTEMPT_FAILED:
                std::cout << "Connection Failed" << std::endl;
                break;
            case ID_GAME_START:
                std::cout << "Game started " << std::endl;
                started = true;
                break;
            default: 
                break;
        }
        peer->DeallocatePacket(result);
    }
}

//==============================================
// CREATE COMPONENTS
//============================================== 

IComponent::Pointer NetworkManager::createRemotePlayerComponent(GameObject& newGameObject) {

    IComponent::Pointer component = std::make_shared<RemotePlayerComponent>(newGameObject);

    newGameObject.addComponent(component);

    remotePlayerComponentList.push_back(component);

    EventData data;
    data.Component = component;

    EventManager::getInstance().addEvent(Event {EventType::RemotePlayerComponent_Create, data});

    return component;
}

IComponent::Pointer NetworkManager::createRemoteItemComponent(GameObject& newGameObject, int type) {

    IComponent::Pointer component = std::make_shared<RemoteItemComponent>(newGameObject);

    newGameObject.addComponent(component);

    //Push inside the correspondent list
    if(type == 0){
        std::dynamic_pointer_cast<RemoteItemComponent>(component).get()->setType(0);
        remoteTrapComponentList.push_back(component);
    }
    if(type == 1){
        std::dynamic_pointer_cast<RemoteItemComponent>(component).get()->setType(1);
        remoteRedShellComponentList.push_back(component);
    }
    if(type == 2){
        std::dynamic_pointer_cast<RemoteItemComponent>(component).get()->setType(2);
        remoteBlueShellComponentList.push_back(component);
    }

    return component;
}

//==============================================
// DELEGATES
//============================================== 

//============================================== 
//Collision events
void itemBoxCollisionEvent(EventData eData) //Collision with an item
{
    NetworkManager::getInstance().itemBoxCollision(eData);
}

void startLineCollisionEvent(EventData eData) //Collision of the player with the racing line
{
    auto player = std::static_pointer_cast<MoveComponent>(eData.Component);

    auto line = player->getGameObject().getComponent<StartLineComponent>();

    //If there is a racing line
    if(line != nullptr) {
        //If who collided is the player
        if(player->getGameObject().getId() == 25000)
        {
            //If it is the last lap
            if(player->getGameObject().getComponent<ScoreComponent>()->getLap()>ScoreManager::getInstance().getMaxLaps())
            {
                //End the game
                NetworkManager::getInstance().endGame();
            }
        }
    }
}
//============================================== 

//============================================== 
void createTrapEvent(EventData eData)
 {
     NetworkManager::getInstance().createTrap(eData);
 }
 
 void destroyTrapEvent(EventData eData){
     NetworkManager::getInstance().destroyTrap(eData);
 }
 
 void createRedShellEvent(EventData eData)
 {
     NetworkManager::getInstance().createRedShell(eData);
 }
 
 void destroyRedShellEvent(EventData eData){
     NetworkManager::getInstance().destroyRedShell(eData);
 }
 
 void createBlueShellEvent(EventData eData)
 {
     NetworkManager::getInstance().createBlueShell(eData);
 }
 
 void destroyBlueShellEvent(EventData eData){
     NetworkManager::getInstance().destroyBlueShell(eData);
}
