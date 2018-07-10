#include "RenderIrrlicht.h"
#include "InputIrrlicht.h"
#include "../GameManager/InputManager.h"
#include "../GameObject/RenderComponent/LightRenderComponent.h"
#include "../GameObject/RenderComponent/ObjectRenderComponent.h"
#include "../GameObject/RenderComponent/CameraRenderComponent.h"

//==============================================================
// THINGS TO DO
/*
    >REMOVE THE COMPONENTS BY GENERALIZING FUNCTIONS AND USING PARAMETERS,
        IT'S NOT THAT HARD FFS
*/
//==============================================================

//==============================================================
// Engine Related functions
//==============================================================

void RenderIrrlicht::openWindow(){

    irr::video::E_DRIVER_TYPE driverType = irr::video::E_DRIVER_TYPE::EDT_OPENGL;

    InputIrrlicht* receiver = new InputIrrlicht();

    device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(window.size.x, window.size.y),
        16, window.fullscreen, false, window.vsync, receiver);

    videoDriver = device->getVideoDriver();
    sceneManager = device->getSceneManager();
    geometryCreator = sceneManager->getGeometryCreator();

    //___>
    /*font = sceneManager->getGUIEnvironment()->getFont("../media/img/fontcourier.bmp");
    pos = sceneManager->getGUIEnvironment()->addStaticText(L"Position: ", irr::core::recti(0,0, 200,50));
    lap = sceneManager->getGUIEnvironment()->addStaticText(L"Lap: ", irr::core::recti(0, 20, 200, 50));
    item = sceneManager->getGUIEnvironment()->addStaticText(L"Item: ", irr::core::recti(0, 40, 200, 50));
    pos->setOverrideFont(font);

    createItemIcon(glm::vec2(50,50), "media/img/emptymini.png");*/

    addCamera();
    //sceneManager->setAmbientLight(irr::video::SColorf(0.8,0.8,0.8,1));

    uintptr_t aux = reinterpret_cast<uintptr_t>(device);
    InputManager::getInstance().setDevice(aux);
    InputManager::getInstance().setInputFacade(receiver);

}

void RenderIrrlicht::updateWindow() {

}

void RenderIrrlicht::closeWindow() {

    device->drop();

}

//==============================================================
// Render Related functions
//==============================================================

void RenderIrrlicht::renderDraw() {

    videoDriver->beginScene(true, true, irr::video::SColor(255,150,150,255));
    sceneManager->drawAll();
    sceneManager->getGUIEnvironment()->drawAll();
    videoDriver->endScene();
 
}

void RenderIrrlicht::drawGUI(){
    
    videoDriver->beginScene(true, true, irr::video::SColor(255,150,150,255));
    sceneManager->getGUIEnvironment()->drawAll();
    videoDriver->endScene();
 
}

void RenderIrrlicht::addCamera() {
    camera = sceneManager->addCameraSceneNode();
    camera->setPosition(irr::core::vector3df(0,0,0));
}

void RenderIrrlicht::setCameraTarget(glm::vec3 position, glm::vec3 target) {

}

void RenderIrrlicht::interpolateCamera(float accTime, float maxTime) {
    //Get target position
    auto pos = cameraTarget->getTransformData().position;

    //Get target y angle
    float radianAngle = cameraTarget->getTransformData().rotation.y;

    //Get interpolated distance to the player
    float oldD = cameraTarget->getComponent<CameraRenderComponent>().get()->getOldDistance();
    float newD = cameraTarget->getComponent<CameraRenderComponent>().get()->getDistance();

    float distance = oldD + (accTime * (newD - oldD))/maxTime;

    camera->setTarget(irr::core::vector3df(pos.x, pos.y, pos.z));
    camera->setPosition(irr::core::vector3df(pos.x - distance * cos(radianAngle), pos.y + distance * 0.4, pos.z + distance * sin(radianAngle)));
}

