#include "TResourceNormalTexture.h"

//Destructor of the resource
TResourceNormalTexture::~TResourceNormalTexture()
{
    glDeleteTextures(1, &textureID);
}

//Loader of the resource. We free the memory after giving opengl the data of the texture
bool TResourceNormalTexture::loadResource()
{
    //SFML Class to store our texture
    sf::Image* texture = new sf::Image();
    //Attempt to load the file
    if(texture->loadFromFile(name))
    {
        active=true;
        //Get his width and height
        int sizeX = texture->getSize().x;
        int sizeY = texture->getSize().y;

        //Generate a OpenGL texture for later
        glGenTextures(1, &textureID);

        glActiveTexture(GL_TEXTURE1);

        //Bind and enable the texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        //Set all the parameters of the texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

        //Set the texture to be drawn
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)sizeX, (GLsizei)sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getPixelsPtr());

        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);

        delete texture;

        return true;
    }
    return false;
}

//Draws the texture, restoring the active texture spot after the draw
void TResourceNormalTexture::draw()
{
    if(active)
    {
        glActiveTexture(GL_TEXTURE1);
        //Bind and enable the texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        glActiveTexture(GL_TEXTURE0);
    }
}