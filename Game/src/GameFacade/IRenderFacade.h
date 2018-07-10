#pragma once

#include <iostream>
#include <string>

#include "../GameManager/InputManager.h"

//Window data structures
struct vec2d {
    int x;
    int y;
};

struct Window {
    vec2d size;
    bool fullscreen;
    bool vsync;
};

class IRenderFacade{

public:

    //==============================================================
    // Class Related functions
    //==============================================================

    //Constructor
    IRenderFacade() {}

    //Destructor
    virtual ~IRenderFacade() {}

    //Initializer
    void init(int x, int y, bool fScreen, bool vsync) {

        window.size.x = x;
        window.size.y = y;

        window.fullscreen = fScreen;
        window.vsync = vsync;

        inputManager = &InputManager::getInstance();

    }

    //==============================================================
    // Engine Related functions
    //==============================================================

    //Creates a window depending on the engine
    virtual void openWindow() = 0;

    //Updates window info in the engine
    virtual void updateWindow() = 0;

    //Closes engine window
    virtual void closeWindow() = 0;

    //==============================================================
    // Render Related functions
    //==============================================================

    //Renders all the scene
    virtual void renderDraw() = 0;

    //Add a camera to the game
    virtual void addCamera() = 0;

    //Update the current camera
    virtual void interpolateCamera(float accTime, float maxTime) = 0;
    virtual void setCameraTarget(glm::vec3 position, glm::vec3 target) = 0;

    //Add an object to the game
    virtual void addObject(IComponent* ptr) = 0;

    //Add an object to the game
    virtual void addObject(IComponent* ptr, float radius, float length, int tesselation, bool transparency) = 0;

    //Add an animation to the game
    virtual void addAnimation(IComponent* ptr) = 0;
    virtual void addAnimation(uint16_t id, const char * mesh, int frames, const char* texture) = 0;

    //Delete an animation from the resource manager
    virtual void deleteAnimation(const char * animation) = 0;

    //Add a light to the game
    virtual void addLight(IComponent* ptr) = 0;

    //Delete an object or light of the game
    virtual void deleteObject(IComponent* ptr) = 0;
    virtual void deleteObject(uint16_t id) = 0;

    //Change the position of an object in-game
    virtual void updateObjectTransform(uint16_t id, GameObject::TransformationData transform) = 0;

    //Update game animations
    virtual void updateAnimations(float dTime) = 0;

    //Update single animation
    virtual void updateAnimation(IComponent* ptr) = 0;
    
    //Dont so nothing in irrlicht, only works on rps
    virtual void setClipping(bool b) = 0;

    //add mesh lod
    virtual void addMeshLoD(int lvl, const char* mesh) = 0;

    //Change mesh
    virtual bool changeMesh(int id, std::string newMesh) = 0;

    //Particles
    virtual void createParticleSystem(uint16_t id, const char* shape, glm::vec3 position, float radius, int birthrate, float particleLife,
                                        glm::vec3 birthDirection, glm::vec3 deathDirection, float variationDirection,
                                        float birthSize, float deathSize, float variationSize,
                                        glm::vec4 birthColor, glm::vec4 deathColor, float variationColor) = 0;

    /////////////////////////////////////////
    // ANIMATIONS
    /////////////////////////////////////////
    virtual void stopAnimation(uint16_t id) = 0;
    virtual void loopOnceAnimation(uint16_t id) = 0;
    virtual void playAnimation(uint16_t id) = 0;
    virtual void loopAnimation(uint16_t id) = 0;
    virtual void resetAnimation(uint16_t id) = 0;
    virtual void changeAnimation(uint16_t id, uint16_t animation) = 0;
    virtual bool isAnimationPLaying(uint16_t) = 0;
    virtual void setFramerate(uint16_t, float framerate) = 0;

    //==============================================================
    //  VISUAL INTERFACE
    //==============================================================
    ////////////
    //  HUD RELATED
    ////////////
    //Updates item icon in the HUD
    virtual void updateItemIcon() = 0;
    
    ////////////
    //  Image
    ////////////
    //Adds an image on the specific point given with the image specified
    virtual int32_t addImage( std::string img, glm::vec2 pos) = 0;

    //Substitutes an image with another at the same position
    virtual void changeImage(int32_t id, std::string img) = 0;

    //Deletes specified rectangle by its index in the rectangle array
    virtual void deleteImage(int32_t id) = 0;

    //Clean images off of the screen
    virtual void cleanImages() = 0;

    ////////////
    //  Rectangle
    ////////////

    //Add rectangle of the given color and alpha channel, at the specified position with the given size
    virtual int32_t addRectangleColor(glm::vec2 pos, glm::vec2 size, int r, int g, int b, int a) = 0;

    //Change color of the rectangle known by the id given
    virtual void changeRectangleColor(int32_t id, int r, int g, int b, int a) = 0;

    //Deletes the rectangle with the passed id
    virtual void deleteRectangleColor(int32_t id) = 0;