void RenderIrrlicht::addObject(IComponent* ptr) {

    ObjectRenderComponent* cmp = dynamic_cast<ObjectRenderComponent*>(ptr);

    if(cmp != nullptr){

        auto shape = cmp->getObjectShape();
        auto obj = cmp->getGameObject();
        //Transform the data to irrlicht type
        auto pos = obj.getTransformData().position;
        auto rot = obj.getTransformData().rotation;
        auto sca = obj.getTransformData().scale;
        irr::core::vector3df irrPos = irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z);
        irr::core::vector3df irrRot = irr::core::vector3df((float)rot.x,(float)rot.y, (float)rot.z);
        irr::core::vector3df irrSca = irr::core::vector3df((float)sca.x,(float)sca.y, (float)sca.z);

        irr::scene::ISceneNode * node;

        //Initialize the node
        switch(shape){

            case ObjectRenderComponent::Shape::Cube: {
                node = sceneManager->addCubeSceneNode();
                auto var = videoDriver->getTexture(cmp->getImg().c_str());
                node->setMaterialTexture(0, var);
            }
            break;
            case ObjectRenderComponent::Shape::Sphere: {
                node = sceneManager->addSphereSceneNode();
                auto var = videoDriver->getTexture(cmp->getImg().c_str());
                node->setMaterialTexture(0, var);
            }
            break;
            case ObjectRenderComponent::Shape::Plane: {
                auto plane = geometryCreator->createPlaneMesh(irr::core::dimension2d<irr::f32>(1,1));
                node = sceneManager->addMeshSceneNode(plane);
                auto var = videoDriver->getTexture(cmp->getImg().c_str());
                node->setMaterialTexture(0, var);
            }
            break;
            case ObjectRenderComponent::Shape::Mesh: {
                auto plane = sceneManager->getMesh(cmp->getMesh().c_str());
                node = sceneManager->addMeshSceneNode(plane);
            }
            break;
            case ObjectRenderComponent::Shape::Arrow: {
                auto plane = geometryCreator->createArrowMesh(4,8,10,6,1,3);
                node = sceneManager->addMeshSceneNode(plane);
                auto var = videoDriver->getTexture(cmp->getImg().c_str());
                node->setMaterialTexture(0, var);
            }
            break;
            case ObjectRenderComponent::Shape::Portion: {
                auto plane = sceneManager->getMesh("media/mesh/portion/portion.obj");
                node = sceneManager->addMeshSceneNode(plane);
                auto var = videoDriver->getTexture(cmp->getImg().c_str());
                node->setMaterialTexture(0, var);
                node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
            }
            break;
            case ObjectRenderComponent::Shape::Pyramid: {
                auto plane = sceneManager->getMesh("media/mesh/pyramid/pyramid.obj");
                node = sceneManager->addMeshSceneNode(plane);
                auto var = videoDriver->getTexture(cmp->getImg().c_str());
                node->setMaterialTexture(0, var);
                node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
            }
            break;
            default:
            break;
        }

        //Set node transformation
        node->setPosition(irrPos);
        node->setRotation(irrRot);
        node->setScale(irrSca);
    
        nodeMap.insert(std::pair<uint16_t, irr::scene::ISceneNode*>(obj.getId(), node));
    }
}

void RenderIrrlicht::addObject(IComponent* ptr, float radius, float length, int tesselation, bool transparency) {

    ObjectRenderComponent* cmp = dynamic_cast<ObjectRenderComponent*>(ptr);

    if(cmp != nullptr){

        auto shape = cmp->getObjectShape();
        auto obj = cmp->getGameObject();
        //Transform the data to irrlicht type
        auto pos = obj.getTransformData().position;
        auto rot = obj.getTransformData().rotation;
        auto sca = obj.getTransformData().scale;
        irr::core::vector3df irrPos = irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z);
        irr::core::vector3df irrRot = irr::core::vector3df((float)rot.x,(float)rot.y, (float)rot.z);
        irr::core::vector3df irrSca = irr::core::vector3df((float)sca.x,(float)sca.y, (float)sca.z);

        irr::scene::ISceneNode * node;

        //Initialize the node
        switch(shape){

            case ObjectRenderComponent::Shape::Cylinder: {
                auto plane = geometryCreator->createCylinderMesh(radius,length,tesselation);
                node = sceneManager->addMeshSceneNode(plane);
                auto var = videoDriver->getTexture(cmp->getImg().c_str());
                node->setMaterialTexture(0, var);
                if(transparency == true)
                {
                    node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                }
            }
            break;
            case ObjectRenderComponent::Shape::Cone: {
                auto plane = geometryCreator->createConeMesh(radius,length,tesselation);
                node = sceneManager->addMeshSceneNode(plane);
                auto var = videoDriver->getTexture(cmp->getImg().c_str());
                node->setMaterialTexture(0, var);
                if(transparency == true)
                {
                    node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                }
            }
            break;
            default:
            break;
        }

        //Set node transformation
        node->setPosition(irrPos);
        node->setRotation(irrRot);
        node->setScale(irrSca);

        nodeMap.insert(std::pair<uint16_t, irr::scene::ISceneNode*>(obj.getId(), node));
    }
}

