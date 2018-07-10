#pragma once

#include <irrlicht.h>
#include <map>
#include <string>
#include <cmath>

#ifdef _WIN32
#define M_PI 3.141592653589
#endif

#include "IRenderFacade.h"


class RenderIrrlicht : public IRenderFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    RenderIrrlicht() : IRenderFacade() {}

    //Destructor
    virtual ~RenderIrrlicht() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openWindow();

    //Updates window info in the engine
    virtual void updateWindow();

    //Closes engine window
    virtual void closeWindow();

    //==============================================================
    // Render Related functions
    //==============================================================

    //Renders all the scene
    virtual void renderDraw();

    //Add a camera to the game
    virtual void addCamera();

    //Update the current camera
    virtual void interpolateCamera(float accTime, float maxTime);
    virtual void setCameraTarget(glm::vec3 position, glm::vec3 target);

    //Add an object to the game
    virtual void addObject(IComponent* ptr);

    //Add an object to the game (Cylinder or Cone)
    virtual void addObject(IComponent* ptr, float radius, float length, int tesselation, bool transparency);

    //Add an animation to the game
    virtual void addAnimation(IComponent* ptr) {};
    virtual void addAnimation(uint16_t id, const char * mesh, int frames, const char* texture) {}
    
    //Delete an animation from the resource manager
    virtual void deleteAnimation(const char * animation) {}

    //Add a light to the game
    virtual void addLight(IComponent* ptr);

    //Delete an object of the game
    virtual void deleteObject(IComponent* ptr);

    //Change the position of an object in-game
    virtual void updateObjectTransform(uint16_t id, GameObject::TransformationData transform);

    //Update game animations
    virtual void updateAnimations(float dTime) {};

    //Update single animation
    virtual void updateAnimation(IComponent* ptr) {};
    
    //Dont do nothing, only for the facade
    virtual void setClipping(bool b)    {};

    //add mesh lod
    virtual void addMeshLoD(int lvl, const char* mesh)  {};

    //Change mesh
    virtual bool changeMesh(int id, std::string newMesh)   {return false;};

    virtual void createParticleSystem(uint16_t id, const char* shape, glm::vec3 position, float radius, int birthrate, float particleLife,
                                        glm::vec3 birthDirection, glm::vec3 deathDirection, float variationDirection,
                                        float birthSize, float deathSize, float variationSize,
                                        glm::vec4 birthColor, glm::vec4 deathColor, float variationColor);
                                        
    virtual void deleteObject(uint16_t id);

    /////////////////////////////////////////
    // ANIMATIONS
    /////////////////////////////////////////
    virtual void stopAnimation(uint16_t id){}
    virtual void loopOnceAnimation(uint16_t id){}
    virtual void playAnimation(uint16_t id){}
    virtual void loopAnimation(uint16_t id){}
    virtual void resetAnimation(uint16_t id){}
    virtual void changeAnimation(uint16_t id, uint16_t animation){}
    virtual bool isAnimationPLaying(uint16_t){return false;}
    virtual void setFramerate(uint16_t, float framerate){}

    //==============================================================
    //  VISUAL INTERFACE
    //==============================================================
    ////////////
    //  HUD RELATED
    ////////////
    //Updates item icon in the HUD
    virtual void updateItemIcon();
    
    ////////////
    //  Image
    ////////////
    //Adds an image on the specific point given with the "color" specified
    virtual int32_t addImage( std::string img, glm::vec2 pos);

    //Substitutes an image with another in the same position
    virtual void changeImage(int32_t id, std::string img);

    //Deletes specified rectangle by its index in the rectangle array
    virtual void deleteImage(int32_t id);

    //Clean rectangles off of the screen
    virtual void cleanImages();

    /////////////
    //  Rectangle
    /////////////

    //Add rectangle of the given color and alpha channel, at the specified position with the given size
    virtual int32_t addRectangleColor(glm::vec2 pos, glm::vec2 size, int r, int g, int b, int a);

    //Change color of the rectangle known by the id given
    virtual void changeRectangleColor(int32_t id, int r, int g, int b, int a);
    
    //Deletes the rectangle with the passed id
    virtual void deleteRectangleColor(int32_t id);

    //Clean all rectangles off of the screen
    virtual void cleanRectangles();

    ////////////
    //  Text  //
    ////////////

    //Adds specified text at the specified point with specified size, with the specified color and font
    virtual int32_t addText( std::string text, glm::vec2 pos, int r, int g, int b, int a, glm::vec2 size, std::string font);

    //Changes the specified text with the given message
    virtual void changeText(int32_t id, std::string text);

    //Changes the font of the game
    virtual void changeFontText(int32_t id, std::string fontFile);

    //Changes the color of the text specified by the id
    virtual void changeColorText(int32_t id, int r, int g, int b, int a);

    //Changes the color of the background of the text specified by the id
    virtual void changeBackgroundColorText(int32_t id, int r, int g, int b, int a);

    //deletes text in the specified position of the text array
    virtual void deleteText(int32_t id);

    //Erase all text from the screen
    virtual void cleanTexts();

    //Erase all Visual interface elements off the screen
    virtual void cleanInterface();
    //Add SkyBox
    virtual void addSkybox(IComponent* ptr, std::string t, std::string bo, std::string l, std::string r, std::string f, std::string ba);

    ///////////////////////////////
    ///////      DEBUG      ///////    
    ///////////////////////////////
    
    //Funciones inútiles que deberían de ser vaporizadas en el infierno digital
    virtual void createRectangle2D(glm::vec2 pos, std::string img);

    virtual void deleteRectangle2D();
    
    virtual void createRectangleCol2D(glm::vec2 pos, std::string img);

    virtual void deleteRectangleCol2D();

    virtual void createTitleText(glm::vec2 pos, std::string text);

    virtual void deleteTitleText();

    virtual void setTitleText(std::string text);

    virtual void createDescriptionText(glm::vec2 pos, std::string text);

    virtual void deleteDescriptionText();

    virtual void setDescriptionText(std::string text);

    virtual void createSubTitleText(glm::vec2 pos, std::string text);

    virtual void deleteSubTitleText();

    virtual void setSubTitleText(std::string text);

    virtual void createSubDescriptionText(glm::vec2 pos, std::string text);

    virtual void deleteSubDescriptionText();

    virtual void setSubDescriptionText(std::string text);

    //==============================================================
    // VISUAL EFFECTS
    //==============================================================
    //Set the postprocessing state
    virtual void setPostProcessing(bool b){};
    
    //Set the current postprocessing option to render
    virtual void setPostProcessingOption(int o) {};
    
    //Set the scene to black and white
    virtual void setBlackAndWhite(bool b){};

    //Set the scene to neon visual
    virtual void setNeon(bool b){};

    //Set the neon factor
    virtual void setNeonFactor(float n){};

    //Set the blur effect
    virtual void setBlurEffect(bool b){};

    //Set the blur effect origin
    virtual void setBlurOrigin(float x, float y){};

    //Set the blur effect effect intensity
    virtual void setBlurIntensity(float i){};

    //Set the blur effect radius
    virtual void setBlurRadius(float r){};

    virtual void setBackface(float r){};
    virtual bool getBackface(){};

    virtual void setFrustum(float r){};
    virtual bool getFrustum(){};
    