    //Clean all rectangles off of the screen
    virtual void cleanRectangles() = 0;

    ////////////
    //  Text
    ////////////

    //Adds specified text at the specified point with specified size, with the specified color and font
    virtual int32_t addText( std::string text, glm::vec2 pos = glm::vec2(0,0) , int r = 255, int g = 255, int b = 255, int a = 255, glm::vec2 size = glm::vec2(20,50), std::string font = "") = 0;

    //Changes the specified text with the given message
    virtual void changeText(int32_t id, std::string text) = 0;

    //Changes the font of the game
    virtual void changeFontText(int32_t id, std::string font) = 0;

    //Changes the color of the text specified by the id
    virtual void changeColorText(int32_t id, int r, int g, int b, int a) = 0;

    //Changes the color of the background of the text specified by the id
    virtual void changeBackgroundColorText(int32_t id, int r, int g, int b, int a) = 0;

    //deletes text in the specified position of the text array
    virtual void deleteText(int32_t id) = 0;

    //Erase all text from the screen
    virtual void cleanTexts() = 0;

    //Erase all visual interface elements from the screen
    virtual void cleanInterface() = 0;
    //Add SkyBox
    virtual void addSkybox(IComponent* ptr, std::string t, std::string bo, std::string l, std::string r, std::string f, std::string ba) = 0;

    ///////////////////////////////
    ///////      DEBUG      ///////    
    ///////////////////////////////
    //Update logo video
    virtual void updateLogo() = 0;

    virtual void createRectangle2D(glm::vec2 pos, std::string img) = 0;

    virtual void deleteRectangle2D()  = 0;
    
    virtual void createRectangleCol2D(glm::vec2 pos, std::string img) = 0;

    virtual void deleteRectangleCol2D() = 0;

    virtual void createTitleText(glm::vec2 pos, std::string text) = 0;

    virtual void deleteTitleText() = 0;

    virtual void setTitleText(std::string text) = 0;

    virtual void createDescriptionText(glm::vec2 pos, std::string text) = 0;

    virtual void deleteDescriptionText() = 0;

    virtual void setDescriptionText(std::string text) = 0;

    virtual void createSubTitleText(glm::vec2 pos, std::string text) = 0;

    virtual void deleteSubTitleText() = 0;

    virtual void setSubTitleText(std::string text) = 0;

    virtual void createSubDescriptionText(glm::vec2 pos, std::string text) = 0;

    virtual void deleteSubDescriptionText() = 0;

    virtual void setSubDescriptionText(std::string text) = 0;
    
    virtual void drawGUI() = 0;

    virtual void createItemIcon(glm::vec2 pos, std::string img)  = 0;

    virtual void deleteItemIcon()  = 0;

    //==============================================================
    // VISUAL EFFECTS
    //==============================================================
    //Set the postprocessing state to active or not
    virtual void setPostProcessing(bool b) = 0;

    //Set the current postprocessing option to render
    virtual void setPostProcessingOption(int o) = 0;

    //Set the scene to black and white
    virtual void setBlackAndWhite(bool b) = 0;

    //Set the scene to neon visual
    virtual void setNeon(bool b) = 0;

    //Set the neon factor
    virtual void setNeonFactor(float n) = 0;

    //Set the blur effect
    virtual void setBlurEffect(bool b) = 0;

    //Set the blur effect origin
    virtual void setBlurOrigin(float x, float y) = 0;

    //Set the blur effect effect intensity
    virtual void setBlurIntensity(float i) = 0;

    //Set the blur effect radius
    virtual void setBlurRadius(float r) = 0;

    virtual void setBackface(float r) = 0;
    virtual bool getBackface() = 0;

    virtual void setFrustum(float r) = 0;
    virtual bool getFrustum() = 0;

    //==============================================================
    // Window Related functions
    //==============================================================

    //Resizes the window
    void resizeWindow(int x, int y) {
        window.size.x = x;
        window.size.y = y;

        updateWindow();
    }

    //Changes fullscreen state and returns it
    bool fullscreenWindow() {
         
        if(window.fullscreen)
            window.fullscreen = false;
        else 
            window.fullscreen = true;

        updateWindow();

        return window.fullscreen;

    }

    //Changes sync state and return it
    bool syncWindow() {

        if(window.vsync)
            window.vsync = false;
        else 
            window.vsync = true;

        updateWindow();

        return window.vsync;
    }

    Window& getWindow(){
        return window;
    }

    //Sets the gameObject to be targeted by the camera
    void setCameraTarget(GameObject& newCameraTarget) {
        cameraTarget = &newCameraTarget;
    }
    //Returns the gameObject to be targeted by the camera
    GameObject& getCameraTarget() {
        return *cameraTarget;
    }


protected:

    //Window declaration
    Window window;
    //InputManager declaration
    InputManager* inputManager = nullptr;
    //Camera target
    GameObject* cameraTarget = nullptr;

};