void RenderIrrlicht::addLight(IComponent* ptr) {

    LightRenderComponent* cmp = dynamic_cast<LightRenderComponent*>(ptr);

    if(cmp != nullptr){

        auto obj = cmp->getGameObject();
        auto type = cmp->getLightType();
        auto rad = cmp->getLightRadius();
        //Transform the data to irrlicht type
        auto pos = obj.getTransformData().position;
        auto rot = obj.getTransformData().rotation;
        auto sca = obj.getTransformData().scale;
        irr::core::vector3df irrPos = irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z);
        irr::core::vector3df irrRot = irr::core::vector3df((float)rot.x,(float)rot.y, (float)rot.z);
        irr::core::vector3df irrSca = irr::core::vector3df((float)sca.x,(float)sca.y, (float)sca.z);

        irr::scene::ISceneNode * node;

        //Initialize the node
        switch(type){

            case LightRenderComponent::Type::Point: {
                auto light = sceneManager->addLightSceneNode(0, irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z), irr::video::SColorf(1.0,1.0,1.0), rad); 
                auto & type = light->getLightData();
                node = light;
                type.Type = irr::video::E_LIGHT_TYPE::ELT_POINT;

            }
            break;
            case LightRenderComponent::Type::Directional: {
                auto light = sceneManager->addLightSceneNode(0, irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z), irr::video::SColorf(1.0,1.0,1.0), rad); 
                auto & type = light->getLightData();
                node = light;
                type.Type = irr::video::E_LIGHT_TYPE::ELT_DIRECTIONAL;
            }
            default:
            break;
        }

        //Set node transformation
        node->setPosition(irrPos);
        node->setRotation(irrRot);
        node->setScale(irrSca);

        nodeMap.insert(std::pair<uint16_t, irr::scene::ISceneNode*>(obj.getId(), node));
    }
}

void RenderIrrlicht::deleteObject(IComponent* ptr) {
    
    auto id = ptr->getGameObject().getId();
    auto itr = nodeMap.find(id);

    if(itr != nodeMap.end()){
        auto node = itr->second;
        node->remove();
        nodeMap.erase(id);
    }
}

void RenderIrrlicht::updateObjectTransform(uint16_t id, GameObject::TransformationData transform) {
    auto pos = transform.position;
    auto rot = transform.rotation;
    auto sca = transform.scale;
    irr::core::vector3df irrPos = irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z);
    irr::core::vector3df irrSca = irr::core::vector3df((float)sca.x,(float)sca.y, (float)sca.z);

    auto iterator = nodeMap.find(id);
    if(iterator != nodeMap.end()){

        irr::core::quaternion quadX = irr::core::vector3df(rot.x,0,0);
        irr::core::quaternion quadY = irr::core::vector3df(0,rot.y,0);
        irr::core::quaternion quadZ = irr::core::vector3df(0,0,rot.z);

        irr::core::quaternion quadRes = quadY*quadX*quadZ;

        irr::core::vector3df vec;
        quadRes.toEuler(vec);

        irr::core::vector3df vecRes;

        vecRes.X = vec.X*180/M_PI;
        vecRes.Y = vec.Y*180/M_PI;
        vecRes.Z = vec.Z*180/M_PI;

        auto node = iterator->second;
        node->setPosition(irrPos);
        node->setRotation(vecRes);
        node->setScale(irrSca);
    }
}

