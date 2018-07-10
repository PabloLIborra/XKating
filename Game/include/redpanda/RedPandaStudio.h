#pragma once

//=========================================================================
//                               INCLUDES                                //
//=========================================================================
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <chrono>

#include "RedPanda.h"

//=========================================================================
//                        RED PANDA STUDIO CLASS                         //
//=========================================================================

namespace rps {

//Enumerators
enum PPOption{
    DEFAULT,
    BLACK_WHITE,
    NEGATIVE,
    HIGH_CONTRAST,
    MEDIAN,
    NEON,
    BLUR
};

class RedPandaStudio{

public:

    RedPandaStudio() {}
    ~RedPandaStudio() {
        //Delete frame buffers
        glDeleteFramebuffers(1, &depthBuffer);
    }

    //////////////////////////////
    //  DEVICE CONSTRUCTORS
    static RedPandaStudio& createDevice(int width, int height, int depth, int framerate, bool vsync, bool fullscreen);
    void updateDevice();
    void dropDevice();

    //////////////////////////////
    //  NODE CONSTRUCTORS

    //Creates an object and returns a TMesh
    TNode* createObjectNode(TNode* parent, glm::vec3 position, const char* mesh); 
    //Creates an animated mesh object and returns a pointer to the TNode that cointains the TAnimation
    TNode* createAnimatedNode(TNode* parent, glm::vec3 pos, const char* animation, bool loop, int frames, double framerate, const char* texture);
    //Creates a camera and returns a TCamera
    TNode* createCamera(TNode* parent, glm::vec3 position, glm::vec3 target);
    //Creates a light and returns a TLight
    TNode* createLight(TNode* parent, glm::vec3 position, glm::vec3 intensity);
    //Creates a spotlight and returns a TSpotlight
    TNode* createSpotlight(TNode* parent, glm::vec3 position, glm::vec3 intensity, glm::vec3 direction, float cutoff);
    //Creates a billboard and returns a reference to it
    TBillboard* createBillboard(const char* n, glm::vec3 p);
    //Creates an emitter and return a TEmitter
    TNode* createEmitter(TNode* parent, const char* shape, glm::vec3 nPosition, float nRadius, 
            int nBirthrate, float nParticleLife, glm::vec3 nBirthDirection, float nBirthSize, glm::vec4 nBirthColor);

    TNode* createEmitter(TNode* parent, const char* shape, glm::vec3 nPosition, float nRadius, int nBirthrate, float nParticleLife,
            glm::vec3 nBirthDirection, glm::vec3 nDeathDirection, float nVariationDirection, float nBirthSize, float nDeathSize, 
            float nVariationSize, glm::vec4 nBirthColor, glm::vec4 nDeathColor, float nVariationColor);
    //Deletes a mesh, camera or light, given a TMesh, TCamera or TLight
    void deleteObject(TNode* leaf);

    void updateCamera(glm::vec3 position, glm::vec3 target);

    void setGUIDrawFunction(void (*)());

    ///////////////////////////////////////////
    // RESOURCE DESTRUCTORS
    void deleteAnimation(const char* n);

    ////////////////////////////////////////
    //  GRAPHICS OPTIONS AND PARAMETERS
    //Initializes all parameters and programs to operate with the post processing
    void initPostProcessing();
    
    //Initializes the draw of the postprocessing (frame buffer)
    void initDrawPostProcessing();

    //Draws the post processing on a quad
    void quadDrawPostProcessing();

    //Resize the texture to the actual window size
    void resizePostProcessing(float x, float y);

    //Initializes all parameters and programs to operate with the shadow mapping
    void initShadowMapping();

    //Draws the shadow mapping
    void drawShadowMapping();

    //Activates and deactivates the culling. The second parameter determinates which type of faces are culled (when deactivating the culling, that parameter doesnt matter)
    void setCulling(bool b, GLenum e);

    //Activates and deactivates the CPU-frustum culling. Actually is done using bounding boxed for every Mesh and OBJ
    void setFrustumCulling(bool b);

    //To add some mesh into lod array
    void addMeshLoD(int lvl, const char* mesh);    

    //////////////////////////////
    //  GETTERS
    SDL_Window* getWindow()                             {   return window;               }
    TNode* getSceneRoot()                               {   return scene;                }
    ResourceManager* getResourceManager()               {   return resourceManager;      }
    SDL_GLContext* getContext()                         {   return context;              }  
    bool getSilhouette()                                {   return silhouetteActivated;  }


    //////////////////////////////
    //  SETTERS
    void setAmbient(float v)                            {   glUniform1f(ambientID, v);   }  
    void setWindow(SDL_Window* rw )                     {   window = rw;                 }   
    void setSilhouette(bool b)                          {   silhouetteActivated = b;     }
    void setNLightsForward(int i)                       {   nLightsForward = i;          }
    void setNLightsBack(int i)                          {   nLightsBack = i;             }


