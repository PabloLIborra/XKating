#pragma once

#include <redpanda/RedPandaStudio.h>

#include <map>
#include <string>

#include "IRenderFacade.h"
#include "InputRedPanda.h"

#include "../GameManager/InputManager.h"


class RenderRedPanda : public IRenderFacade {

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    RenderRedPanda() : IRenderFacade() {}

    //Destructor
    virtual ~RenderRedPanda() {}

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openWindow();

    //Updates window info in the engine
    virtual void updateWindow();

    //Closes engine window
    virtual void closeWindow();

    rps::RedPandaStudio* getDevice() { return device; }

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
    virtual void addAnimation(IComponent* ptr);
    virtual void addAnimation(uint16_t id, const char * mesh, int frames, const char* texture);

    //Delete an animation from the resource manager
    virtual void deleteAnimation(const char * animation);
    
    //Add a light to the game
    virtual void addLight(IComponent* ptr);

    //Add an skybox to the game
    virtual void addSkybox(IComponent* ptr, std::string t, std::string bo, std::string l, std::string r, std::string f, std::string ba);

    //Delete an object of the game
    virtual void deleteObject(IComponent* ptr);
    virtual void deleteObject(uint16_t id);

    //Change the position of an object in-game
    virtual void updateObjectTransform(uint16_t id, GameObject::TransformationData transform);

    //Update game animations
    virtual void updateAnimations(float dTime);

    //Update single animation
    virtual void updateAnimation(IComponent* ptr);
    
    //Set active or inactive clipping
    virtual void setClipping(bool b)    {};

    //add mesh lod
    virtual void addMeshLoD(int lvl, const char* mesh);

    //Change mesh
    virtual bool changeMesh(int id, std::string newMesh);

    //Particles
    virtual void createParticleSystem(uint16_t id, const char* shape, glm::vec3 position, float radius, int birthrate, float particleLife,
                                        glm::vec3 birthDirection, glm::vec3 deathDirection, float variationDirection,
                                        float birthSize, float deathSize, float variationSize,
                                        glm::vec4 birthColor, glm::vec4 deathColor, float variationColor);


    /////////////////////////////////////////
    // ANIMATIONS
    /////////////////////////////////////////
    virtual void stopAnimation(uint16_t id);
    virtual void loopOnceAnimation(uint16_t id);
    virtual void playAnimation(uint16_t id);
    virtual void loopAnimation(uint16_t id);
    virtual void resetAnimation(uint16_t id);
    virtual void changeAnimation(uint16_t id, uint16_t animation);
    virtual bool isAnimationPLaying(uint16_t);
    virtual void setFramerate(uint16_t, float framerate);

    ////////////
    //  Image
    ////////////
    //Adds an image on the specific point given with the image specified
    virtual int32_t addImage( std::string img, glm::vec2 pos);

    //Substitutes an image with another at the same position
    virtual void changeImage(int32_t id, std::string img);

    //Deletes specified rectangle by its index in the rectangle array
    virtual void deleteImage(int32_t id);

    //Clean images off of the screen
    virtual void cleanImages();

    ////////////
    //  Rectangle
    ////////////

    //Add rectangle of the given color and alpha channel, at the specified position with the given size
    virtual int32_t addRectangleColor(glm::vec2 pos, glm::vec2 size, int r, int g, int b, int a);

    //Change color of the rectangle known by the id given
    virtual void changeRectangleColor(int32_t id, int r, int g, int b, int a);

    //Deletes the rectangle with the passed id
    virtual void deleteRectangleColor(int32_t id);

    //Clean all rectangles off of the screen
    virtual void cleanRectangles();

    ////////////
    //  Text
    ////////////

    //Adds specified text at the specified point with specified size, with the specified color and font
    virtual int32_t addText( std::string text, glm::vec2 pos = glm::vec2(0,0) , int r = 255, int g = 255, int b = 255, int a = 255, glm::vec2 size = glm::vec2(20,50), std::string font = "");

    //Changes the specified text with the given message
    virtual void changeText(int32_t id, std::string text);

    //Changes the font of the game
    virtual void changeFontText(int32_t id, std::string font);

    //Changes the color of the text specified by the id
    virtual void changeColorText(int32_t id, int r, int g, int b, int a);

    //Changes the color of the background of the text specified by the id
    virtual void changeBackgroundColorText(int32_t id, int r, int g, int b, int a);

    //deletes text in the specified position of the text array
    virtual void deleteText(int32_t id);

    //Erase all text from the screen
    virtual void cleanTexts();

    //Erase all visual interface elements from the screen
    virtual void cleanInterface();

    ///////////////////////////////
    ///////      DEBUG      ///////    
    ///////////////////////////////
    
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
    virtual void setPostProcessing(bool b);
    
    //Set the current postprocessing option to render
    virtual void setPostProcessingOption(int o);

    //Set the scene to black and white
    virtual void setBlackAndWhite(bool b);

    //Set the scene to neon visual
    virtual void setNeon(bool b);

    //Set the neon factor
    virtual void setNeonFactor(float n);

    //Set the blur effect
    virtual void setBlurEffect(bool b);

    //Set the blur effect origin
    virtual void setBlurOrigin(float x, float y);

    //Set the blur effect effect intensity
    virtual void setBlurIntensity(float i);

    //Set the blur effect radius
    virtual void setBlurRadius(float r);

    virtual void setBackface(float r)                        {        backface = r;            }
    bool getBackface()                                       {        return backface;         }

    virtual void setFrustum(float r)                         {        frustum = r;             }
    bool getFrustum()                                        {        return frustum;          }
    
private: 
    //Update the logo video
    virtual void updateLogo();

    virtual void drawGUI();

    virtual void createItemIcon(glm::vec2 pos, std::string img);

    virtual void deleteItemIcon();

    virtual void updateItemIcon();

    //RenderRedPanda own device
    rps::RedPandaStudio* device = nullptr;

    //RedPanda node map
    std::map<uint16_t, TNode*> nodeMap;
    std::map<uint16_t, TAnimation*> animationMap;

    //Value angleY camera
    float valueY;

    float sum;

    bool backface = false;
    bool frustum = false;

};