void RenderIrrlicht::addSkybox(IComponent* ptr, std::string t, std::string bo, std::string l, std::string r, std::string f, std::string ba)
{
    ObjectRenderComponent* cmp = dynamic_cast<ObjectRenderComponent*>(ptr);

    if(cmp != nullptr){

        auto obj = cmp->getGameObject();
        //Transform the data to irrlicht type
        auto pos = obj.getTransformData().position;
        auto rot = obj.getTransformData().rotation;
        auto sca = obj.getTransformData().scale;
        irr::core::vector3df irrPos = irr::core::vector3df((float)pos.x,(float)pos.y, (float)pos.z);
        irr::core::vector3df irrRot = irr::core::vector3df((float)rot.x,(float)rot.y, (float)rot.z);
        irr::core::vector3df irrSca = irr::core::vector3df((float)sca.x,(float)sca.y, (float)sca.z);

        irr::scene::ISceneNode * node;

        //Initialize the node
        t = "media/img/" + t;
        bo = "media/img/" + bo;
        l = "media/img/" + l;
        r = "media/img/" + r;
        f = "media/img/" + f;
        ba = "media/img/" + ba;
        auto top = videoDriver->getTexture(t.c_str());
        auto bot = videoDriver->getTexture(bo.c_str());
        auto left = videoDriver->getTexture(l.c_str());
        auto right = videoDriver->getTexture(r.c_str());
        auto front = videoDriver->getTexture(f.c_str());
        auto back = videoDriver->getTexture(ba.c_str());
        node = sceneManager->addSkyBoxSceneNode(top,bot,left,right,front,back);

        //Set node transformation
        node->setPosition(irrPos);
        node->setRotation(irrRot);
        node->setScale(irrSca);
    
        nodeMap.insert(std::pair<uint16_t, irr::scene::ISceneNode*>(obj.getId(), node));
    }
}

void RenderIrrlicht::createParticleSystem(uint16_t id, const char* shape, glm::vec3 position, float radius, int birthrate, float particleLife,
                                        glm::vec3 birthDirection, glm::vec3 deathDirection, float variationDirection,
                                        float birthSize, float deathSize, float variationSize,
                                        glm::vec4 birthColor, glm::vec4 deathColor, float variationColor) {}
                                        
void RenderIrrlicht::deleteObject(uint16_t id) {}

    ///////////////////////////////
    ///////      DEBUG      ///////    
    ///////////////////////////////

void RenderIrrlicht::updateLogo(){
    
}


void RenderIrrlicht::createRectangle2D(glm::vec2 pos, std::string img)
{
    irr::core::position2d<irr::s32> position(pos.x, pos.y);
    auto image = videoDriver->getTexture(img.c_str());
    rectangle = sceneManager->getGUIEnvironment()->addImage(image, position);
}

void RenderIrrlicht::deleteRectangle2D()
{
    if(rectangle != nullptr)
        rectangle->remove();
}

void RenderIrrlicht::createRectangleCol2D(glm::vec2 pos, std::string img)
{
    irr::core::position2d<irr::s32> position(pos.x, pos.y);
    auto image = videoDriver->getTexture(img.c_str());
    rectangleCol = sceneManager->getGUIEnvironment()->addImage(image, position);
}

void RenderIrrlicht::deleteRectangleCol2D()
{
    rectangleCol->remove();
}

///////////////////
/// TITLE
///////////////////

void RenderIrrlicht::createTitleText(glm::vec2 pos, std::string text)
{
    std::wstring text_aux;
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    const wchar_t* txt = text_aux.c_str();

    title = sceneManager->getGUIEnvironment()->addStaticText(txt, irr::core::recti(pos.x, pos.y, pos.x+200,pos.y+50));
    title->setText(txt);
}

void RenderIrrlicht::deleteTitleText()
{
    title->remove();
}


void RenderIrrlicht::setTitleText(std::string text)
{
    std::wstring text_aux;
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    const wchar_t* txt = text_aux.c_str();

    title->setText(txt);
}

///////////////////
/// DESCRIPTION
///////////////////