private: 
    //==============================================================
    // PRIVATE FUNCTIONS
    //==============================================================

    //Update the logo video
    virtual void updateLogo();

    virtual void drawGUI();

    virtual void createItemIcon(glm::vec2 pos, std::string img);

    virtual void deleteItemIcon();


    //Get device
    irr::IrrlichtDevice* getDevice(){
        return device;
    };

    //Get videodriver
    irr::video::IVideoDriver* getVideoDriver(){
        return videoDriver;
    };

    //Get scenemanager
    irr::scene::ISceneManager* getSceneManager(){
        return sceneManager;
    };


    //==============================================================
    // PRIVATE DATA
    //==============================================================

    ////////////////////
    // MAIN DATA
    ////////////////////

    //Irrlicht own window
    irr::IrrlichtDevice* device;

    //Irrlicht scene manager
    irr::scene::ISceneManager* sceneManager;

    //Irrlicht video manager
    irr::video::IVideoDriver* videoDriver;

    //Irrlicht geometry creator
    const irr::scene::IGeometryCreator* geometryCreator;

    //Irrlicht node map
    std::map<uint16_t, irr::scene::ISceneNode*> nodeMap;

    //Irrlicht camera
    irr::scene::ICameraSceneNode* camera;

    ////////////////////
    // GUI DATA
    ////////////////////

    //Array of present GUI elements
    irr::s32 GUIId = 0;
    std::vector<irr::gui::IGUIImage*> GUIImageArray;
    std::vector<irr::gui::IGUIStaticText*> GUITextArray;
    std::vector<irr::gui::IGUIImage*> GUIRectangleColorArray;

    ////////////////////
    // *****TO ERASE*****
    ////////////////////
    irr::gui::IGUIStaticText* pos;

    irr::gui::IGUIStaticText* lap;

    irr::gui::IGUIStaticText* item;

    irr::gui::IGUIFont* font;

    std::vector<irr::gui::IGUIElement*> rectanglesAI;


    //Camera Debug
    irr::gui::IGUIElement* rectangle;

    irr::gui::IGUIElement* rectangleCol;

    irr::gui::IGUIStaticText* title;

    irr::gui::IGUIStaticText* description;

    irr::gui::IGUIStaticText* subTitle;

    irr::gui::IGUIStaticText* subDescription;

    irr::gui::IGUIElement* itemIMG;



};
