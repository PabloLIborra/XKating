#include "TResourceSkybox.h"

//Constructor of the skybox
TResourceSkybox::TResourceSkybox()
{
    for(int i = 0; i < 6; i++)
    {
        sf::Image* img = new sf::Image();
        textures.push_back(img);
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

//Loads the resource of the skybox
bool TResourceSkybox::loadResource(const char* route, int i)
{
    if(i>= 0 && i<6)
    {
        if(textures[i]->loadFromFile(route))
        {
            return true;
        }
    }
    return false;
}

//Draws the skybox
void TResourceSkybox::draw()
{
    glUniformMatrix4fv(view, 1, GL_FALSE, &TEntity::viewMatrix()[0][0]);

    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

//Initialices the skybox
bool TResourceSkybox::initSkybox()
{
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, textures[0]->getSize().x, textures[0]->getSize().y);

    for(int i = 0; i < 6; i++)
    {
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, textures[i]->getSize().x, textures[i]->getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, textures[i]->getPixelsPtr());
    }
    return true;
}