void RenderIrrlicht::createDescriptionText(glm::vec2 pos, std::string text)
{
    std::wstring text_aux;
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    const wchar_t* txt = text_aux.c_str();

    description = sceneManager->getGUIEnvironment()->addStaticText(txt, irr::core::recti(pos.x, pos.y, pos.x+320,pos.y+100));
    description->setText(txt);
}

void RenderIrrlicht::deleteDescriptionText()
{
    description->remove();
}

void RenderIrrlicht::setDescriptionText(std::string text)
{
    std::wstring text_aux;
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    const wchar_t* txt = text_aux.c_str();

    description->setText(txt);
}

///////////////////
/// SUBTITLE
///////////////////

void RenderIrrlicht::createSubTitleText(glm::vec2 pos, std::string text)
{
    std::wstring text_aux;
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    const wchar_t* txt = text_aux.c_str();

    subTitle = sceneManager->getGUIEnvironment()->addStaticText(txt, irr::core::recti(pos.x, pos.y, pos.x+200,pos.y+50));
    subTitle->setText(txt);
}

void RenderIrrlicht::deleteSubTitleText()
{
    subTitle->remove();
}


void RenderIrrlicht::setSubTitleText(std::string text)
{
    std::wstring text_aux;
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    const wchar_t* txt = text_aux.c_str();

    subTitle->setText(txt);
}

///////////////////
/// SUBDESCRIPTION
///////////////////

void RenderIrrlicht::createSubDescriptionText(glm::vec2 pos, std::string text)
{
    std::wstring text_aux;
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    const wchar_t* txt = text_aux.c_str();

    subDescription = sceneManager->getGUIEnvironment()->addStaticText(txt, irr::core::recti(pos.x, pos.y, pos.x+180,pos.y+50));
    subDescription->setText(txt);
}

void RenderIrrlicht::deleteSubDescriptionText()
{
    subDescription->remove();
}

void RenderIrrlicht::setSubDescriptionText(std::string text)
{
    std::wstring text_aux;
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    const wchar_t* txt = text_aux.c_str();

    subDescription->setText(txt);
}


void RenderIrrlicht::createItemIcon(glm::vec2 pos, std::string img)
{
    irr::core::position2d<irr::s32> position(pos.x, pos.y);
    auto image = videoDriver->getTexture(img.c_str());
    itemIMG = sceneManager->getGUIEnvironment()->addImage(image, position);
}

void RenderIrrlicht::deleteItemIcon()
{
    if(itemIMG != nullptr)
        itemIMG->remove();
}

void RenderIrrlicht::updateItemIcon(){

    /*int itemID = ObjectManager::getInstance().getObject(cameraTarget->getId()).get()->getComponent<ItemHolderComponent>().get()->getItemType();


    switch(itemID){

        case -1: //EMPTY
                 deleteItemIcon();
                 createItemIcon(glm::vec2(50,50), "media/img/emptymini.png");
                 break;

        case 0: //RED SHELL
                deleteItemIcon();
                createItemIcon(glm::vec2(50,50), "media/img/iconoRuedamini.png");
                 break;

        case 1: //BLUE SHELL
                deleteItemIcon();
                createItemIcon(glm::vec2(50,50), "media/img/iconoBombamini.png");
                 break;

        case 2: //TRAP
                deleteItemIcon();
                createItemIcon(glm::vec2(50,50), "media/img/iconoTrampamini.png");
                 break;

        case 3: //MUSHROOM
                deleteItemIcon();
                createItemIcon(glm::vec2(50,50), "media/img/iconoSetamini.png");
                 break;

        case 4: //STAR
                deleteItemIcon();
                createItemIcon(glm::vec2(50,50), "media/img/iconoBotellamini.png");
                 break;
    }*/
}

//==============================================================
// VISUAL INTERFACE
//==============================================================
////////////
//  Image
////////////