    //  POSTPROCESSING GETTERS & SETTERS
    bool getPPActive()                                  {   return postProcessingActive; }
    void setPPActive(bool b)                            {   postProcessingActive = b;    }
    void setPPOption(PPOption  o)                       {   postProcessingOption = o;    }
    PPOption getPPOption()                              {   return postProcessingOption; }
    void setPPOffset(int  o)                            {   offset = o;                  }
    void setPPBlurPos(float x, float y)                 {   blur_x = x; blur_y = y;      }
    void setPPBlurStrength(float  s)                    {   blur_strength = s;           }
    void setPPBlurDist(float  d)                        {   blur_dist = d;               }
    void setPPNeonFactor(float  n)                      {   neonFactor = n;              }   
    

private: 

    //////////////////////////////
    //  FUNCTIONS
    void initSDLWindow(int width, int height, int depth, int framerate, bool vsync, bool fullscreen);
    void initOpenGL();
    void initScene();
    void renderLights();
    void renderAllLights();
    void updateActiveLights();
    void updateActiveNearLights();
    void renderCamera();
    void renderBillboards();
    void renderParticles();
    void updateParticles();
    void calculateNodeTransform(TNode* node, glm::mat4& mat);  //Given a node, returns its accumulated transform. Should receive an identity as input
    TNode* addRotScaPos(TNode* parent, glm::vec3 position); //Returns the Position Node
    void deleteNode(TNode* node); //Deletes a node and all his children
    

    //=========================
    //  GENERAL VARIABLES
    //=========================
    SDL_Window* window;
    SDL_GLContext* context;
    TNode *scene;
    ResourceManager *resourceManager;

    //=========================
    //  CAMERA/LIGHTS
    //=========================
    TNode *camera;
    std::vector<TNode*> lights;
    std::vector<TNode*> spotlights;
    int nLightsBack = 2;
    int nLightsForward = 3;
    int currentLight = 0;
    GLuint ambientID;

    //=========================
    //  BILLBOARDS
    //=========================
    //Vector containing all the billboards in the scene
    std::vector<TBillboard*> billboards;
    //Billboard shader
    GLuint billboardID;

    //=========================
    //  PARTICLES
    //=========================
    std::vector<TNode*> emitters;
    GLuint particlesID;
    GLuint paticlesVertexArray;

    //=========================
    //  SKYBOX
    //=========================
    //Skybox
    TResourceSkybox*  skybox;
    //Skybox shader
    GLuint skyboxID;
    //SKybox vertex array
    GLuint skyVertexArray;

    //=========================
    //  POST-PROCESSING
    //=========================
    bool postProcessingActive = false; //Boolean that checks if postprocessing is activated

	GLuint postProcessingBuffer;   //Texture (color) buffer
	GLuint renderBuffer;           //Render buffer ID
	GLuint colorMap;               //Texture in which we paint the scene
    GLuint processingID;           //Shadow map program ID
    GLuint postprocessing_sampler; //Sampler2D of the texture rendered to the quad
    GLuint processingQuadVAO, processingQuadVBO; //Quad indexes
    
    //Window size (used to paint over the quad)
    int windowWidth = 1024;
    int windowHeight = 1024;

    //Option settings
    PPOption postProcessingOption = PPOption::DEFAULT; //Integer that chooses the postprocessing option

    float neonFactor = 0;         //Negative factor of the neon
    float offset = 1/300;         //Kernel offset of pixel picking
    float blur_x = 0.f;           //blur X position (-1 to 1)
    float blur_y = 0.f;           //blur Y position (-1 to 1)
    float blur_strength = 2.2;    //blur strength
    float blur_dist = 1.f;        //blur distance of pixel picking

    //Options ID's
    GLuint optionID;              
    GLuint neonFactorID;         
    GLuint offsetID;
    GLuint blur_xID;
    GLuint blur_yID;
    GLuint blur_strengthID;
    GLuint blur_distID;
    //=========================
    //  SHADOWMAP
    //=========================
	GLuint depthBuffer;                  //Depth buffer
	GLuint depthMap;                     //Texture in which we paint the scene
    GLuint shadowID;                     //Shadow map program ID
    GLuint shadowQuadID;                 //TESTING
	GLuint shadowRenderBuffer;           //Render buffer ID
    GLuint shadowQuadVAO, shadowQuadVBO; //Quad indexes
    GLuint shadow_sampler;               //Sampler2D of the texture rendered to the quad
    GLuint shadowMap_sampler;            //Sampler2D of the depth map in the normal shader

    //Shadow map texture size
    int shadowWidth = 1024;
    int shadowHeight = 1024;

    //=========================
    //  TIME
    //=========================
    //Chrono
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
    double fps = 0;
    bool firstUpdate = true;

    //Chrono flag
    bool showFPS = false;

    //=========================
    //  Silhouette
    //=========================
    bool silhouetteActivated = false;
    GLuint silFlagIdentifier;

};

//Transformation facade
//Input node has to be of type Mesh/Light/Camera
void translateNode(TNode* node, glm::vec3 position);
void rotateNode(TNode* node, glm::vec3 rotation);
void scaleNode(TNode* node, glm::vec3 scale);


}