int32_t RenderIrrlicht::addImage(std::string img, glm::vec2 pos)
{
    //Create position in 2D inside the screen
    irr::core::position2d<irr::s32> position(pos.x, pos.y);

    //Get the texture/img of the rectangle
    auto image = videoDriver->getTexture(img.c_str());

    //Insert the image on the scene
    irr::gui::IGUIImage* element = sceneManager->getGUIEnvironment()->addImage(image, position);
    //Add it to the array
    GUIImageArray.push_back(element);

    //Set ID
    element->setID(GUIId);
    GUIId++;

    //Return the id
    int32_t id = (int32_t) element->getID();
    return id;
}

void RenderIrrlicht::changeImage(int32_t id, std::string img)
{
    for(unsigned int i = 0; i < GUIImageArray.size(); i++){
        if(id == GUIImageArray.at(i)->getID()){
            //Load image as ITexture*
            auto image = videoDriver->getTexture(img.c_str());

            //Set the image to the object
            GUIImageArray.at(i)->setImage(image);
        }
    }
}

void RenderIrrlicht::deleteImage(int32_t id)
{
    //Search for the GUI element
    for(unsigned int i = 0; i < GUIImageArray.size(); i++){
        if(id == GUIImageArray.at(i)->getID()){
            
            //Delete the element
            GUIImageArray.at(i)->remove();

            //Delete from array
            GUIImageArray.erase(GUIImageArray.begin()+i);
        }
    }   
}

void RenderIrrlicht::cleanImages()
{
    //Clear array of members
    for(unsigned int i = 0; i < GUIImageArray.size(); i++){
        //Erase each image
        GUIImageArray.at(i)->remove();
    }

    //Clean array
    GUIImageArray.clear();
}

////////////
//  Rectangle
////////////

int32_t RenderIrrlicht::addRectangleColor(glm::vec2 pos, glm::vec2 size, int r, int g, int b, int a)
{
    //Create position and size in 2D inside the screen
    irr::core::rect<irr::s32> rectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

    //Insert element inside the scene
    irr::gui::IGUIImage* element = sceneManager->getGUIEnvironment()->addImage(rectangle);

    //Apply color
    irr::video::SColor color((int32_t) a, (int32_t) r, (int32_t) g, (int32_t) b);
    element->setUseAlphaChannel(true);                                  //Alpha channel
    element->setScaleImage(true);                                       //Alpha channel
    element->setColor(color);                                           //Color applied
    std::string white_texture("media/img/white_rectangle.png");         //White texture loading
    element->setImage(videoDriver->getTexture(white_texture.c_str()));  //White texture
    
    //Add it to the array
    GUIRectangleColorArray.push_back(element);

    //Set ID
    element->setID(GUIId);
    GUIId++;

    //Return the id*/
    int32_t id = (int32_t) element->getID();
    return id;
}

void RenderIrrlicht::changeRectangleColor(int32_t id, int r, int g, int b, int a)
{
    for(unsigned int i = 0; i < GUIRectangleColorArray.size(); i++){
        if(id == GUIRectangleColorArray.at(i)->getID()){
            //Create color
            irr::video::SColor color((int32_t) a, (int32_t) r, (int32_t) g, (int32_t) b);

            //Set the image to the object
            GUIRectangleColorArray.at(i)->setColor(color);
        }
    }
}
    
void RenderIrrlicht::deleteRectangleColor(int32_t id)
{
    //Search for the GUI element
    for(unsigned int i = 0; i < GUIRectangleColorArray.size(); i++){
        if(id == GUIRectangleColorArray.at(i)->getID()){
            
            //Delete the element
            GUIRectangleColorArray.at(i)->remove();

            //Delete from array
            GUIRectangleColorArray.erase(GUIRectangleColorArray.begin()+i);
        }
    }
}

void RenderIrrlicht::cleanRectangles()
{
    //Clear array of members
    for(unsigned int i = 0; i < GUIRectangleColorArray.size(); i++){
        //Erase each rectangle
        GUIRectangleColorArray.at(i)->remove();
    }

    //Clean array
    GUIRectangleColorArray.clear();
}

////////////
//  Text
////////////

int32_t RenderIrrlicht::addText( std::string text, glm::vec2 pos, int r, int g, int b, int a, glm::vec2 size, std::string fontFile)
{
    //Auxiliar variables
    std::wstring text_aux;
    irr::gui::IGUIFont* font;
    irr::video::SColor color((int32_t) a, (int32_t) r, (int32_t) g, (int32_t) b);

    //Converting to wstring type
    for(unsigned int i = 0; i < text.length(); ++i)
    text_aux += wchar_t( text[i] );

    //Converting to irrlicht inner text type
    const wchar_t* txt = text_aux.c_str();

    //Adding to scene rectangle text
    irr::gui::IGUIStaticText* element = sceneManager->getGUIEnvironment()->addStaticText(txt, irr::core::recti(pos.x, pos.y, pos.x+size.x,pos.y+size.y));

    //Set font
    if(fontFile.compare("")!=0)
    {
        //Get font from bitmap file
        font = sceneManager->getGUIEnvironment()->getFont(fontFile.c_str());

    }else{
        //Get font from actual scene
        font = element->getActiveFont();
    }

    //override actual font
    element->setOverrideFont(font);

    //override actual color
    element->setOverrideColor(color);

    //Set ID
    element->setID(GUIId);
    GUIId++;

    //Add to array of GUI elements
    GUITextArray.push_back(element);

    //Return the id
    int32_t id = (int32_t) element->getID();
    return id;
};


void RenderIrrlicht::changeText(int32_t id, std::string text)
{
    //Search for the text element
    for(unsigned int i = 0; i < GUITextArray.size(); i++){
        if(id == GUITextArray.at(i)->getID()){
            
            //Auxiliar text for transposition
            std::wstring text_aux;

            //Converting to wstring type
            for(unsigned int i = 0; i < text.length(); ++i)
            text_aux += wchar_t( text[i] );

            //Converting to irrlicht inner text type
            const wchar_t* txt = text_aux.c_str();

            //Change text
            GUITextArray.at(i)->setText(txt);
        }
    }  
}

void RenderIrrlicht::changeFontText(int32_t id, std::string fontFile)
{
    //Search for the text element
    for(unsigned int i = 0; i < GUITextArray.size(); i++){
        if(id == GUITextArray.at(i)->getID()){
            //Get font
            irr::gui::IGUIFont* font = sceneManager->getGUIEnvironment()->getFont(fontFile.c_str());

            //Override it
            GUITextArray.at(i)->setOverrideFont(font);
        }
    }  
}


void RenderIrrlicht::changeColorText(int32_t id, int r, int g, int b, int a)
{
    //Search for the text element
    for(unsigned int i = 0; i < GUITextArray.size(); i++){
        if(id == GUITextArray.at(i)->getID()){
            
            //Set color
            irr::video::SColor color((int32_t) a, (int32_t) r, (int32_t) g, (int32_t) b);
             
            //Change color
            GUITextArray.at(i)->setOverrideColor(color);
        }
    }  
}

void RenderIrrlicht::changeBackgroundColorText(int32_t id, int r, int g, int b, int a)
{
    //Search for the text element
    for(unsigned int i = 0; i < GUITextArray.size(); i++){
        if(id == GUITextArray.at(i)->getID()){
            
            //Set color
            irr::video::SColor color((int32_t) a, (int32_t) r, (int32_t) g, (int32_t) b);
             
            //Change color
            GUITextArray.at(i)->setBackgroundColor(color);
        }
    }  
}

void RenderIrrlicht::deleteText(int32_t id)
{
    //Search for the GUI element
    for(unsigned int i = 0; i < GUITextArray.size(); i++){
        if(id == GUITextArray.at(i)->getID()){
            
            //Delete the element
            GUITextArray.at(i)->remove();

            //Delete from array
            GUITextArray.erase(GUITextArray.begin()+i);
        }
    }  
}
 
void RenderIrrlicht::cleanTexts()
{
    //Clear array of texts
    for(unsigned int i = 0; i < GUITextArray.size(); i++){
        //Erase each text
        GUITextArray.at(i)->remove();
    }

    //Clean array
    GUITextArray.clear();
}

void RenderIrrlicht::cleanInterface()
{
    //Invoke the erasing images for every type of GUI resource
    cleanImages();
    cleanRectangles();
    cleanTexts();

    //All objects start from 0
    GUIId = 0;
}